/**
 * One XCP session against a live slave: the commands Frankie asked to prove,
 * plus DAQ when the slave publishes an address map.
 */
import fs from 'fs'
import path from 'path'
import { XcpError } from 'src/main/xcp/xcpProtocol'
import { XcpMaster } from 'src/main/xcp/xcpMaster'

import { parseA2lObjects, type A2lObject } from 'src/main/share/a2l'

export { parseA2lObjects, type A2lObject }

export function findA2l(dir: string): string | null {
  if (!fs.existsSync(dir)) return null
  const names = fs.readdirSync(dir).filter((name) => name.toLowerCase().endsWith('.a2l'))
  return names.length ? path.join(dir, names[0]) : null
}

function readInt(buf: Buffer, size: number): number {
  if (size === 1) return buf[0]
  if (size === 2) return buf.readUInt16LE(0)
  return buf.readUInt32LE(0)
}

function writeInt(value: number, size: number): number[] {
  const buf = Buffer.alloc(size)
  if (size === 1) buf[0] = value & 0xff
  else if (size === 2) buf.writeUInt16LE(value & 0xffff, 0)
  else buf.writeUInt32LE(value >>> 0, 0)
  return Array.from(buf)
}

export interface SessionTarget {
  addr: number
  ext: number
  size: number
  event?: number
  segment?: number
}

export interface SessionOptions {
  /** Repeat GET_STATUS this many times (the xcp_test latency loop, shortened). */
  statusRepeats?: number
  upload?: SessionTarget
  download?: SessionTarget
  daq?: SessionTarget
  /** Try PROGRAM_START and expect the slave to reject it. */
  expectProgramUnsupported?: boolean
  /**
   * Connect, status, identity and a read of `upload` only.
   * Used while another thread inside the slave is writing calibration data.
   */
  readonly?: boolean
  /** GET_ID types to request. Defaults to ASCII, ASAM name, A2L and EPK. */
  idTypes?: number[]
}

/** Choose a non-stack measurement and a small integer characteristic from an A2L. */
export function sessionOptionsFromA2l(
  objects: A2lObject[],
  extra: Partial<SessionOptions> = {}
): { options: SessionOptions; note: string } {
  const prefer = (list: A2lObject[]) =>
    list.find((obj) => /counter/i.test(obj.name) && !/max/i.test(obj.name)) ??
    list.find((obj) => /counter|tick|cycle/i.test(obj.name)) ??
    list.find((obj) => obj.size === 4 || obj.size === 2 || obj.size === 1) ??
    list[0]
  const stable = objects.filter((obj) => obj.extension !== 2 && obj.size >= 1 && obj.size <= 4)
  const measurements = stable.filter((obj) => obj.kind === 'measurement')
  const withEvent = measurements.filter((obj) => obj.event != null)
  const stackDaq = objects.filter(
    (obj) =>
      obj.kind === 'measurement' &&
      obj.extension === 2 &&
      obj.event != null &&
      obj.size >= 1 &&
      obj.size <= 4
  )
  const characteristics = stable.filter(
    (obj) => obj.kind === 'characteristic' && !/delay|time_us|cycle_us|(^|\.)run$/i.test(obj.name)
  )
  const calChars = characteristics.filter((obj) => obj.address >= 0x80000000)
  const calPool = calChars.length ? calChars : characteristics
  const download =
    calPool.find((obj) => obj.size === 2) ??
    calPool.find((obj) => obj.size === 4 || obj.size === 1) ??
    calPool[0]
  // Stack variables are only valid while their event runs. Read a calibration
  // parameter instead when the demo has no absolute measurement.
  const upload = prefer(withEvent.length ? withEvent : measurements) ?? download
  const daq =
    (upload?.kind === 'measurement' && upload.event != null ? upload : prefer(withEvent)) ??
    prefer(stackDaq)
  const toTarget = (
    obj: A2lObject | undefined,
    withEventChannel: boolean
  ): SessionTarget | undefined => {
    if (!obj) return undefined
    return {
      addr: obj.address,
      ext: obj.extension,
      size: obj.size,
      event: withEventChannel ? obj.event : undefined,
      segment: obj.address >= 0x80000000 ? (obj.address >>> 16) & 0xffff : undefined
    }
  }
  const readonly = extra.readonly === true
  const note = [
    upload
      ? `upload ${upload.name} ext=${upload.extension} size=${upload.size}`
      : 'no upload target',
    readonly
      ? 'download skipped (read-only)'
      : download
        ? `download ${download.name}`
        : 'no download target',
    readonly
      ? 'daq skipped (read-only)'
      : daq?.event != null
        ? `daq ${daq.name} event=${daq.event}`
        : 'no daq target'
  ].join('; ')
  return {
    note,
    options: {
      statusRepeats: 3,
      expectProgramUnsupported: true,
      upload: toTarget(upload, false),
      download: toTarget(download, false),
      daq: daq?.event != null ? toTarget(daq, true) : undefined,
      ...extra
    }
  }
}

export interface CommandResult {
  command: string
  status: 'pass' | 'unsupported'
  detail: string
}

function unsupported(err: unknown): boolean {
  return err instanceof XcpError && (err.code === 0x20 || err.code === 0x24 || err.code === 0x10)
}

/**
 * Run the session. Throws on the first command the master should support and
 * the slave should answer. Records slave-side "not implemented" separately.
 */
export async function exerciseXcpSession(
  master: XcpMaster,
  options: SessionOptions
): Promise<CommandResult[]> {
  const results: CommandResult[] = []
  const note = (command: string, detail: string, status: 'pass' | 'unsupported' = 'pass') => {
    results.push({ command, status, detail })
  }

  const info = await master.connect(0)
  note(
    'CONNECT',
    `maxCto=${info.maxCto} maxDto=${info.maxDto} proto=0x${info.protocolLayerVersion.toString(16)} resources=${JSON.stringify(info.resource)}`
  )

  const status = await master.getStatus()
  note('GET_STATUS', `daqRunning=${status.sessionStatus.daqRunning}`)
  const repeats = options.statusRepeats ?? 1
  for (let i = 1; i < repeats; i++) await master.getStatus()
  if (repeats > 1) note('GET_STATUS x' + repeats, 'ok')

  try {
    const version = await master.getVersion()
    note('GET_VERSION', JSON.stringify(version))
  } catch (err) {
    if (!unsupported(err)) throw err
    note('GET_VERSION', (err as Error).message, 'unsupported')
  }

  if (info.commModeBasic.optional) {
    const comm = await master.getCommModeInfo()
    note('GET_COMM_MODE_INFO', JSON.stringify(comm))
  } else {
    try {
      const comm = await master.getCommModeInfo()
      note('GET_COMM_MODE_INFO', JSON.stringify(comm))
    } catch (err) {
      if (!unsupported(err)) throw err
      note('GET_COMM_MODE_INFO', 'not advertised and rejected', 'unsupported')
    }
  }

  for (const type of options.idTypes ?? [0, 1, 4, 5]) {
    const name = ['ASCII', 'ASAM_NAME', 'ASAM_PATH', 'URL', 'A2L', 'EPK'][type] ?? String(type)
    try {
      const id = await master.readId(type)
      const preview = Buffer.from(id.data.slice(0, 48)).toString('latin1').replace(/\n/g, ' ')
      note(`GET_ID ${name}`, `mode=0x${id.mode.toString(16)} length=${id.length} "${preview}"`)
    } catch (err) {
      // 0x22: this identification type is outside the set the slave implements.
      if (!unsupported(err) && !(err instanceof XcpError && err.code === 0x22)) throw err
      note(`GET_ID ${name}`, (err as Error).message, 'unsupported')
    }
  }

  if (info.resource.calpag) {
    try {
      const pag = await master.getPagProcessorInfo()
      note('GET_PAG_PROCESSOR_INFO', JSON.stringify(pag))
    } catch (err) {
      if (!unsupported(err)) throw err
      note('GET_PAG_PROCESSOR_INFO', (err as Error).message, 'unsupported')
    }
  }

  if (options.upload) {
    const { addr, ext, size } = options.upload
    let short = await master.shortUpload(size, addr, ext)
    await master.setMta(addr, ext)
    let uploaded = await master.upload(size)
    // A free-running counter moves between the two commands. Retry for a stable
    // variable, and accept a small integer delta when the signal is live.
    for (let attempt = 0; attempt < 4 && !short.equals(uploaded); attempt++) {
      short = await master.shortUpload(size, addr, ext)
      await master.setMta(addr, ext)
      uploaded = await master.upload(size)
    }
    if (!short.equals(uploaded)) {
      if (short.length !== uploaded.length || short.length > 4) {
        throw new Error(
          `SET_MTA+UPLOAD ${uploaded.toString('hex')} != SHORT_UPLOAD ${short.toString('hex')}`
        )
      }
      const delta = Math.abs(readInt(uploaded, uploaded.length) - readInt(short, short.length))
      if (delta > 100000) {
        throw new Error(
          `SET_MTA+UPLOAD ${uploaded.toString('hex')} != SHORT_UPLOAD ${short.toString('hex')}`
        )
      }
    }
    note('SET_MTA+UPLOAD', `addr=0x${addr.toString(16)} ext=${ext} ${short.toString('hex')}`)
    note('SHORT_UPLOAD', short.toString('hex'))
  }

  if (options.download && !options.readonly) {
    const { addr, ext, size } = options.download
    const segment = options.download.segment ?? (addr >= 0x80000000 ? (addr >>> 16) & 0xffff : 0)
    try {
      await master.setCalPage(0x03, segment, 0)
      const page = await master.getCalPage(0x02, segment)
      note('SET_CAL_PAGE/GET_CAL_PAGE', `segment=${segment} page=${page}`)
    } catch (err) {
      if (!unsupported(err)) throw err
      note('SET_CAL_PAGE/GET_CAL_PAGE', (err as Error).message, 'unsupported')
    }
    const before = await master.shortUpload(size, addr, ext)
    const next = (readInt(before, size) + 1) >>> 0
    await master.setMta(addr, ext)
    await master.download(writeInt(next, size))
    const after = await master.shortUpload(size, addr, ext)
    if (readInt(after, size) !== next) {
      throw new Error(
        `DOWNLOAD read-back ${after.toString('hex')} != written ${next.toString(16)} (was ${before.toString('hex')})`
      )
    }
    await master.setMta(addr, ext)
    await master.download(Array.from(before))
    const restored = await master.shortUpload(size, addr, ext)
    if (!restored.equals(before)) {
      throw new Error('DOWNLOAD restore failed')
    }
    note(
      'DOWNLOAD',
      `addr=0x${addr.toString(16)} ${before.toString('hex')} -> ${after.toString('hex')} restored`
    )
  }

  if (options.daq && info.resource.daq && !options.readonly) {
    const { addr, ext, size, event } = options.daq
    if (event == null) throw new Error('DAQ target has no event channel')
    const proc = await master.getDaqProcessorInfo()
    const resolution = await master.getDaqResolutionInfo()
    note('GET_DAQ_PROCESSOR_INFO', JSON.stringify(proc))
    note('GET_DAQ_RESOLUTION_INFO', JSON.stringify(resolution))
    const ev = await master.getDaqEventInfo(event)
    note('GET_DAQ_EVENT_INFO', JSON.stringify(ev))
    await master.freeDaq()
    await master.allocDaq(1)
    await master.allocOdt(0, 1)
    await master.allocOdtEntry(0, 0, 1)
    await master.setDaqPtr(0, 0, 0)
    await master.writeDaq(0, size, ext, addr)
    // Bit 4: timestamp. XCPlite's timestamp is fixed-width, so the field is present either way.
    await master.setDaqListMode(0x10, 0, event, 1, 0)
    // Mode 2 selects the list. XCPlite (and the ASAM sequence) starts the
    // selected lists with START_STOP_SYNCH mode 1; a direct start is rejected
    // when the slave is built with its command checks enabled.
    const started = await master.startStopDaqList(2, 0)
    await master.startStopSynch(1)
    note('DAQ setup+start', `firstPid=${started.firstPid} event=${event}`)
    let baseline: Buffer | null = null
    try {
      baseline = await master.shortUpload(size, addr, ext)
    } catch (err) {
      // Extension 2 is the stack frame captured when the event fires.
      if (ext !== 2 || !(err instanceof XcpError)) throw err
    }
    const packets: Buffer[] = []
    const deadline = Date.now() + 1500
    while (packets.length < 6 && Date.now() < deadline) {
      const packet = await master.readIndication(200)
      if (packet && packet[0] < 0xfc) packets.push(packet)
    }
    await master.startStopSynch(0)
    if (packets.length < 2) {
      throw new Error(
        `DAQ produced ${packets.length} packets: ${packets.map((p) => p.toString('hex')).join(' | ')}`
      )
    }
    const uploaded = baseline ? readInt(baseline, size) : null
    let matched: number[] | null = null
    // Absolute PID (offset 1), relative PID + byte DAQ id (2), word DAQ id (4),
    // plus the same layouts with a 4-byte timestamp (5, 6, 8). XCPlite uses
    // ODT, align, DAQ word, then a dword timestamp, so the value starts at 8.
    for (const offset of [1, 2, 4, 5, 6, 8, 9]) {
      const values: number[] = []
      let ok = true
      for (const packet of packets) {
        if (packet.length < offset + size) {
          ok = false
          break
        }
        values.push(readInt(packet.subarray(offset, offset + size), size))
      }
      if (!ok) continue
      // Without a memory snapshot, only the XCPlite data field (after the
      // dword timestamp) counts. Earlier offsets are the ODT header or the clock.
      if (uploaded == null && offset !== 8) continue
      const changed = values.some((value) => value !== values[0])
      const near = uploaded != null && values.some((value) => Math.abs(value - uploaded) < 1e7)
      const exact = uploaded != null && values.every((value) => value === uploaded)
      // Stack-relative signals cannot be read outside the event; a changing field is enough.
      if (exact || (changed && (near || uploaded == null))) {
        matched = values
        break
      }
    }
    if (!matched) {
      throw new Error(
        `DAQ bytes did not contain ${baseline ? baseline.toString('hex') : 'a changing field'}: ${packets.map((p) => p.toString('hex')).join(' | ')}`
      )
    }
    note('DAQ data', matched.join(','))
    try {
      const t1 = await master.getDaqClock()
      await new Promise((resolve) => setTimeout(resolve, 5))
      const t2 = await master.getDaqClock()
      if (t2 === t1) throw new Error(`GET_DAQ_CLOCK did not advance (${t1})`)
      note('GET_DAQ_CLOCK', `${t1} -> ${t2}`)
    } catch (err) {
      if (!unsupported(err)) throw err
      note('GET_DAQ_CLOCK', (err as Error).message, 'unsupported')
    }
  } else if (info.resource.daq) {
    try {
      const t1 = await master.getDaqClock()
      await new Promise((resolve) => setTimeout(resolve, 5))
      const t2 = await master.getDaqClock()
      note('GET_DAQ_CLOCK', `${t1} -> ${t2}`)
    } catch (err) {
      if (!unsupported(err)) throw err
      note('GET_DAQ_CLOCK', (err as Error).message, 'unsupported')
    }
  }

  if (options.expectProgramUnsupported && !options.readonly) {
    try {
      await master.programStart()
      note('PROGRAM_START', 'slave accepted programming')
    } catch (err) {
      if (!unsupported(err) && !(err instanceof XcpError)) throw err
      note('PROGRAM_START', err instanceof Error ? err.message : String(err), 'unsupported')
    }
  }

  await master.disconnect()
  note('DISCONNECT', 'ok')
  return results
}
