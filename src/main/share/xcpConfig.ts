/**
 * XCP connection stored in the project file and opened by name from a script.
 */

export type XcpTransportKind = 'can' | 'udp' | 'tcp'

export interface XcpDaqEvent {
  channel: number
  name: string
  timeCycle?: number
  timeUnit?: number
  priority?: number
  /** Filled from GET_DAQ_EVENT_INFO. */
  source: 'slave'
}

export interface XcpConnectionConfig {
  id: string
  name: string
  transport: XcpTransportKind
  /** CAN device name. Required when {@link transport} is `can`. */
  device: string
  /** Command response timeout, milliseconds. */
  timeout: number
  canIdCmd: number
  canIdResp: number
  /** 29-bit identifiers when true, 11-bit otherwise. */
  extended: boolean
  canfd: boolean
  brs: boolean
  padding: boolean
  paddingValue: number
  /** Padded frame length in bytes. */
  dlc: number
  ip: string
  port: number
  /** Event channels read from the slave with GET_DAQ_EVENT_INFO. */
  daqEvents: XcpDaqEvent[]
  /** Selected DAQ event channel. */
  daqEvent?: number
}

/** Inline address a script can pass instead of a saved connection name. */
export interface XcpCanInline {
  transport?: 'can'
  name?: string
  canIdCmd: number
  canIdResp: number
  extended?: boolean
  canfd?: boolean
  brs?: boolean
  padding?: boolean
  paddingValue?: number
  dlc?: number
  timeout?: number
}

export interface XcpEthInline {
  transport: 'udp' | 'tcp'
  name?: string
  ip: string
  port: number
  timeout?: number
}

export type XcpInlineAddr = XcpCanInline | XcpEthInline

/** Keep slave events. Drop A2L-sourced events from older project files. */
export function slaveDaqEvents(events: unknown): XcpDaqEvent[] {
  if (!Array.isArray(events)) return []
  const out: XcpDaqEvent[] = []
  for (const event of events) {
    if (!event || typeof event !== 'object') continue
    const item = event as {
      channel?: unknown
      name?: unknown
      source?: unknown
      timeCycle?: unknown
      timeUnit?: unknown
      priority?: unknown
    }
    if (item.source === 'a2l') continue
    if (typeof item.channel !== 'number' || !Number.isInteger(item.channel) || item.channel < 0) {
      continue
    }
    out.push({
      channel: item.channel,
      name: typeof item.name === 'string' && item.name ? item.name : `event ${item.channel}`,
      timeCycle: typeof item.timeCycle === 'number' ? item.timeCycle : undefined,
      timeUnit: typeof item.timeUnit === 'number' ? item.timeUnit : undefined,
      priority: typeof item.priority === 'number' ? item.priority : undefined,
      source: 'slave'
    })
  }
  return out
}

export function defaultXcpConnection(
  partial: Partial<XcpConnectionConfig> & { id: string; name: string }
): XcpConnectionConfig {
  return {
    id: partial.id,
    name: partial.name,
    transport: partial.transport ?? 'can',
    device: partial.device ?? '',
    timeout: partial.timeout ?? 1000,
    canIdCmd: partial.canIdCmd ?? 0x7e0,
    canIdResp: partial.canIdResp ?? 0x7e1,
    extended: partial.extended ?? false,
    canfd: partial.canfd ?? false,
    brs: partial.brs ?? false,
    padding: partial.padding ?? false,
    paddingValue: partial.paddingValue ?? 0,
    dlc: partial.dlc ?? 8,
    ip: partial.ip ?? '127.0.0.1',
    port: partial.port ?? 5555,
    daqEvents: slaveDaqEvents(partial.daqEvents),
    daqEvent:
      partial.daqEvent != null && Number.isInteger(partial.daqEvent) && partial.daqEvent >= 0
        ? partial.daqEvent
        : undefined
  }
}

/**
 * Rebuild saved connections so older project files that still carry A2L or
 * seed & key fields load. Those fields are not kept.
 */
export function normalizeXcpProject(xcp: unknown): Record<string, XcpConnectionConfig> {
  if (!xcp || typeof xcp !== 'object') return {}
  const out: Record<string, XcpConnectionConfig> = {}
  for (const [key, raw] of Object.entries(xcp as Record<string, unknown>)) {
    if (!raw || typeof raw !== 'object') continue
    const item = raw as Partial<XcpConnectionConfig> & { id?: string; name?: string }
    const id = typeof item.id === 'string' && item.id ? item.id : key
    const name = typeof item.name === 'string' && item.name ? item.name : id
    out[key] = defaultXcpConnection({ ...item, id, name })
  }
  return out
}

function isIpv4OrHost(value: string): boolean {
  if (!value || value.length > 253) return false
  return /^[A-Za-z0-9._:-]+$/.test(value)
}

/** Return human-readable problems. An empty list means the config can be opened. */
export function validateXcpConnection(raw: XcpConnectionConfig): string[] {
  const errors: string[] = []
  const cfg = raw
  if (!cfg.name || !cfg.name.trim()) errors.push('name is required')
  if (!Number.isInteger(cfg.timeout) || cfg.timeout < 1 || cfg.timeout > 600000) {
    errors.push('timeout must be an integer from 1 to 600000 ms')
  }
  if (cfg.transport !== 'can' && cfg.transport !== 'udp' && cfg.transport !== 'tcp') {
    errors.push('transport must be can, udp, or tcp')
  }
  if (cfg.transport === 'can') {
    if (!cfg.device || !cfg.device.trim()) errors.push('CAN device is required')
    const maxId = cfg.extended ? 0x1fffffff : 0x7ff
    for (const [label, id] of [
      ['canIdCmd', cfg.canIdCmd],
      ['canIdResp', cfg.canIdResp]
    ] as const) {
      if (!Number.isInteger(id) || id < 0 || id > maxId) {
        errors.push(`${label} must be an integer from 0 to 0x${maxId.toString(16)}`)
      }
    }
    if (cfg.canIdCmd === cfg.canIdResp) errors.push('canIdCmd and canIdResp must differ')
    if (!Number.isInteger(cfg.paddingValue) || cfg.paddingValue < 0 || cfg.paddingValue > 255) {
      errors.push('paddingValue must be a byte')
    }
    if (!Number.isInteger(cfg.dlc) || cfg.dlc < 1 || cfg.dlc > 64) {
      errors.push('dlc must be a frame length from 1 to 64')
    }
    if (cfg.brs && !cfg.canfd) errors.push('brs requires canfd')
  } else {
    if (!isIpv4OrHost(cfg.ip)) errors.push('IP address is required')
    if (!Number.isInteger(cfg.port) || cfg.port < 1 || cfg.port > 65535) {
      errors.push('port must be an integer from 1 to 65535')
    }
  }
  if (cfg.daqEvent != null && (!Number.isInteger(cfg.daqEvent) || cfg.daqEvent < 0)) {
    errors.push('DAQ event channel must be a non-negative integer')
  }
  return errors
}

function isEth(input: XcpInlineAddr): input is XcpEthInline {
  return input.transport === 'udp' || input.transport === 'tcp'
}

/**
 * Turn a script argument into a connection config.
 * A string is a connection name saved by the GUI. An object is an inline address.
 */
export function resolveXcpConnection(
  input: string | XcpInlineAddr,
  configs: Record<string, XcpConnectionConfig> | undefined,
  device?: string
): XcpConnectionConfig {
  if (typeof input === 'string') {
    const found = Object.values(configs ?? {}).find((item) => item.name === input)
    if (!found) throw new Error(`XCP connection '${input}' not found`)
    const cfg = defaultXcpConnection(found)
    if (device && cfg.transport === 'can') cfg.device = device
    const errors = validateXcpConnection(cfg)
    if (errors.length) throw new Error(errors.join('; '))
    return cfg
  }
  if (isEth(input)) {
    return defaultXcpConnection({
      id: 'inline',
      name: input.name ?? 'inline',
      transport: input.transport,
      ip: input.ip,
      port: input.port,
      timeout: input.timeout ?? 1000
    })
  }
  return defaultXcpConnection({
    id: 'inline',
    name: input.name ?? 'inline',
    transport: 'can',
    device: device ?? '',
    timeout: input.timeout ?? 1000,
    canIdCmd: input.canIdCmd,
    canIdResp: input.canIdResp,
    extended: input.extended,
    canfd: input.canfd,
    brs: input.brs,
    padding: input.padding,
    paddingValue: input.paddingValue,
    dlc: input.dlc
  })
}

export function mergeDaqEvents(base: XcpDaqEvent[], extra: XcpDaqEvent[]): XcpDaqEvent[] {
  const map = new Map<number, XcpDaqEvent>()
  for (const event of base) map.set(event.channel, event)
  for (const event of extra) {
    const prev = map.get(event.channel)
    map.set(
      event.channel,
      prev ? { ...prev, ...event, name: event.name || prev.name, source: event.source } : event
    )
  }
  return [...map.values()].sort((a, b) => a.channel - b.channel)
}
