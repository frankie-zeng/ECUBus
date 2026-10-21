/**
 * Names for XCP packets shown in the Trace window.
 * A command is labelled `can-sim.CONNECT`. A slave packet is `can-sim.RES`,
 * `can-sim.ERR_CMD_UNKNOWN`, `can-sim.EV_DAQ_OVERLOAD`, or `can-sim.DAQ0.ODT1`.
 */
import {
  ByteOrder,
  GET_VERSION_SUB,
  XcpCommand,
  XcpErrorCode,
  XcpResponse,
  xcpErrorMessage
} from './xcpProtocol'

export type XcpTraceKind = 'cmd' | 'res' | 'err' | 'ev' | 'serv' | 'daq' | 'stim'

/** Trace bus method. Colors and the XCP filter listen for these. */
export type XcpTraceMethod = 'xcpSent' | 'xcpRecv' | 'xcpErr' | 'xcpEvent' | 'xcpDaq'

/**
 * How a DAQ DTO identifies its list, from GET_DAQ_PROCESSOR_INFO DAQ_KEY_BYTE
 * bits 6..7.
 */
export type XcpDaqIdField =
  | 'absolute-odt'
  | 'relative-byte'
  | 'relative-word'
  | 'relative-word-aligned'

export interface XcpTraceRecord {
  method: XcpTraceMethod
  kind: XcpTraceKind
  /** `can-sim.CONNECT` */
  name: string
  /** CONNECT, RES, ERR_CMD_UNKNOWN, DAQ0.ODT1, ... */
  symbol: string
  dir: 'out' | 'in'
  /** Extra text for the row detail, such as the error sentence or DAQ list. */
  detail?: string
  daqList?: number
  odt?: number
}

/** Called by a transport for every XCP packet that goes on the wire. */
export interface XcpTraceHook {
  tx(packet: Buffer, ts?: number): void
  rx(packet: Buffer, ts?: number): void
  close(): void
}

const eventNames: Record<number, string> = {
  0x00: 'EV_RESUME_MODE',
  0x01: 'EV_CLEAR_DAQ',
  0x02: 'EV_STORE_DAQ',
  0x03: 'EV_STORE_CAL',
  0x05: 'EV_CMD_PENDING',
  0x06: 'EV_DAQ_OVERLOAD',
  0x07: 'EV_SESSION_TERMINATED',
  0x08: 'EV_TIME_SYNC',
  0x09: 'EV_STIM_TIMEOUT',
  0x0a: 'EV_SLEEP',
  0x0b: 'EV_WAKE_UP',
  0xfe: 'EV_USER',
  0xff: 'EV_TRANSPORT'
}

const serviceNames: Record<number, string> = {
  0x00: 'SERV_RESET',
  0x01: 'SERV_TEXT'
}

function hexByte(value: number): string {
  return (value & 0xff).toString(16).toUpperCase().padStart(2, '0')
}

/** `can-sim.CONNECT`. An empty connection name becomes `XCP`. */
export function xcpTraceName(connection: string, symbol: string): string {
  const who = connection.trim() || 'XCP'
  return `${who}.${symbol}`
}

/** Command PID to CONNECT, SHORT_UPLOAD, GET_VERSION, or `CMD_AB`. */
export function xcpCommandName(pid: number, next?: number): string {
  if ((pid & 0xff) === XcpCommand.GET_VERSION && next === GET_VERSION_SUB) return 'GET_VERSION'
  const name = XcpCommand[pid & 0xff]
  return typeof name === 'string' ? name : `CMD_${hexByte(pid)}`
}

/** ERR second byte to ERR_CMD_UNKNOWN, or `ERR_3F` when the code is not standard. */
export function xcpErrorName(code: number): string {
  const name = XcpErrorCode[code & 0xff]
  return typeof name === 'string' ? name : `ERR_${hexByte(code)}`
}

/** EV second byte to EV_DAQ_OVERLOAD, or `EV_1C` when the code is not standard. */
export function xcpEventName(code: number): string {
  return eventNames[code & 0xff] ?? `EV_${hexByte(code)}`
}

/** SERV second byte to SERV_RESET / SERV_TEXT, or `SERV_1C`. */
export function xcpServiceName(code: number): string {
  return serviceNames[code & 0xff] ?? `SERV_${hexByte(code)}`
}

/** DAQ_KEY_BYTE bits 6..7. Any other value stays absolute ODT. */
export function daqIdFieldFromKeyByte(keyByte: number): XcpDaqIdField {
  switch ((keyByte >> 6) & 0x03) {
    case 1:
      return 'relative-byte'
    case 2:
      return 'relative-word'
    case 3:
      return 'relative-word-aligned'
    default:
      return 'absolute-odt'
  }
}

function readWord(packet: Buffer, offset: number, byteOrder: ByteOrder): number {
  const lo = packet[offset] ?? 0
  const hi = packet[offset + 1] ?? 0
  return byteOrder === ByteOrder.MOTOROLA ? (lo << 8) | hi : lo | (hi << 8)
}

function describeDaq(
  connection: string,
  packet: Buffer,
  field: XcpDaqIdField,
  byteOrder: ByteOrder,
  prefix: 'DAQ' | 'STIM'
): XcpTraceRecord {
  const odt = packet[0] ?? 0
  let daqList: number | undefined
  if (field === 'relative-byte') daqList = packet[1] ?? 0
  else if (field === 'relative-word') daqList = readWord(packet, 1, byteOrder)
  else if (field === 'relative-word-aligned') daqList = readWord(packet, 2, byteOrder)
  const symbol = daqList == undefined ? `${prefix}.ODT${odt}` : `${prefix}${daqList}.ODT${odt}`
  const detail = daqList == undefined ? `ODT ${odt}` : `DAQ list ${daqList}, ODT ${odt}`
  return {
    method: 'xcpDaq',
    kind: prefix === 'DAQ' ? 'daq' : 'stim',
    name: xcpTraceName(connection, symbol),
    symbol,
    dir: prefix === 'DAQ' ? 'in' : 'out',
    detail,
    daqList,
    odt
  }
}

/** Master to slave. PID >= 0xC0 is a command. A lower PID is a STIM DTO. */
export function describeXcpTx(
  packet: Buffer,
  connection: string,
  field: XcpDaqIdField = 'absolute-odt',
  byteOrder: ByteOrder = ByteOrder.INTEL
): XcpTraceRecord {
  const pid = packet[0] ?? 0
  if (pid >= 0xc0) {
    const symbol = xcpCommandName(pid, packet[1])
    return {
      method: 'xcpSent',
      kind: 'cmd',
      name: xcpTraceName(connection, symbol),
      symbol,
      dir: 'out'
    }
  }
  return describeDaq(connection, packet, field, byteOrder, 'STIM')
}

/** Slave to master: RES, ERR, EV, SERV, or a DAQ DTO. */
export function describeXcpRx(
  packet: Buffer,
  connection: string,
  field: XcpDaqIdField = 'absolute-odt',
  byteOrder: ByteOrder = ByteOrder.INTEL
): XcpTraceRecord {
  const pid = packet[0] ?? 0
  if (pid === XcpResponse.RES) {
    return {
      method: 'xcpRecv',
      kind: 'res',
      name: xcpTraceName(connection, 'RES'),
      symbol: 'RES',
      dir: 'in'
    }
  }
  if (pid === XcpResponse.ERR) {
    const code = packet[1] ?? 0
    const symbol = xcpErrorName(code)
    return {
      method: 'xcpErr',
      kind: 'err',
      name: xcpTraceName(connection, symbol),
      symbol,
      dir: 'in',
      detail: xcpErrorMessage(code)
    }
  }
  if (pid === XcpResponse.EV) {
    const symbol = xcpEventName(packet[1] ?? 0)
    return {
      method: 'xcpEvent',
      kind: 'ev',
      name: xcpTraceName(connection, symbol),
      symbol,
      dir: 'in'
    }
  }
  if (pid === XcpResponse.SERV) {
    const symbol = xcpServiceName(packet[1] ?? 0)
    return {
      method: 'xcpEvent',
      kind: 'serv',
      name: xcpTraceName(connection, symbol),
      symbol,
      dir: 'in'
    }
  }
  return describeDaq(connection, packet, field, byteOrder, 'DAQ')
}
