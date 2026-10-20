import { describe, expect, it } from 'vitest'
import {
  defaultXcpConnection,
  mergeDaqEvents,
  normalizeXcpProject,
  resolveXcpConnection,
  validateXcpConnection,
  type XcpConnectionConfig
} from 'src/main/share/xcpConfig'
import { XcpMaster, XcpTransport } from 'src/main/xcp/xcpMaster'

function saved(partial: Partial<XcpConnectionConfig> = {}): XcpConnectionConfig {
  return defaultXcpConnection({ id: '1', name: 'ecu', device: 'CAN1', ...partial })
}

describe('XCP project config', () => {
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

  it('keeps the latest slave event name for a channel', () => {
    const merged = mergeDaqEvents(
      [{ channel: 0, name: 'tick', source: 'slave' }],
      [
        { channel: 0, name: 'tick-live', source: 'slave', priority: 1 },
        { channel: 1, name: 'loop', source: 'slave' }
      ]
    )
    expect(merged.map((item) => item.name)).toEqual(['tick-live', 'loop'])
    expect(merged[0].priority).toBe(1)
  })

  it('drops A2L and seed & key fields when an older project file is loaded', () => {
    const loaded = normalizeXcpProject({
      old: {
        id: 'old',
        name: 'ecu',
        device: 'CAN1',
        a2lFile: '/tmp/ecu.a2l',
        a2lObjects: [{ name: 'counter', address: 1 }],
        seedKeyDll: 'key.dll',
        daqEvents: [
          { channel: 0, name: 'from-file', source: 'a2l' },
          { channel: 1, name: 'tick', source: 'slave' }
        ],
        daqEvent: 1
      }
    })
    const cfg = loaded.old
    expect(cfg).not.toHaveProperty('a2lFile')
    expect(cfg).not.toHaveProperty('a2lObjects')
    expect(cfg).not.toHaveProperty('seedKeyDll')
    expect(cfg.daqEvents).toEqual([{ channel: 1, name: 'tick', source: 'slave' }])
    expect(cfg.daqEvent).toBe(1)
    expect(validateXcpConnection(cfg)).toEqual([])
    const roundTrip = JSON.parse(JSON.stringify(cfg)) as XcpConnectionConfig
    expect(Object.keys(roundTrip).sort()).toEqual(Object.keys(saved()).sort())
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
})
