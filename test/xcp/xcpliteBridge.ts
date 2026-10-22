/**
 * Bridge an XCPlite XCP-on-UDP slave onto SIMULATE_CAN as an external node.
 *
 * XCPlite speaks only Ethernet. This process is the UDP master from the slave's
 * point of view and a JSON-RPC client from EcuBus-Pro's point of view. It joins
 * the virtual bus through the TCP JSON-RPC server that starts when a project
 * simulate device is open (`setProjectSimulateCount`), using `Can.Init` on a
 * free handle. It does not construct `SIMULATE_CAN` itself.
 *
 * Each XCP packet is one CAN-FD frame on the command / response ids. The 4-byte
 * Ethernet header stays on the UDP side. CONNECT's MAX_CTO / MAX_DTO are clamped
 * to 64 so the XCP-on-CAN master does not build a packet that cannot fit in a
 * CAN-FD frame.
 */
import dgram from 'dgram'
import net from 'net'
import { decodeEthDatagram, encodeEthPacket } from 'src/main/xcp/xcpUdp'

const CAN_FD_MAX = 64

const FIXED_CRO_LENGTH: Record<number, number> = {
  0xff: 2,
  0xfe: 1,
  0xfd: 1,
  0xfc: 1,
  0xfb: 1,
  0xfa: 2,
  0xf9: 4,
  0xf8: 3,
  0xf6: 8,
  0xf5: 2,
  0xf4: 8,
  0xf3: 8,
  0xec: 6,
  0xeb: 4,
  0xea: 3,
  0xe9: 1,
  0xe8: 5,
  0xe7: 4,
  0xe6: 3,
  0xe5: 3,
  0xe4: 5,
  0xe3: 4,
  0xe2: 6,
  0xe1: 8,
  0xe0: 8,
  0xdf: 4,
  0xde: 4,
  0xdd: 2,
  0xdc: 1,
  0xdb: 1,
  0xda: 1,
  0xd9: 1,
  0xd8: 4,
  0xd7: 4,
  0xd6: 1,
  0xd5: 4,
  0xd4: 5,
  0xd3: 6,
  0xd2: 1,
  0xd1: 8,
  0xcf: 1,
  0xce: 1,
  0xcd: 3,
  0xcc: 4,
  0xcb: 5,
  0xc8: 8,
  0xc0: 2
}

/** XCP command length inside a CAN frame that may have been padded to the DLC. */
export function xcpCroLength(frame: Buffer): number {
  const cmd = frame[0]
  const n = frame[1] ?? 0
  let len: number | undefined
  if (cmd === 0xf7 || cmd === 0xf0 || cmd === 0xd0) len = 2 + n
  else if (cmd === 0xed) len = 8 + n
  else len = FIXED_CRO_LENGTH[cmd]
  if (len == null || len <= 0 || len > frame.length) return frame.length
  return len
}

function clampConnect(packet: Buffer, lastCommand: number): Buffer {
  if (lastCommand !== 0xff || packet[0] !== 0xff || packet.length < 8) return packet
  const out = Buffer.from(packet)
  if (out[3] > CAN_FD_MAX) out[3] = CAN_FD_MAX
  if (out.readUInt16LE(4) > CAN_FD_MAX) out.writeUInt16LE(CAN_FD_MAX, 4)
  return out
}

export interface XcpliteCanBridgeOptions {
  /** JSON-RPC listener started by an open project simulate device. */
  host?: string
  port: number
  /** Free SIMULATE_CAN handle. Must not be the handle the project already owns. */
  handle: number
  slavePort: number
  canIdCmd: number
  canIdResp: number
  slaveHost?: string
  /** Match the XCP master's CAN-FD BRS flag. The read socket includes BRS in its id. */
  brs?: boolean
}

interface RpcOk<T> {
  result?: string
  controllers?: Array<{ controllerId: number }>
  hardwareObjects?: Array<{ hohId: number; controllerId: number; objectType: string }>
  indications?: T[]
  mode?: string
}

interface RxIndication {
  id?: number
  data?: number[]
  controllerId?: number
}

interface PendingRpc {
  resolve: (value: RpcOk<RxIndication>) => void
  reject: (err: Error) => void
  timer: NodeJS.Timeout
}

function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

export class XcpliteCanBridge {
  readonly host: string
  readonly port: number
  readonly handle: number
  /** Transmit hardware-object id assigned by Can.Init. Handle * 2 for the default HTH. */
  hth = -1

  private readonly slavePort: number
  private readonly slaveHost: string
  private readonly canIdCmd: number
  private readonly canIdResp: number
  private readonly brs: boolean
  private socket: net.Socket | null = null
  private udp: dgram.Socket | null = null
  private txCounter = 0
  private lastCommand = 0
  private nextId = 1
  private buf = ''
  private closed = false
  private chain: Promise<void> = Promise.resolve()
  private pending = new Map<number, PendingRpc>()
  private reader: Promise<void> | null = null

  constructor(options: XcpliteCanBridgeOptions) {
    this.host = options.host ?? '127.0.0.1'
    this.port = options.port
    this.handle = options.handle
    this.slavePort = options.slavePort
    this.slaveHost = options.slaveHost ?? '127.0.0.1'
    this.canIdCmd = options.canIdCmd
    this.canIdResp = options.canIdResp
    this.brs = !!options.brs
  }

  async start(): Promise<void> {
    await this.connectTcp()
    this.udp = dgram.createSocket('udp4')
    this.udp.on('message', (msg) => this.onUdp(msg))
    this.udp.on('error', (err) => {
      if (!this.closed) console.error('XCPlite bridge UDP error', err)
    })
    await new Promise<void>((resolve) => this.udp?.bind(0, '127.0.0.1', () => resolve()))
    try {
      await this.initCan()
    } catch (err) {
      this.socket?.destroy()
      this.udp.close()
      this.udp = null
      throw err
    }
    this.reader = this.readCommands()
  }

  private connectTcp(): Promise<void> {
    return new Promise((resolve, reject) => {
      const socket = net.connect({ host: this.host, port: this.port })
      const fail = (err: Error) => {
        socket.destroy()
        reject(
          new Error(
            `SIMULATE_CAN JSON-RPC is not listening on ${this.host}:${this.port} (${err.message}). ` +
              'Open a project simulate device so setProjectSimulateCount starts the listener.'
          )
        )
      }
      socket.once('error', fail)
      socket.once('connect', () => {
        socket.removeListener('error', fail)
        this.socket = socket
        socket.setNoDelay(true)
        socket.on('data', (chunk) => this.onData(chunk))
        socket.on('error', (err) => {
          if (!this.closed) console.error('XCPlite bridge RPC error', err)
          this.failPending(err)
        })
        socket.on('close', () => {
          this.failPending(new Error('SIMULATE_CAN JSON-RPC connection closed'))
        })
        resolve()
      })
    })
  }

  private async initCan(): Promise<void> {
    const init = await this.call('Can.Init', {
      controllers: [
        {
          vendor: 'simulate',
          handle: this.handle,
          name: 'xcplite-bridge',
          canfd: true
        }
      ]
    })
    if (init.result && init.result !== 'E_OK') {
      throw new Error(`Can.Init returned ${init.result}`)
    }
    const opened = (init.controllers ?? []).some((c) => c.controllerId === this.handle)
    if (!opened) {
      throw new Error(
        `Can.Init did not open simulate handle ${this.handle}. The listener may already be initialized.`
      )
    }
    const tx = (init.hardwareObjects ?? []).find(
      (h) => h.controllerId === this.handle && h.objectType === 'TRANSMIT'
    )
    if (!tx) throw new Error(`Can.Init did not assign a transmit HOH for handle ${this.handle}`)
    this.hth = tx.hohId
    const mode = await this.call('Can.SetControllerMode', {
      controller: this.handle,
      transition: 'CAN_T_START'
    })
    if (mode.result !== 'E_OK' || mode.mode !== 'CAN_CS_STARTED') {
      throw new Error(`Can.SetControllerMode did not start handle ${this.handle}`)
    }
  }

  private call(method: string, params: Record<string, unknown>): Promise<RpcOk<RxIndication>> {
    const run = this.chain.then(() => this.callNow(method, params))
    this.chain = run.then(
      () => undefined,
      () => undefined
    )
    return run
  }

  private callNow(method: string, params: Record<string, unknown>): Promise<RpcOk<RxIndication>> {
    const socket = this.socket
    if (!socket || socket.destroyed) {
      return Promise.reject(new Error('SIMULATE_CAN JSON-RPC socket is closed'))
    }
    const id = this.nextId++
    const payload = JSON.stringify({ jsonrpc: '2.0', method, params, id }) + '\n'
    return new Promise((resolve, reject) => {
      const timer = setTimeout(() => {
        this.pending.delete(id)
        reject(new Error(`${method} timed out`))
      }, 5000)
      this.pending.set(id, { resolve, reject, timer })
      socket.write(payload)
    })
  }

  private onData(chunk: Buffer): void {
    this.buf += chunk.toString('utf8')
    let nl = this.buf.indexOf('\n')
    while (nl >= 0) {
      const line = this.buf.slice(0, nl).trim()
      this.buf = this.buf.slice(nl + 1)
      nl = this.buf.indexOf('\n')
      if (!line) continue
      let msg: {
        id?: number
        result?: RpcOk<RxIndication>
        error?: { message?: string; code?: number }
      }
      try {
        msg = JSON.parse(line)
      } catch {
        continue
      }
      if (msg.id == null || !this.pending.has(msg.id)) continue
      const pending = this.pending.get(msg.id)!
      this.pending.delete(msg.id)
      clearTimeout(pending.timer)
      if (msg.error) {
        pending.reject(new Error(`${msg.error.message ?? 'RPC error'} (${msg.error.code ?? ''})`))
      } else {
        pending.resolve(msg.result ?? {})
      }
    }
  }

  private failPending(err: Error): void {
    for (const pending of this.pending.values()) {
      clearTimeout(pending.timer)
      pending.reject(err)
    }
    this.pending.clear()
  }

  private onUdp(msg: Buffer): void {
    if (this.closed || this.hth < 0) return
    for (const packet of decodeEthDatagram(msg)) {
      const out = clampConnect(packet, this.lastCommand)
      if (out.length > CAN_FD_MAX) {
        console.error(`XCPlite bridge dropped ${out.length}-byte packet (CAN-FD max ${CAN_FD_MAX})`)
        continue
      }
      void this.call('Can.Write', {
        hth: this.hth,
        id: this.canIdResp,
        sdu: [...out],
        canfd: true,
        brs: this.brs,
        idType: 'STANDARD'
      })
        .then((res) => {
          if (!this.closed && res.result && res.result !== 'E_OK') {
            console.error('XCPlite bridge Can.Write returned', res.result)
          }
        })
        .catch((err) => {
          if (!this.closed) console.error('XCPlite bridge Can.Write failed', err)
        })
    }
  }

  private async readCommands(): Promise<void> {
    while (!this.closed && this.udp) {
      let indications: RxIndication[] = []
      try {
        const read = await this.call('Can.MainFunction_Read', { max: 32 })
        indications = read.indications ?? []
      } catch (err) {
        if (this.closed) return
        console.error('XCPlite bridge Can.MainFunction_Read failed', err)
        await sleep(20)
        continue
      }
      const commands = indications.filter(
        (frame) => frame.id === this.canIdCmd && Array.isArray(frame.data) && frame.data.length > 0
      )
      if (commands.length === 0) {
        await sleep(2)
        continue
      }
      for (const frame of commands) {
        const raw = Buffer.from(frame.data!)
        const payload = raw.subarray(0, xcpCroLength(raw))
        this.lastCommand = payload[0]
        const datagram = encodeEthPacket(this.txCounter, payload)
        this.txCounter = (this.txCounter + 1) & 0xffff
        await new Promise<void>((resolve, reject) => {
          this.udp?.send(datagram, this.slavePort, this.slaveHost, (err) => {
            if (err) reject(err)
            else resolve()
          })
        })
      }
    }
  }

  async stop(): Promise<void> {
    if (this.closed && !this.socket && !this.udp) return
    this.closed = true
    try {
      if (this.socket && !this.socket.destroyed) {
        await this.call('Can.DeInit', {})
      }
    } catch {
      // The listener may already be gone.
    }
    this.udp?.close()
    this.udp = null
    this.socket?.destroy()
    this.socket = null
    this.failPending(new Error('bridge stopped'))
    await this.reader?.catch(() => undefined)
    this.reader = null
  }
}
