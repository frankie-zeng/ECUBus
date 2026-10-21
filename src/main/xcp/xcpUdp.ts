/**
 * XCP-on-Ethernet (UDP) transport binding.
 *
 * XCPlite and the ASAM XCP-on-Ethernet layer put a 4-byte header in front of
 * every packet: little-endian payload length, then a little-endian counter.
 * One datagram may carry several packets. EVENT and SERV packets are not the
 * response to a command; they are queued for {@link XcpUdpTransport.readIndication}.
 *
 * @module xcpUdp
 * @category XCP
 */

import dgram from 'dgram'
import { isCtoResponse } from './xcpProtocol'
import { XcpTraceHook } from './xcpTrace'
import { XcpTransport } from './xcpMaster'

/**
 * Split one XCP-on-Ethernet datagram into XCP packets.
 * A zero length marks padding / end of the datagram. If that padding hides
 * another header on the next 4-byte boundary, parsing continues there.
 * @category XCP
 */
export function decodeEthDatagram(datagram: Buffer): Buffer[] {
  const packets: Buffer[] = []
  let i = 0
  while (i + 4 <= datagram.length) {
    const len = datagram.readUInt16LE(i)
    if (len === 0) {
      const aligned = (i + 3) & ~3
      if (aligned !== i && aligned + 4 <= datagram.length && datagram.readUInt16LE(aligned) !== 0) {
        i = aligned
        continue
      }
      break
    }
    if (len > datagram.length - i - 4) break
    packets.push(Buffer.from(datagram.subarray(i + 4, i + 4 + len)))
    i += 4 + len
  }
  return packets
}

/** Build one UDP payload: 4-byte transport header plus the XCP packet. @category XCP */
export function encodeEthPacket(counter: number, payload: Buffer): Buffer {
  const frame = Buffer.alloc(4 + payload.length)
  frame.writeUInt16LE(payload.length, 0)
  frame.writeUInt16LE(counter & 0xffff, 2)
  payload.copy(frame, 4)
  return frame
}

/**
 * XCP transport over UDP, used to talk to an XCPlite (or any XCP-on-Ethernet) slave.
 * @category XCP
 */
export class XcpUdpTransport implements XcpTransport {
  private socket: dgram.Socket
  private txCounter = 0
  private responses: Buffer[] = []
  private indications: Buffer[] = []
  private waiters: Array<(packet: Buffer | null) => void> = []
  private closed = false
  private tail: Promise<unknown> = Promise.resolve()
  private readonly ready: Promise<void>

  constructor(
    private readonly host: string,
    private readonly port: number,
    private readonly trace?: XcpTraceHook
  ) {
    this.socket = dgram.createSocket('udp4')
    this.ready = new Promise((resolve) => {
      this.socket.once('listening', () => resolve())
    })
    this.socket.on('message', (msg) => {
      for (const packet of decodeEthDatagram(msg)) {
        this.trace?.rx(packet)
        if (isCtoResponse(packet)) this.responses.push(packet)
        else this.indications.push(packet)
      }
      this.wake()
    })
    this.socket.on('error', () => {
      /* the next request reports the timeout if the socket dies */
    })
    this.socket.bind(0, '127.0.0.1')
  }

  private wake(): void {
    const pending = this.waiters.splice(0)
    for (const waiter of pending) waiter(null)
  }

  private exclusive<T>(fn: () => Promise<T>): Promise<T> {
    const run = this.tail.then(fn, fn)
    this.tail = run.then(
      () => undefined,
      () => undefined
    )
    return run
  }

  private async sendPacket(payload: Buffer): Promise<void> {
    await this.ready
    const frame = encodeEthPacket(this.txCounter, payload)
    this.txCounter = (this.txCounter + 1) & 0xffff
    await new Promise<void>((resolve, reject) => {
      this.socket.send(frame, this.port, this.host, (err) => {
        if (err) reject(err)
        else {
          this.trace?.tx(payload)
          resolve()
        }
      })
    })
  }

  private async take(kind: 'response' | 'indication', timeout: number): Promise<Buffer | null> {
    const queue = kind === 'response' ? this.responses : this.indications
    const deadline = Date.now() + timeout
    // eslint-disable-next-line no-constant-condition
    while (true) {
      if (queue.length > 0) return queue.shift() as Buffer
      if (this.closed) return null
      const left = deadline - Date.now()
      if (left <= 0) return null
      await new Promise<void>((resolve) => {
        let settled = false
        const finish = () => {
          if (settled) return
          settled = true
          clearTimeout(timer)
          this.waiters = this.waiters.filter((w) => w !== onWake)
          resolve()
        }
        const timer = setTimeout(finish, left)
        const onWake = () => finish()
        this.waiters.push(onWake)
        // A datagram may have arrived between the length check and registration.
        if (queue.length > 0) finish()
      })
    }
  }

  async request(request: Buffer, timeout = 1000): Promise<Buffer> {
    if (this.closed) throw new Error('XCP UDP transport is closed')
    return this.exclusive(async () => {
      // Drop a stale RES left behind by a timed-out command. DAQ stays queued.
      this.responses = []
      await this.sendPacket(request)
      const packet = await this.take('response', timeout)
      if (!packet) throw new Error('XCP UDP response timeout')
      return packet
    })
  }

  /** Wait for one DAQ, EVENT or SERV packet. Resolves `null` on timeout. */
  async readIndication(timeout = 1000): Promise<Buffer | null> {
    if (this.closed) throw new Error('XCP UDP transport is closed')
    return this.exclusive(() => this.take('indication', timeout))
  }

  async send(request: Buffer): Promise<void> {
    if (this.closed) throw new Error('XCP UDP transport is closed')
    await this.sendPacket(request)
  }

  close(): void {
    if (this.closed) return
    this.closed = true
    this.wake()
    this.socket.close()
    this.trace?.close()
  }
}
