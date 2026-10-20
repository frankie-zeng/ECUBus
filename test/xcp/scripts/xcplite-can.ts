/**
 * User script: the public worker API, compiled against resources/lib/js the
 * same way EcuBus-Pro compiles a project script. Addresses come from the
 * environment because the XCPlite slave prints them at startup.
 */
import {
  XcpAllocDaq,
  XcpAllocOdt,
  XcpAllocOdtEntry,
  XcpCloseConnection,
  XcpConnect,
  XcpCreateConnection,
  XcpDisconnect,
  XcpDownload,
  XcpFreeDaq,
  XcpGetCalPage,
  XcpGetCommModeInfo,
  XcpGetDaqClock,
  XcpGetStatus,
  XcpGetVersion,
  XcpProgramStart,
  XcpReadId,
  XcpReadIndication,
  XcpSetCalPage,
  XcpSetDaqListMode,
  XcpSetDaqPtr,
  XcpSetMta,
  XcpShortUpload,
  XcpStartStopDaqList,
  XcpStartStopSynch,
  XcpUpload,
  XcpWriteDaq
} from 'ECB'

function envNum(name: string): number {
  const value = Number(process.env[name])
  if (!Number.isFinite(value)) throw new Error(`missing ${name}`)
  return value
}

function same(a: number[], b: number[]): boolean {
  return a.length === b.length && a.every((byte, i) => byte === b[i])
}

async function main(): Promise<void> {
  const handle = await XcpCreateConnection({
    name: 'xcplite',
    canIdCmd: envNum('XCP_CMD'),
    canIdResp: envNum('XCP_RESP'),
    canfd: true,
    padding: false
  })
  const info = await XcpConnect(handle)
  if (info.maxCto > 64 || info.maxDto > 64) {
    throw new Error(`CAN-FD clamp missing: MAX_CTO=${info.maxCto} MAX_DTO=${info.maxDto}`)
  }
  const status = await XcpGetStatus(handle)
  const version = await XcpGetVersion(handle)
  const comm = info.commModeBasic.optional ? await XcpGetCommModeInfo(handle) : null
  const id = await XcpReadId(handle, 0)

  const addr = envNum('XCP_ADDR')
  const ext = envNum('XCP_EXT')
  const short = await XcpShortUpload(handle, 4, addr, ext)
  await XcpSetMta(handle, addr, ext)
  const uploaded = await XcpUpload(handle, 4)
  if (!same(short, uploaded)) {
    const delta = Math.abs(
      (short[0] | (short[1] << 8) | (short[2] << 16) | (short[3] << 24)) -
        (uploaded[0] | (uploaded[1] << 8) | (uploaded[2] << 16) | (uploaded[3] << 24))
    )
    if (delta > 100000) throw new Error(`UPLOAD ${uploaded} != SHORT_UPLOAD ${short}`)
  }

  const cal = envNum('XCP_CAL_ADDR')
  const seg = envNum('XCP_CAL_SEG')
  await XcpSetCalPage(handle, 0x03, seg, 0)
  const page = await XcpGetCalPage(handle, 0x02, seg)
  const before = await XcpShortUpload(handle, 4, cal, 0)
  const next = before.slice()
  next[0] = (next[0] + 1) & 0xff
  await XcpSetMta(handle, cal, 0)
  await XcpDownload(handle, next)
  const after = await XcpShortUpload(handle, 4, cal, 0)
  if (after[0] !== next[0]) throw new Error(`DOWNLOAD read-back ${after} != ${next}`)
  await XcpSetMta(handle, cal, 0)
  await XcpDownload(handle, before)

  const event = envNum('XCP_EVENT')
  await XcpFreeDaq(handle)
  await XcpAllocDaq(handle, 1)
  await XcpAllocOdt(handle, 0, 1)
  await XcpAllocOdtEntry(handle, 0, 0, 1)
  await XcpSetDaqPtr(handle, 0, 0, 0)
  await XcpWriteDaq(handle, 0, 4, ext, addr)
  await XcpSetDaqListMode(handle, 0x10, 0, event, 1, 0)
  await XcpStartStopDaqList(handle, 2, 0)
  await XcpStartStopSynch(handle, 1)
  let packets = 0
  const deadline = Date.now() + 1500
  while (packets < 2 && Date.now() < deadline) {
    const packet = await XcpReadIndication(handle, 200)
    if (packet && packet[0] < 0xfc) packets++
  }
  await XcpStartStopSynch(handle, 0)
  if (packets < 2) throw new Error(`DAQ produced ${packets} packets`)

  const t1 = await XcpGetDaqClock(handle)
  await new Promise((resolve) => setTimeout(resolve, 5))
  const t2 = await XcpGetDaqClock(handle)
  if (t1 === t2) throw new Error(`GET_DAQ_CLOCK stuck at ${t1}`)

  let program = 'accepted'
  try {
    await XcpProgramStart(handle)
  } catch (err) {
    program = err instanceof Error ? err.message : String(err)
  }

  await XcpDisconnect(handle)
  await XcpCloseConnection(handle)
  console.log(
    'XCPLITE_SCRIPT_OK ' +
      JSON.stringify({
        maxCto: info.maxCto,
        maxDto: info.maxDto,
        daqRunning: status.sessionStatus.daqRunning,
        version,
        comm,
        id: Buffer.from(id.data).toString('latin1'),
        page,
        upload: Buffer.from(short).toString('hex'),
        packets,
        clock: [t1, t2],
        program
      })
  )
  process.exit(0)
}

main().catch((err) => {
  console.error(err)
  process.exit(1)
})
