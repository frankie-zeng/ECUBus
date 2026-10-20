/**
 * XCP-on-Ethernet (TCP) transport binding.
 *
 * The framing is the same 4-byte header as UDP (little-endian length, then a
 * little-endian counter), but the bytes are a stream: one read can split a
 * packet or carry several. XCPlite's TCP server accepts a single connection
 * and reads one header plus that many payload bytes per command.
 *
 * @module xcpTcp
 * @category XCP
 */

import net from 'net'
import { isCtoResponse } from './xcpProtocol'
import { XcpTransport } from './xcpMaster'
import { encodeEthPacket } from './xcpUdp'

/**
 * Pull every complete XCP-on-Ethernet frame out of a stream buffer.
 * `offset` is how far the caller may discard. A short tail stays buffered.
 * @category XCP
 */
export function takeEthFrames(buffer: Buffer): { frames: Buffer[]; offset: number } {
  const frames: Buffer[] = []
  let i = 0
  while (i + 4 <= buffer.length) {
    const len = buffer.readUInt16LE(i)
    if (len === 0) {
      const aligned = (i + 3) & ~3
      if (aligned !== i && aligned + 4 <= buffer.length && buffer.readUInt16LE(aligned) !== 0) {
        i = aligned
        continue
      }
      if (aligned !== i && aligned + 4 > buffer.length) break
      break
    }
    if (i + 4 + len > buffer.length) break
    frames.push(Buffer.from(buffer.subarray(i + 4, i + 4 + len)))
    i += 4 + len
  }
  return { frames, offset: i }
}

/**
 * XCP transport over TCP, used for XCPlite slaves built with TCP (for example
 * `struct_demo` and `point_cloud_demo`).
 * @category XCP
 */
export class XcpTcpTransport implements XcpTransport {
  private socket: net.Socket
  private rx = Buffer.alloc(0)
  private txCounter = 0
  private responses: Buffer[] = []
  private indications: Buffer[] = []
  private waiters: Array<() => void> = []
  private closed = false
  private tail: Promise<unknown> = Promise.resolve()
  private readonly ready: Promise<void>

  constructor(
    private readonly host: string,
    private readonly port: number
  ) {
    this.socket = net.connect({ host, port })
    this.ready = new Promise((resolve, reject) => {
      const onError = (err: Error) => reject(err)
      this.socket.once('connect', () => {
        this.socket.off('error', onError)
        resolve()
      })
      this.socket.once('error', onError)
    })
    this.socket.on('data', (chunk: Buffer) => {
      const merged = this.rx.length === 0 ? Buffer.from(chunk) : Buffer.concat([this.rx, chunk])
      this.rx = merged
      const { frames, offset } = takeEthFrames(this.rx)
      if (offset > 0) this.rx = Buffer.from(this.rx.subarray(offset))
      for (const frame of frames) {
        if (isCtoResponse(frame)) this.responses.push(frame)
        else this.indications.push(frame)
      }
      if (frames.length) this.wake()
    })
    this.socket.on('error', () => {
      /* the next request reports the timeout if the socket dies */
    })
    this.socket.on('close', () => {
      this.closed = true
      this.wake()
    })
  }

  private wake(): void {
    const pending = this.waiters.splice(0)
    for (const waiter of pending) waiter()
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
      this.socket.write(frame, (err) => {
        if (err) reject(err)
        else resolve()
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
        if (queue.length > 0) finish()
      })
    }
  }

  async request(request: Buffer, timeout = 1000): Promise<Buffer> {
    if (this.closed) throw new Error('XCP TCP transport is closed')
    return this.exclusive(async () => {
      this.responses = []
      await this.sendPacket(request)
      const packet = await this.take('response', timeout)
      if (!packet) throw new Error('XCP TCP response timeout')
      return packet
    })
  }

  async readIndication(timeout = 1000): Promise<Buffer | null> {
    if (this.closed) throw new Error('XCP TCP transport is closed')
    return this.exclusive(() => this.take('indication', timeout))
  }

  async send(request: Buffer): Promise<void> {
    if (this.closed) throw new Error('XCP TCP transport is closed')
    await this.sendPacket(request)
  }

  close(): void {
    if (this.closed) return
    this.closed = true
    this.wake()
    this.socket.destroy()
  }
}
