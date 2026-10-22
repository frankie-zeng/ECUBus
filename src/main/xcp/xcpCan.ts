/**
 * XCP-on-CAN transport binding.
 *
 * Implements {@link XcpTransport} on top of the existing CAN hardware layer
 * ({@link CAN_SOCKET} / {@link CanBase}). The raw XCP packet is carried directly
 * in the CAN frame data field: command messages use the TX identifier, responses
 * are received on the RX identifier.
 *
 * @module xcpCan
 * @category XCP
 */

import { CAN_SOCKET, CanBase } from '../docan/base'
import {
  CAN_ERROR_ID,
  CAN_ID_TYPE,
  CanError,
  CanMsgType,
  getDlcByLen,
  getLenByDlc
} from '../share/can'
import { isCtoResponse } from './xcpProtocol'
import { XcpTraceHook } from './xcpTrace'
import { XcpTransport } from './xcpMaster'

/**
 * XCP-on-CAN addressing configuration.
 * @category XCP
 */
export interface XcpCanAddr {
  /** Optional connection name (for logging / identification). */
  name?: string
  /** CAN identifier the master sends commands (CMD/STIM) on. */
  canIdCmd: number
  /** CAN identifier the slave sends responses (RES/DAQ) on. */
  canIdResp: number
  /** Standard (11-bit) or extended (29-bit) identifiers. */
  extended?: boolean
  /** Use CAN-FD frames. */
  canfd?: boolean
  /** Use bit-rate switching (CAN-FD only). */
  brs?: boolean
  /** Pad every command frame to a fixed length. */
  padding?: boolean
  /** Padding byte value (default 0x00). */
  paddingValue?: number
  /**
   * Fixed frame length when {@link XcpCanAddr.padding} is enabled
   * (classic CAN default 8).
   */
  dlc?: number
}

function toMsgType(addr: XcpCanAddr): CanMsgType {
  return {
    idType: addr.extended ? CAN_ID_TYPE.EXTENDED : CAN_ID_TYPE.STANDARD,
    brs: !!addr.brs,
    canfd: !!addr.canfd,
    remote: false
  }
}

/**
 * XCP transport over a single CAN channel.
 * @category XCP
 */
export class XcpCanTransport implements XcpTransport {
  private readonly txSocket: CAN_SOCKET
  private readonly rxSocket: CAN_SOCKET
  private readonly addr: XcpCanAddr
  private closed = false
  /** Command responses (RES/ERR) that arrived ahead of the waiter. */
  private responses: Buffer[] = []
  /** DAQ, EVENT and SERV packets. */
  private indications: Buffer[] = []
  /** Serializes socket reads so a command wait and a DAQ read cannot race. */
  private tail: Promise<unknown> = Promise.resolve()

  constructor(
    base: CanBase,
    addr: XcpCanAddr,
    private readonly trace?: XcpTraceHook
  ) {
    this.addr = addr
    const msgType = toMsgType(addr)
    this.txSocket = new CAN_SOCKET(base, addr.canIdCmd, msgType, { name: addr.name })
    this.rxSocket = new CAN_SOCKET(base, addr.canIdResp, msgType, { name: addr.name })
  }

  private frame(packet: Buffer): Buffer {
    if (!this.addr.padding) return packet
    const target = this.addr.canfd
      ? getLenByDlc(getDlcByLen(Math.max(packet.length, 1), true), true)
      : Math.max(this.addr.dlc ?? 8, packet.length)
    if (packet.length >= target) return packet
    const pad = Buffer.alloc(target - packet.length, this.addr.paddingValue ?? 0)
    return Buffer.concat([packet, pad])
  }

  private exclusive<T>(fn: () => Promise<T>): Promise<T> {
    const run = this.tail.then(fn, fn)
    this.tail = run.then(
      () => undefined,
      () => undefined
    )
    return run
  }

  /** Move frames already queued on the socket into the RES / DAQ buckets. */
  private drainSocketBuffer(): void {
    while (this.rxSocket.recvBuffer.length) {
      const val = this.rxSocket.recvBuffer.shift()
      if (!val || val instanceof CanError) continue
      this.classify(val.data, val.ts)
    }
  }

  private classify(packet: Buffer, ts?: number): void {
    this.trace?.rx(packet, ts)
    if (isCtoResponse(packet)) this.responses.push(packet)
    else this.indications.push(packet)
  }

  private async readFrame(timeout: number): Promise<{ data: Buffer; ts: number } | null> {
    try {
      return await this.rxSocket.read(timeout)
    } catch (err) {
      if (err instanceof CanError && err.errorId === CAN_ERROR_ID.CAN_READ_TIMEOUT) return null
      throw err
    }
  }

  async request(request: Buffer, timeout = 1000): Promise<Buffer> {
    if (this.closed) throw new Error('XCP CAN transport is closed')
    return this.exclusive(async () => {
      // A previous timeout may have left a RES in the queue. DAQ frames stay.
      this.drainSocketBuffer()
      this.responses = []
      const txTs = await this.txSocket.write(this.frame(request))
      this.trace?.tx(request, txTs)
      const deadline = Date.now() + timeout
      while (this.responses.length === 0) {
        const left = deadline - Date.now()
        if (left <= 0) throw new Error('XCP CAN response timeout')
        const frame = await this.readFrame(left)
        if (!frame) throw new Error('XCP CAN response timeout')
        // DAQ/EVENT frames that share the response CAN id must not be parsed
        // as the command response.
        this.classify(frame.data, frame.ts)
      }
      return this.responses.shift() as Buffer
    })
  }

  /**
   * Wait for one DAQ, EVENT or SERV frame on the response CAN id.
   * Resolves `null` on timeout.
   */
  async readIndication(timeout = 1000): Promise<Buffer | null> {
    if (this.closed) throw new Error('XCP CAN transport is closed')
    return this.exclusive(async () => {
      this.drainSocketBuffer()
      if (this.indications.length > 0) return this.indications.shift() as Buffer
      const deadline = Date.now() + timeout
      while (this.indications.length === 0) {
        const left = deadline - Date.now()
        if (left <= 0) return null
        const frame = await this.readFrame(left)
        if (!frame) return null
        this.classify(frame.data, frame.ts)
      }
      return this.indications.shift() as Buffer
    })
  }

  async send(request: Buffer): Promise<void> {
    if (this.closed) throw new Error('XCP CAN transport is closed')
    const txTs = await this.txSocket.write(this.frame(request))
    this.trace?.tx(request, txTs)
  }

  close(): void {
    if (this.closed) return
    this.closed = true
    this.txSocket.close()
    this.rxSocket.close()
    this.trace?.close()
  }
}
