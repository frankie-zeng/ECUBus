/**
 * Bridge an XCPlite XCP-on-UDP slave onto one node of the in-process simulate
 * CAN bus.
 *
 * XCPlite speaks only Ethernet. This process is the UDP master from the slave's
 * point of view and a CAN node from EcuBus-Pro's point of view: each XCP packet
 * is one CAN-FD frame on the command / response ids. The 4-byte Ethernet header
 * stays on the UDP side. CONNECT's MAX_CTO / MAX_DTO are clamped to 64 so the
 * XCP-on-CAN master does not build a packet that cannot fit in a CAN-FD frame.
 */
import dgram from 'dgram'
import { CAN_SOCKET, CanBase } from 'src/main/docan/base'
import { CAN_ERROR_ID, CAN_ID_TYPE, CanError, CanMsgType } from 'src/main/share/can'
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

const msgType: CanMsgType = {
  idType: CAN_ID_TYPE.STANDARD,
  brs: false,
  canfd: true,
  remote: false
}

export class XcpliteCanBridge {
  private udp: dgram.Socket | null = null
  private cmdSocket: CAN_SOCKET | null = null
  private respSocket: CAN_SOCKET | null = null
  private txCounter = 0
  private lastCommand = 0
  private closed = false
  private reader: Promise<void> | null = null

  constructor(
    private readonly base: CanBase,
    private readonly slavePort: number,
    private readonly canIdCmd: number,
    private readonly canIdResp: number,
    private readonly slaveHost = '127.0.0.1'
  ) {}

  async start(): Promise<void> {
    this.udp = dgram.createSocket('udp4')
    this.udp.on('message', (msg) => this.onUdp(msg))
    this.udp.on('error', (err) => {
      if (!this.closed) console.error('XCPlite bridge UDP error', err)
    })
    await new Promise<void>((resolve) => this.udp?.bind(0, '127.0.0.1', () => resolve()))
    this.cmdSocket = new CAN_SOCKET(this.base, this.canIdCmd, msgType)
    this.respSocket = new CAN_SOCKET(this.base, this.canIdResp, msgType)
    this.reader = this.readCommands()
  }

  private onUdp(msg: Buffer): void {
    if (this.closed || !this.respSocket) return
    for (const packet of decodeEthDatagram(msg)) {
      const out = clampConnect(packet, this.lastCommand)
      if (out.length > CAN_FD_MAX) {
        console.error(`XCPlite bridge dropped ${out.length}-byte packet (CAN-FD max ${CAN_FD_MAX})`)
        continue
      }
      void this.respSocket.write(out).catch((err) => {
        if (!this.closed) console.error('XCPlite bridge CAN write failed', err)
      })
    }
  }

  private async readCommands(): Promise<void> {
    while (!this.closed && this.cmdSocket && this.udp) {
      let frame: Buffer
      try {
        const read = await this.cmdSocket.read(500)
        frame = read.data
      } catch (err) {
        if (err instanceof CanError && err.errorId === CAN_ERROR_ID.CAN_READ_TIMEOUT) continue
        if (this.closed) return
        throw err
      }
      const payload = frame.subarray(0, xcpCroLength(frame))
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

  async stop(): Promise<void> {
    this.closed = true
    this.cmdSocket?.close()
    this.respSocket?.close()
    this.udp?.close()
    this.cmdSocket = null
    this.respSocket = null
    this.udp = null
    await this.reader?.catch(() => undefined)
  }
}
