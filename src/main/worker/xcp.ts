/**
 * XCP (ASAM MCD-1 XCP) worker API — drive an XCP slave from a user script.
 *
 * @remarks
 * All functions bridge to the main process via `emitWorkerEventWithReply('xcpApi', ...)`.
 * Use {@link XcpCreateConnection} to open an XCP-on-CAN connection and obtain a handle,
 * then pass that handle to the command helpers. Always call {@link XcpCloseConnection}
 * when done.
 *
 * This first iteration targets XCP-on-CAN and the standard command set (session,
 * memory transfer, calibration/paging, basic DAQ and programming). The protocol
 * codec is validated against the test vectors of the mature
 * {@link https://github.com/christoph2/pyxcp | pyXCP} master.
 *
 * @module xcp
 * @category XCP
 *
 * @example
 * ```ts
 * import { XcpCreateConnection, XcpConnect, XcpShortUpload, XcpCloseConnection } from 'ecubus-worker'
 *
 * Util.Init(async () => {
 *   const handle = await XcpCreateConnection({
 *     name: 'ecu',
 *     canIdCmd: 0x7e0,
 *     canIdResp: 0x7e1,
 *     padding: true
 *   })
 *   const info = await XcpConnect(handle)
 *   console.log('MAX_CTO', info.maxCto, 'MAX_DTO', info.maxDto)
 *   const bytes = await XcpShortUpload(handle, 4, 0x1000)
 *   console.log('mem', bytes)
 *   await XcpCloseConnection(handle)
 * })
 * ```
 */

import { emitWorkerEventWithReply } from './uds'
import type { XcpCanAddr } from '../xcp/xcpCan'
import type { XcpEthInline } from '../share/xcpConfig'
import type {
  BuildChecksumResponse,
  ConnectResponse,
  GetCommModeInfoResponse,
  GetIdResponse,
  GetSeedResponse,
  GetStatusResponse,
  GetVersionResponse,
  ResourceFlags,
  StartStopDaqListResponse
} from '../xcp/xcpProtocol'

/**
 * XCP-on-CAN addressing configuration passed to {@link XcpCreateConnection}.
 * @category XCP
 */
export type { XcpCanAddr }

async function xcpCommand<T>(handle: string, method: string, args: any[] = []): Promise<T> {
  return (await emitWorkerEventWithReply('xcpApi', {
    op: 'command',
    handle,
    method,
    args
  })) as T
}

/**
 * Open an XCP connection and return an opaque handle string.
 *
 * Pass a connection name saved in the XCP window (`XcpCreateConnection('ecu')`),
 * an XCP-on-CAN address object, or `{ transport: 'udp' | 'tcp', ip, port }`.
 *
 * @param addr - Saved connection name, or an inline CAN / Ethernet address.
 * @param device - Optional CAN channel name. Overrides the device on a CAN connection.
 * @returns Opaque handle string to pass to the other Xcp* functions.
 * @throws If the name is unknown, the config is invalid, or no CAN device is open.
 * @category XCP
 *
 * @example
 * ```ts
 * const handle = await XcpCreateConnection('ecu')
 * ```
 */
export async function XcpCreateConnection(
  addr: XcpCanAddr | XcpEthInline | string,
  device?: string
): Promise<string> {
  const handle = await emitWorkerEventWithReply('xcpApi', {
    op: 'createConnection',
    addr: typeof addr === 'string' ? undefined : addr,
    name: typeof addr === 'string' ? addr : undefined,
    device
  })
  return handle as string
}

/**
 * Close an XCP connection previously opened with {@link XcpCreateConnection}.
 * @category XCP
 */
export async function XcpCloseConnection(handle: string): Promise<void> {
  await emitWorkerEventWithReply('xcpApi', { op: 'closeConnection', handle })
}

/** CONNECT to the slave and read back its properties. @category XCP */
export function XcpConnect(handle: string, mode = 0): Promise<ConnectResponse> {
  return xcpCommand(handle, 'connect', [mode])
}

/** DISCONNECT from the slave. @category XCP */
export function XcpDisconnect(handle: string): Promise<number[]> {
  return xcpCommand(handle, 'disconnect')
}

/** GET_STATUS: current session and resource-protection status. @category XCP */
export function XcpGetStatus(handle: string): Promise<GetStatusResponse> {
  return xcpCommand(handle, 'getStatus')
}

/** GET_COMM_MODE_INFO: optional communication modes and timing. @category XCP */
export function XcpGetCommModeInfo(handle: string): Promise<GetCommModeInfoResponse> {
  return xcpCommand(handle, 'getCommModeInfo')
}

/** GET_VERSION: protocol and transport layer versions. @category XCP */
export function XcpGetVersion(handle: string): Promise<GetVersionResponse> {
  return xcpCommand(handle, 'getVersion')
}

/** GET_ID: request the identifier header (identifier bytes follow via {@link XcpUpload}). @category XCP */
export function XcpGetId(handle: string, mode: number): Promise<GetIdResponse> {
  return xcpCommand(handle, 'getId', [mode])
}

/** GET_SEED: request a seed for seed & key unlock. @category XCP */
export function XcpGetSeed(
  handle: string,
  mode: number,
  resource: number
): Promise<GetSeedResponse> {
  return xcpCommand(handle, 'getSeed', [mode, resource])
}

/** UNLOCK: send the computed key to unlock a protected resource. @category XCP */
export function XcpUnlock(handle: string, length: number, key: number[]): Promise<ResourceFlags> {
  return xcpCommand(handle, 'unlock', [length, key])
}

/** SET_MTA: set the Memory Transfer Address pointer. @category XCP */
export function XcpSetMta(
  handle: string,
  address: number,
  addressExtension = 0
): Promise<number[]> {
  return xcpCommand(handle, 'setMta', [address, addressExtension])
}

/** UPLOAD: read `count` elements from the current MTA. @category XCP */
export function XcpUpload(handle: string, count: number): Promise<number[]> {
  return xcpCommand(handle, 'upload', [count])
}

/** SHORT_UPLOAD: read `count` elements from an explicit address. @category XCP */
export function XcpShortUpload(
  handle: string,
  count: number,
  address: number,
  addressExtension = 0
): Promise<number[]> {
  return xcpCommand(handle, 'shortUpload', [count, address, addressExtension])
}

/** BUILD_CHECKSUM: compute a checksum over a memory block from the current MTA. @category XCP */
export function XcpBuildChecksum(
  handle: string,
  blockSize: number
): Promise<BuildChecksumResponse> {
  return xcpCommand(handle, 'buildChecksum', [blockSize])
}

/** DOWNLOAD: write `data` to the current MTA. @category XCP */
export function XcpDownload(handle: string, data: number[]): Promise<number[]> {
  return xcpCommand(handle, 'download', [data])
}

/** SHORT_DOWNLOAD: write `data` to an explicit address. @category XCP */
export function XcpShortDownload(
  handle: string,
  address: number,
  addressExtension: number,
  data: number[]
): Promise<number[]> {
  return xcpCommand(handle, 'shortDownload', [address, addressExtension, data])
}

/** MODIFY_BITS: atomic read-modify-write of a 16-bit word at the current MTA. @category XCP */
export function XcpModifyBits(
  handle: string,
  shiftValue: number,
  andMask: number,
  xorMask: number
): Promise<number[]> {
  return xcpCommand(handle, 'modifyBits', [shiftValue, andMask, xorMask])
}

/** SET_CAL_PAGE: activate a calibration page. @category XCP */
export function XcpSetCalPage(
  handle: string,
  mode: number,
  segment: number,
  page: number
): Promise<number[]> {
  return xcpCommand(handle, 'setCalPage', [mode, segment, page])
}

/** GET_CAL_PAGE: get the active calibration page of a segment. @category XCP */
export function XcpGetCalPage(handle: string, mode: number, segment: number): Promise<number> {
  return xcpCommand(handle, 'getCalPage', [mode, segment])
}

/** SET_DAQ_PTR: select the DAQ list / ODT / ODT-entry for subsequent WRITE_DAQ. @category XCP */
export function XcpSetDaqPtr(
  handle: string,
  daqListNumber: number,
  odtNumber: number,
  odtEntryNumber: number
): Promise<number[]> {
  return xcpCommand(handle, 'setDaqPtr', [daqListNumber, odtNumber, odtEntryNumber])
}

/** WRITE_DAQ: configure the currently selected ODT entry. @category XCP */
export function XcpWriteDaq(
  handle: string,
  bitOffset: number,
  size: number,
  addressExtension: number,
  address: number
): Promise<number[]> {
  return xcpCommand(handle, 'writeDaq', [bitOffset, size, addressExtension, address])
}

/** SET_DAQ_LIST_MODE: set the mode of a DAQ list. @category XCP */
export function XcpSetDaqListMode(
  handle: string,
  mode: number,
  daqListNumber: number,
  eventChannelNumber: number,
  prescaler: number,
  priority: number
): Promise<number[]> {
  return xcpCommand(handle, 'setDaqListMode', [
    mode,
    daqListNumber,
    eventChannelNumber,
    prescaler,
    priority
  ])
}

/** START_STOP_DAQ_LIST: start/stop/select a single DAQ list. @category XCP */
export function XcpStartStopDaqList(
  handle: string,
  mode: number,
  daqListNumber: number
): Promise<StartStopDaqListResponse> {
  return xcpCommand(handle, 'startStopDaqList', [mode, daqListNumber])
}

/** START_STOP_SYNCH: start/stop all selected DAQ lists synchronously. @category XCP */
export function XcpStartStopSynch(handle: string, mode: number): Promise<number[]> {
  return xcpCommand(handle, 'startStopSynch', [mode])
}

/** GET_DAQ_CLOCK: get the current slave DAQ clock timestamp. @category XCP */
export function XcpGetDaqClock(handle: string): Promise<number> {
  return xcpCommand(handle, 'getDaqClock')
}

/** GET_DAQ_PROCESSOR_INFO: DAQ capabilities of the slave. @category XCP */
export function XcpGetDaqProcessorInfo(handle: string): Promise<{
  properties: number
  maxDaq: number
  maxEvent: number
  minDaq: number
  keyByte: number
}> {
  return xcpCommand(handle, 'getDaqProcessorInfo')
}

/** GET_DAQ_RESOLUTION_INFO: ODT granularity and timestamp format. @category XCP */
export function XcpGetDaqResolutionInfo(handle: string): Promise<{
  granularityDaq: number
  maxSizeDaq: number
  granularityStim: number
  maxSizeStim: number
  timestampMode: number
  timestampTicks: number
}> {
  return xcpCommand(handle, 'getDaqResolutionInfo')
}

/** GET_DAQ_EVENT_INFO: properties of one event channel. @category XCP */
export function XcpGetDaqEventInfo(
  handle: string,
  eventChannelNumber: number
): Promise<{
  properties: number
  maxDaqList: number
  nameLength: number
  timeCycle: number
  timeUnit: number
  priority: number
}> {
  return xcpCommand(handle, 'getDaqEventInfo', [eventChannelNumber])
}

/** GET_PAG_PROCESSOR_INFO: calibration segment count and paging properties. @category XCP */
export function XcpGetPagProcessorInfo(
  handle: string
): Promise<{ maxSegments: number; properties: number }> {
  return xcpCommand(handle, 'getPagProcessorInfo')
}

/**
 * GET_ID plus the identification bytes (inline or via UPLOAD).
 * @category XCP
 */
export function XcpReadId(
  handle: string,
  type: number
): Promise<{ mode: number; length: number; data: number[] }> {
  return xcpCommand(handle, 'readId', [type])
}

/**
 * Read one asynchronous DAQ, EVENT or SERV packet.
 * Resolves `null` when `timeout` elapses.
 * @category XCP
 */
export function XcpReadIndication(handle: string, timeout = 1000): Promise<number[] | null> {
  return xcpCommand(handle, 'readIndication', [timeout])
}

/** FREE_DAQ: free the dynamic DAQ list configuration. @category XCP */
export function XcpFreeDaq(handle: string): Promise<number[]> {
  return xcpCommand(handle, 'freeDaq')
}

/** ALLOC_DAQ: allocate a number of dynamic DAQ lists. @category XCP */
export function XcpAllocDaq(handle: string, daqCount: number): Promise<number[]> {
  return xcpCommand(handle, 'allocDaq', [daqCount])
}

/** ALLOC_ODT: allocate ODTs for a dynamic DAQ list. @category XCP */
export function XcpAllocOdt(
  handle: string,
  daqListNumber: number,
  odtCount: number
): Promise<number[]> {
  return xcpCommand(handle, 'allocOdt', [daqListNumber, odtCount])
}

/** ALLOC_ODT_ENTRY: allocate ODT entries for a dynamic ODT. @category XCP */
export function XcpAllocOdtEntry(
  handle: string,
  daqListNumber: number,
  odtNumber: number,
  odtEntriesCount: number
): Promise<number[]> {
  return xcpCommand(handle, 'allocOdtEntry', [daqListNumber, odtNumber, odtEntriesCount])
}

/** PROGRAM_START: begin a programming sequence. @category XCP */
export function XcpProgramStart(handle: string): Promise<number[]> {
  return xcpCommand(handle, 'programStart')
}

/** PROGRAM_CLEAR: erase a range of program memory. @category XCP */
export function XcpProgramClear(
  handle: string,
  mode: number,
  clearRange: number
): Promise<number[]> {
  return xcpCommand(handle, 'programClear', [mode, clearRange])
}

/** PROGRAM: program `data` to the current MTA. @category XCP */
export function XcpProgram(
  handle: string,
  numberOfElements: number,
  data: number[]
): Promise<number[]> {
  return xcpCommand(handle, 'program', [numberOfElements, data])
}

/** PROGRAM_RESET: reset the slave after a programming sequence. @category XCP */
export function XcpProgramReset(handle: string): Promise<number[]> {
  return xcpCommand(handle, 'programReset')
}

/**
 * GET_SEED + UNLOCK using the GenerateKeyEx DLL configured on this connection.
 * The DLL is the same one UDS secure access uses. `resource` is the XCP
 * resource byte (CAL/PAG = 1, DAQ = 4, STIM = 8, PGM = 16).
 * @category XCP
 */
export function XcpUnlockWithSeedKey(handle: string, resource: number): Promise<ResourceFlags> {
  return xcpCommand(handle, 'unlockWithSeedKey', [resource])
}
