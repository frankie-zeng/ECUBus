import { describe, expect, it } from 'vitest'
import { parseA2lEvents, parseA2lObjects } from 'src/main/share/a2l'
import {
  defaultXcpConnection,
  mergeDaqEvents,
  resolveXcpConnection,
  validateXcpConnection,
  type XcpConnectionConfig
} from 'src/main/share/xcpConfig'
import { XcpMaster, XcpTransport } from 'src/main/xcp/xcpMaster'
import { unlockWithSeedKey } from 'src/main/xcp/xcpSeedKey'

const A2L = `
/begin MEASUREMENT counter ""
  UBYTE 0 0 0 255
  ECU_ADDRESS 0x1000
  ECU_ADDRESS_EXTENSION 1
  /begin IF_DATA XCP
    /begin DAQ_EVENT VARIABLE
      /begin AVAILABLE_EVENT_LIST
        EVENT 1
      /end AVAILABLE_EVENT_LIST
    /end DAQ_EVENT
  /end IF_DATA
/end MEASUREMENT
/begin CHARACTERISTIC gain ""
  VALUE 0x80010000 UWORD 0 100
/end CHARACTERISTIC
/begin EVENT "mainloop" "mainloop" 0x1 DAQ 0xFF 0 0 0 CONSISTENCY EVENT /end EVENT
`

function saved(partial: Partial<XcpConnectionConfig> = {}): XcpConnectionConfig {
  return defaultXcpConnection({ id: '1', name: 'ecu', device: 'CAN1', ...partial })
}

describe('XCP project config', () => {
  it('parses measurements, characteristics and events from an A2L', () => {
    const objects = parseA2lObjects(A2L)
    expect(objects.find((item) => item.name === 'counter')).toMatchObject({
      kind: 'measurement',
      address: 0x1000,
      extension: 1,
      size: 1,
      event: 1
    })
    expect(objects.find((item) => item.name === 'gain')).toMatchObject({
      kind: 'characteristic',
      address: 0x80010000,
      size: 2
    })
    expect(parseA2lEvents(A2L)).toEqual([{ name: 'mainloop', channel: 1 }])
  })

  it('validates CAN and Ethernet fields and survives a project-file round trip', () => {
    expect(validateXcpConnection(saved())).toEqual([])
    const udp = saved({ transport: 'udp', ip: '127.0.0.1', port: 5555, timeout: 1500 })
    expect(validateXcpConnection(udp)).toEqual([])
    const loaded = JSON.parse(JSON.stringify(udp)) as XcpConnectionConfig
    expect(validateXcpConnection(loaded)).toEqual([])
    expect(loaded.timeout).toBe(1500)

    expect(validateXcpConnection(saved({ name: '  ' })).join(' ')).toContain('name')
    expect(validateXcpConnection(saved({ device: '' })).join(' ')).toContain('device')
    expect(validateXcpConnection(saved({ canIdCmd: 0x7e1 })).join(' ')).toContain('differ')
    expect(validateXcpConnection(saved({ timeout: 0 })).join(' ')).toContain('timeout')
    expect(validateXcpConnection(saved({ brs: true, canfd: false })).join(' ')).toContain('brs')
    expect(validateXcpConnection(saved({ transport: 'tcp', ip: '', port: 0 })).join(' ')).toContain(
      'IP'
    )
  })

  it('opens a GUI connection by name and keeps an inline address', () => {
    const cfg = saved({ timeout: 2500, canfd: true })
    const byName = resolveXcpConnection('ecu', { [cfg.id]: cfg }, 'Other')
    expect(byName.timeout).toBe(2500)
    expect(byName.device).toBe('Other')
    expect(byName.canfd).toBe(true)

    const inline = resolveXcpConnection(
      { transport: 'udp', ip: '10.0.0.2', port: 18001, timeout: 800 },
      {}
    )
    expect(inline.transport).toBe('udp')
    expect(inline.port).toBe(18001)
    expect(validateXcpConnection(inline)).toEqual([])

    expect(() => resolveXcpConnection('missing', {})).toThrow(/not found/)
  })

  it('merges slave event names over A2L channels', () => {
    const merged = mergeDaqEvents(
      [{ channel: 0, name: 'tick', source: 'a2l' }],
      [
        { channel: 0, name: 'tick-live', source: 'slave', priority: 1 },
        { channel: 1, name: 'loop', source: 'slave' }
      ]
    )
    expect(merged.map((item) => item.name)).toEqual(['tick-live', 'loop'])
    expect(merged[0].priority).toBe(1)
  })

  it('uses the configured timeout on every request', async () => {
    let seen = 0
    const transport: XcpTransport = {
      async request(_packet, timeout) {
        seen = timeout ?? 0
        return Buffer.from([0xff, 0x01, 0x80, 8, 8, 0, 0x14, 0x14])
      },
      async send() {
        return undefined
      },
      close() {
        return undefined
      }
    }
    const master = new XcpMaster(transport, 2500)
    await master.connect(0)
    expect(seen).toBe(2500)
  })

  it('computes an UNLOCK key with the seed & key provider', async () => {
    const master = {
      async getSeed(mode: number) {
        if (mode === 0) return { length: 4, seed: [1, 2] }
        return { length: 2, seed: [3, 4] }
      },
      async unlock(_length: number, key: number[] | Buffer) {
        expect(Buffer.from(key as number[]).equals(Buffer.from([0xaa, 0xbb]))).toBe(true)
        return { calpag: true, daq: false, stim: false, pgm: false }
      }
    }
    const flags = await unlockWithSeedKey(master as unknown as XcpMaster, 1, {
      generateKey(seed) {
        expect(Array.from(seed)).toEqual([1, 2, 3, 4])
        return Buffer.from([0xaa, 0xbb])
      }
    })
    expect(flags.calpag).toBe(true)
  })
})
