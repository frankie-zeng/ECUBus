/**
 * EcuBus-Pro XCP master against Vector XCPlite.
 *
 * Every XCPlite program that is an XCP slave (UDP server) is driven by
 * {@link XcpMaster}. Programs that are not slaves are listed and skipped.
 * Build first with `bash tools/xcplite/build.sh` (also run from beforeAll).
 */
import { spawn, type ChildProcess } from 'child_process'
import { execFile } from 'child_process'
import dgram from 'dgram'
import net from 'net'
import { promisify } from 'util'
import fs from 'fs'
import os from 'os'
import path from 'path'
import { Worker } from 'worker_threads'
import { afterAll, beforeAll, describe, expect, it } from 'vitest'
import { CanBase } from 'src/main/docan/base'
import { SIMULATE_CAN } from 'src/main/docan/simulate'
import { XcpCanTransport } from 'src/main/xcp/xcpCan'
import { XcpMaster } from 'src/main/xcp/xcpMaster'
import { XcpTcpTransport } from 'src/main/xcp/xcpTcp'
import { XcpUdpTransport } from 'src/main/xcp/xcpUdp'
import { XcpliteCanBridge } from './xcpliteBridge'
import {
  exerciseXcpSession,
  findA2l,
  parseA2lObjects,
  sessionOptionsFromA2l,
  type A2lObject,
  type CommandResult,
  type SessionOptions
} from './xcpliteSession'

const execFileP = promisify(execFile)
const ROOT = process.cwd()
const BUILD = path.join(ROOT, 'tools/xcplite/upstream/build')
const BUILD_NO_A2L = path.join(ROOT, 'tools/xcplite/upstream/build-no_a2l')
const FIXTURE = path.join(ROOT, 'tools/xcplite/upstream/test/fixtures/hello_xcp.a2l')
const ARTIFACT = '/opt/cursor/artifacts/xcplite-report.json'
const UDP_PORT = 5555

export interface ReportRow {
  name: string
  group: string
  relevant: boolean
  reason: string
  result: 'pass' | 'fail' | 'not-run'
  detail: string
}

const report: ReportRow[] = []

function record(row: ReportRow): void {
  const existing = report.findIndex((item) => item.name === row.name && item.group === row.group)
  if (existing >= 0) report[existing] = row
  else report.push(row)
}

function skip(group: string, name: string, reason: string): void {
  record({ name, group, relevant: false, reason, result: 'not-run', detail: '' })
}

const NOT_RELEVANT: Array<[string, string, string]> = [
  ['test', 'type_detection_test_c', 'A2L _Generic type detection; no XCP session'],
  ['test', 'type_detection_test_cpp', 'A2L C++ trait detection; no XCP session'],
  [
    'test',
    'daq_config_test',
    'In-process XcpCommand() unit test; no transport a master can attach to'
  ],
  ['test', 'eth_transport_test', 'Ethernet framing and receive-split unit test'],
  ['test', 'socket_recv_test', 'Ethernet header/payload receive-split unit test'],
  ['test', 'passive_test_c', 'XCP_MODE_DEACTIVATE API contract; not a server'],
  ['test', 'passive_test_cpp', 'XCP_MODE_DEACTIVATE API contract; not a server'],
  ['test', 'a2l_test', 'A2L file generation checked by external a2ltool, not an XCP session'],
  ['test', 'queue_test', 'Lock-free queue benchmark; not an XCP slave'],
  [
    'test',
    'xcp_test',
    'XCPlite XCP-on-UDP client. Its command sequence is reproduced by this suite against the slaves'
  ],
  [
    'test',
    'socket_raw_test',
    'Raw-Ethernet HAL, XCPLITE_CONFIGURATION=raw; not built in the default config'
  ],
  ['test', 'test_socket_raw.sh', 'Needs root and a veth/netns pair'],
  ['test', 'test_bintool.sh', 'Needs a remote Raspberry Pi over SSH'],
  ['test', 'test_bpf_demo_pi.sh', 'Needs a remote Raspberry Pi over SSH'],
  [
    'test',
    'test_examples.sh',
    'XCPlite harness that diffs A2L fixtures; not a slave. This suite is the master side'
  ],
  ['example', 'bpf_demo', 'Not built (XCPLITE_BUILD_BPF_DEMO off, needs libbpf)'],
  [
    'example',
    'cmp_demo',
    'CMP protocol demo with its own codec; not an XCP command scenario and not in the default build'
  ],
  ['example', 'udp_raw_demo', 'Raw-Ethernet configuration; not built'],
  [
    'example',
    'freertos_emu_demo',
    'Separate rtos configuration (FetchContent FreeRTOS); same command surface as hello_xcp, not built'
  ],
  ['example', 'freertos_esp32_demo', 'Target hardware'],
  ['example', 'freertos_stm32_demo', 'Target hardware'],
  ['example', 'silkit_demo', 'Needs SIL Kit and the shm configuration'],
  [
    'example',
    'external_example',
    'Build-system wrapper of the same library API, not a distinct command scenario'
  ],
  [
    'example',
    'fetchcontent_example',
    'Build-system wrapper of the same library API, not a distinct command scenario'
  ],
  ['tool', 'xcpclient', 'Vector XCP master'],
  ['tool', 'xcpdaemon', 'Vector XCP tooling'],
  ['tool', 'ptptool', 'PTP helper, not an XCP slave'],
  ['tool', 'bintool', 'BIN/HEX helper, not an XCP slave'],
  ['tool', 'CANape projects', 'Tool configuration, not a test']
]

class SlaveProc {
  readonly proc: ChildProcess
  log = ''
  constructor(bin: string, args: string[], cwd?: string) {
    this.proc = spawn(bin, args, { cwd, stdio: ['ignore', 'pipe', 'pipe'] })
    const add = (buf: Buffer) => {
      this.log += buf.toString()
      if (this.log.length > 200_000) this.log = this.log.slice(-100_000)
    }
    this.proc.stdout?.on('data', add)
    this.proc.stderr?.on('data', add)
  }
  exited(): boolean {
    return this.proc.exitCode != null || this.proc.signalCode != null
  }
  async stop(): Promise<void> {
    if (this.exited()) return
    this.proc.kill('SIGTERM')
    await sleep(400)
    if (!this.exited()) this.proc.kill('SIGKILL')
    await new Promise((resolve) => {
      if (this.exited()) resolve(undefined)
      else this.proc.once('exit', () => resolve(undefined))
    })
  }
  waitExit(ms: number): Promise<number | null> {
    if (this.exited()) return Promise.resolve(this.proc.exitCode)
    return new Promise((resolve) => {
      const timer = setTimeout(() => resolve(null), ms)
      this.proc.once('exit', (code) => {
        clearTimeout(timer)
        resolve(code)
      })
    })
  }
}

function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

function bin(name: string, dir = BUILD): string {
  return path.join(dir, name)
}

function makeFdBase(handle: number): CanBase {
  return new SIMULATE_CAN({
    handle,
    name: 'xcp-xcplite',
    vendor: 'simulate',
    canfd: true,
    bitrate: { freq: 500000, timeSeg1: 0x0f, timeSeg2: 0x04, sjw: 0x01, preScaler: 0x01 },
    bitratefd: { freq: 2000000, timeSeg1: 0x0f, timeSeg2: 0x04, sjw: 0x01, preScaler: 0x01 },
    id: 'xcp-xcplite-' + handle
  } as any) as unknown as CanBase
}

function portFree(port: number, tcp: boolean): Promise<boolean> {
  return new Promise((resolve) => {
    if (!tcp) {
      const socket = dgram.createSocket('udp4')
      socket.once('error', () => {
        socket.close()
        resolve(false)
      })
      socket.bind(port, '0.0.0.0', () => {
        socket.close()
        resolve(true)
      })
      return
    }
    const server = net.createServer()
    server.once('error', () => {
      server.close()
      resolve(false)
    })
    server.listen(port, '0.0.0.0', () => {
      server.close(() => resolve(true))
    })
  })
}

async function waitPortFree(port: number, tcp: boolean): Promise<void> {
  for (let i = 0; i < 40; i++) {
    if (await portFree(port, tcp)) return
    await sleep(50)
  }
  throw new Error(`${tcp ? 'TCP' : 'UDP'} port ${port} is still in use`)
}

function openTransport(port: number, tcp: boolean) {
  return tcp ? new XcpTcpTransport('127.0.0.1', port) : new XcpUdpTransport('127.0.0.1', port)
}

async function waitForSlave(port: number, proc: SlaveProc, tcp: boolean, ms = 8000): Promise<void> {
  // Let the demo register measurements before CONNECT finalizes the A2L.
  await sleep(tcp ? 800 : 400)
  const start = Date.now()
  let last = 'no response'
  while (Date.now() - start < ms) {
    if (proc.exited()) {
      throw new Error(
        `process exited ${proc.proc.exitCode} before CONNECT\n${proc.log.slice(-800)}`
      )
    }
    const master = new XcpMaster(openTransport(port, tcp), 800)
    try {
      await master.connect(0)
      await master.disconnect()
      return
    } catch (err) {
      last = err instanceof Error ? err.message : String(err)
    } finally {
      master.close()
    }
    // XCPlite's TCP server waits ~100ms after a socket close before accept.
    await sleep(tcp ? 200 : 40)
  }
  throw new Error(`${tcp ? 'TCP' : 'UDP'} :${port} not ready (${last})\n${proc.log.slice(-800)}`)
}

async function readA2l(cwd: string): Promise<A2lObject[]> {
  const deadline = Date.now() + 1500
  let file: string | null = null
  while (Date.now() < deadline) {
    file = findA2l(cwd)
    if (file) break
    await sleep(40)
  }
  if (!file) return []
  return parseA2lObjects(fs.readFileSync(file, 'utf8'))
}

function summarize(results: CommandResult[], note: string): string {
  const pass = results.filter((row) => row.status === 'pass').map((row) => row.command)
  const unsupported = results
    .filter((row) => row.status === 'unsupported')
    .map((row) => row.command)
  return `${note}; pass=${pass.join(', ')}; unsupported=${unsupported.join(', ') || 'none'}`
}

async function driveSlave(opts: {
  bin: string
  args?: string[]
  port?: number
  tcp?: boolean
  readonly?: boolean
  waitForSelfCheck?: boolean
  requireMemory?: boolean
}): Promise<string> {
  // c_demo reads XcpEthServerStatus() before the transmit thread marks itself
  // running, and exits 0 if it loses that race. Retry the spawn only.
  let last: Error | undefined
  for (let attempt = 1; attempt <= 3; attempt++) {
    try {
      return await driveSlaveOnce(opts)
    } catch (err) {
      last = err instanceof Error ? err : new Error(String(err))
      if (!/before CONNECT/.test(last.message) || attempt === 3) throw last
      await sleep(150)
    }
  }
  throw last
}

async function driveSlaveOnce(opts: {
  bin: string
  args?: string[]
  port?: number
  tcp?: boolean
  readonly?: boolean
  waitForSelfCheck?: boolean
  requireMemory?: boolean
}): Promise<string> {
  const tcp = !!opts.tcp
  const port = opts.port ?? UDP_PORT
  await waitPortFree(port, tcp)
  const cwd = fs.mkdtempSync(path.join(os.tmpdir(), 'xcplite-'))
  const proc = new SlaveProc(opts.bin, opts.args ?? [], cwd)
  try {
    await waitForSlave(port, proc, tcp, opts.waitForSelfCheck ? 4000 : 8000)
    const objects = await readA2l(cwd)
    const picked = sessionOptionsFromA2l(objects, {
      readonly: opts.readonly,
      idTypes: opts.readonly ? [0, 1, 5] : undefined,
      expectProgramUnsupported: !opts.readonly,
      statusRepeats: opts.readonly ? 2 : 8
    })
    if (opts.readonly) {
      picked.options.download = undefined
      picked.options.daq = undefined
    }
    if (opts.requireMemory && !picked.options.upload) {
      throw new Error(
        `no non-stack measurement in the A2L\n${picked.note}\n${proc.log.slice(-600)}`
      )
    }
    const master = new XcpMaster(openTransport(port, tcp), 3000)
    let results: CommandResult[]
    try {
      results = await exerciseXcpSession(master, picked.options)
    } finally {
      master.close()
    }
    if (opts.waitForSelfCheck) {
      const code = await proc.waitExit(25000)
      if (code !== 0) {
        throw new Error(`XCPlite self-check exited ${code}\n${proc.log.slice(-1500)}`)
      }
    }
    return summarize(results, picked.note || 'session only')
  } finally {
    await proc.stop()
    fs.rmSync(cwd, { recursive: true, force: true })
  }
}

async function withEcubusSlave(
  port: number,
  run: (map: any, proc: SlaveProc) => Promise<string>
): Promise<string> {
  const proc = new SlaveProc(bin('ecubus_slave'), [String(port)])
  try {
    const start = Date.now()
    let map: any = null
    while (Date.now() - start < 4000) {
      const match = proc.log.match(/ECUBUS_SLAVE (\{.*\})/)
      if (match) {
        map = JSON.parse(match[1])
        break
      }
      if (proc.exited()) break
      await sleep(20)
    }
    if (!map) throw new Error(`ecubus_slave did not print its map\n${proc.log}`)
    return await run(map, proc)
  } finally {
    await proc.stop()
  }
}

function ecubusOptions(map: any): SessionOptions {
  return {
    statusRepeats: 8,
    expectProgramUnsupported: true,
    upload: { addr: map.counter.addr, ext: map.counter.ext, size: 4 },
    download: {
      addr: map.gain.addr,
      ext: map.gain.ext,
      size: 4,
      segment: map.gain.seg
    },
    daq: { addr: map.counter.addr, ext: map.counter.ext, size: 4, event: map.event }
  }
}

async function runWorkerScript(map: any, base: CanBase): Promise<string> {
  const outDir = fs.mkdtempSync(path.join(os.tmpdir(), 'xcplite-script-'))
  const esbuild = path.join(ROOT, 'resources/bin/esbuild_linux')
  const lib = path.join(ROOT, 'resources/lib/js')
  fs.chmodSync(esbuild, 0o755)
  await execFileP(esbuild, [
    path.join(ROOT, 'test/xcp/scripts/xcplite-can.ts'),
    '--bundle',
    '--platform=node',
    '--format=cjs',
    `--alias:ECB=${path.join(lib, 'index.js')}`,
    `--outdir=${outDir}`,
    `--inject:${path.join(lib, 'init.js')}`,
    '--external:*.node'
  ])
  const masters = new Map<string, XcpMaster>()
  let rpcId = 1
  const worker = new Worker(path.join(outDir, 'xcplite-can.js'), {
    stdout: true,
    stderr: true,
    env: {
      ...process.env,
      XCP_CMD: '2016',
      XCP_RESP: '2017',
      XCP_ADDR: String(map.counter.addr),
      XCP_EXT: String(map.counter.ext),
      XCP_CAL_ADDR: String(map.gain.addr),
      XCP_CAL_SEG: String(map.gain.seg),
      XCP_EVENT: String(map.event)
    }
  })
  let log = ''
  worker.on('message', (msg) => {
    if (!msg || msg.type !== 'event') return
    const { id, event, data } = msg.payload
    void (async () => {
      const reply = (body: { data?: any; err?: string }) => {
        worker.postMessage({
          type: 'rpc',
          id: rpcId++,
          method: '__eventDone',
          params: [id, body]
        })
      }
      try {
        reply({ data: await workerXcpApi(base, masters, event, data) })
      } catch (err) {
        reply({ err: err instanceof Error ? err.message : String(err) })
      }
    })()
  })
  worker.stdout?.on('data', (buf) => {
    log += buf.toString()
  })
  worker.stderr?.on('data', (buf) => {
    log += buf.toString()
  })
  const code: number = await new Promise((resolve, reject) => {
    const timer = setTimeout(() => {
      void worker.terminate()
      reject(new Error(`worker script timed out\n${log}`))
    }, 20000)
    worker.on('error', (err) => {
      clearTimeout(timer)
      reject(err)
    })
    worker.on('exit', (exitCode) => {
      clearTimeout(timer)
      resolve(exitCode ?? 1)
    })
  })
  for (const master of masters.values()) master.close()
  if (code !== 0 || !log.includes('XCPLITE_SCRIPT_OK')) {
    throw new Error(`worker script exit ${code}\n${log}`)
  }
  return (
    log
      .trim()
      .split('\n')
      .filter((line) => line.includes('XCPLITE_SCRIPT_OK'))
      .pop() ?? log
  )
}

async function workerXcpApi(
  base: CanBase,
  masters: Map<string, XcpMaster>,
  event: string,
  data: any
): Promise<any> {
  if (event !== 'xcpApi') throw new Error(`unexpected worker event ${event}`)
  if (data.op === 'createConnection') {
    const transport = new XcpCanTransport(base, data.addr)
    const master = new XcpMaster(transport, 3000)
    const handle = 'xcp-script'
    masters.set(handle, master)
    return handle
  }
  if (data.op === 'closeConnection') {
    const master = masters.get(data.handle)
    if (!master) throw new Error(`XCP handle '${data.handle}' not found`)
    master.close()
    masters.delete(data.handle)
    return null
  }
  if (data.op === 'command') {
    const master = masters.get(data.handle)
    if (!master) throw new Error(`XCP handle '${data.handle}' not found`)
    const fn = (master as any)[data.method]
    if (typeof fn !== 'function') throw new Error(`unknown xcp command ${data.method}`)
    const result = await fn.apply(master, data.args ?? [])
    return Buffer.isBuffer(result) ? Array.from(result) : result
  }
  throw new Error(`unknown xcpApi op ${data.op}`)
}

describe.sequential('XCPlite slaves driven by the EcuBus-Pro XCP master', () => {
  beforeAll(async () => {
    await execFileP('bash', [path.join(ROOT, 'tools/xcplite/build.sh')], {
      cwd: ROOT,
      timeout: 180000
    })
    for (const [group, name, reason] of NOT_RELEVANT) skip(group, name, reason)
  }, 180000)

  afterAll(() => {
    fs.mkdirSync(path.dirname(ARTIFACT), { recursive: true })
    fs.writeFileSync(ARTIFACT, JSON.stringify(report, null, 2))
    const lines = [
      '| Name | Group | Relevant | Result | Reason / detail |',
      '| --- | --- | --- | --- | --- |',
      ...report.map(
        (row) =>
          `| ${row.name} | ${row.group} | ${row.relevant ? 'yes' : 'no'} | ${row.result} | ${(row.relevant ? row.detail : row.reason).replace(/\|/g, '/').replace(/\n/g, ' ')} |`
      )
    ]
    fs.writeFileSync('/opt/cursor/artifacts/xcplite-report.md', lines.join('\n') + '\n')
  })

  it('parses the hello_xcp A2L fixture into measurement and calibration targets', () => {
    const objects = parseA2lObjects(fs.readFileSync(FIXTURE, 'utf8'))
    const counter = objects.find((obj) => obj.name === 'global_counter')
    const cal = objects.find((obj) => obj.name === 'params.counter_max')
    expect(counter).toMatchObject({ kind: 'measurement', extension: 1, size: 4, event: 1 })
    expect(cal).toMatchObject({
      kind: 'characteristic',
      extension: 0,
      size: 2,
      address: 0x80010000
    })
    const picked = sessionOptionsFromA2l(objects)
    expect(picked.options.upload?.addr).toBe(counter?.address)
    expect(picked.options.download?.addr).toBe(0x80010000)
    expect(picked.options.daq?.event).toBe(1)
  })

  it('ecubus_slave over UDP', async () => {
    const detail = await withEcubusSlave(18031, async (map) => {
      const master = new XcpMaster(new XcpUdpTransport('127.0.0.1', map.port), 2000)
      try {
        const results = await exerciseXcpSession(master, ecubusOptions(map))
        return summarize(results, `counter=0x${map.counter.addr.toString(16)}`)
      } finally {
        master.close()
      }
    })
    record({
      name: 'ecubus_slave UDP',
      group: 'harness',
      relevant: true,
      reason: 'Dedicated slave built for this suite',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DISCONNECT')
  }, 20000)

  it('ecubus_slave over simulate CAN-FD', async () => {
    const masterBase = makeFdBase(20)
    const bridgeBase = makeFdBase(21)
    let bridge: XcpliteCanBridge | undefined
    try {
      const detail = await withEcubusSlave(18032, async (map) => {
        bridge = new XcpliteCanBridge(bridgeBase, map.port, 0x7e0, 0x7e1)
        await bridge.start()
        const master = new XcpMaster(
          new XcpCanTransport(masterBase, {
            name: 'xcplite',
            canIdCmd: 0x7e0,
            canIdResp: 0x7e1,
            canfd: true,
            padding: false
          }),
          3000
        )
        try {
          const results = await exerciseXcpSession(master, ecubusOptions(map))
          expect(results[0].detail).toContain('maxCto=64')
          expect(results[0].detail).toContain('maxDto=64')
          return summarize(results, 'CAN-FD bridge, MAX_CTO/DTO clamped to 64')
        } finally {
          master.close()
        }
      })
      record({
        name: 'ecubus_slave CAN-FD bridge',
        group: 'harness',
        relevant: true,
        reason: 'Same slave, packets carried on the simulate CAN bus',
        result: 'pass',
        detail
      })
    } finally {
      await bridge?.stop()
      masterBase.close()
      bridgeBase.close()
    }
  }, 20000)

  it('public worker script over simulate CAN-FD', async () => {
    const masterBase = makeFdBase(22)
    const bridgeBase = makeFdBase(23)
    let bridge: XcpliteCanBridge | undefined
    try {
      const detail = await withEcubusSlave(18033, async (map) => {
        bridge = new XcpliteCanBridge(bridgeBase, map.port, 0x7e0, 0x7e1)
        await bridge.start()
        return runWorkerScript(map, masterBase)
      })
      record({
        name: 'worker script xcplite-can.ts',
        group: 'harness',
        relevant: true,
        reason: 'Public Xcp* API compiled against the shipped worker bundle',
        result: 'pass',
        detail
      })
      expect(detail).toContain('XCPLITE_SCRIPT_OK')
    } finally {
      await bridge?.stop()
      masterBase.close()
      bridgeBase.close()
    }
  }, 30000)

  const examples: Array<{ name: string; tcp?: boolean }> = [
    { name: 'hello_xcp' },
    { name: 'hello_xcp_cpp' },
    { name: 'c_demo' },
    { name: 'cpp_demo' },
    { name: 'struct_demo', tcp: true },
    { name: 'multi_thread_demo' },
    { name: 'point_cloud_demo', tcp: true }
  ]
  for (const example of examples) {
    it(`example ${example.name}`, async () => {
      const detail = await driveSlave({
        bin: bin(example.name),
        tcp: example.tcp,
        requireMemory: true
      })
      record({
        name: example.name,
        group: 'example',
        relevant: true,
        reason: example.tcp ? 'Long-running XCP-on-TCP server' : 'Long-running XCP-on-UDP server',
        result: 'pass',
        detail
      })
      expect(detail).toContain('DOWNLOAD')
      expect(detail).toContain('DAQ data')
    }, 60000)
  }

  it('example no_a2l_demo', async () => {
    const detail = await driveSlave({ bin: bin('no_a2l_demo', BUILD_NO_A2L), requireMemory: false })
    record({
      name: 'no_a2l_demo',
      group: 'example',
      relevant: true,
      reason:
        'UDP server with runtime A2L off; session commands, memory only if an address is published',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DISCONNECT')
  }, 30000)

  it('example no_a2l_demo_cpp', async () => {
    const detail = await driveSlave({
      bin: bin('no_a2l_demo_cpp', BUILD_NO_A2L),
      requireMemory: false
    })
    record({
      name: 'no_a2l_demo_cpp',
      group: 'example',
      relevant: true,
      reason:
        'UDP server with runtime A2L off; session commands, memory only if an address is published',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DISCONNECT')
  }, 30000)

  it('example ptp4l_demo', async () => {
    const proc = new SlaveProc(
      bin('ptp4l_demo'),
      [],
      fs.mkdtempSync(path.join(os.tmpdir(), 'xcplite-'))
    )
    await sleep(600)
    if (proc.exited()) {
      const detail = proc.log.trim().slice(0, 300)
      record({
        name: 'ptp4l_demo',
        group: 'example',
        relevant: false,
        reason: `Needs ptp4l/pmc on a PTP clock. Actual startup: ${detail}`,
        result: 'not-run',
        detail
      })
      expect(proc.log).toMatch(/ptp|pmc/i)
      return
    }
    await proc.stop()
    const detail = await driveSlave({ bin: bin('ptp4l_demo'), requireMemory: false })
    record({
      name: 'ptp4l_demo',
      group: 'example',
      relevant: true,
      reason: 'UDP server; PTP timestamps when ptp4l is present',
      result: 'pass',
      detail
    })
  }, 30000)

  it('test daq_test', async () => {
    const detail = await driveSlave({ bin: bin('daq_test'), requireMemory: true })
    record({
      name: 'daq_test',
      group: 'test',
      relevant: true,
      reason: 'Interactive multi-thread DAQ server',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DAQ data')
  }, 60000)

  it('test clock_test', async () => {
    const detail = await driveSlave({ bin: bin('clock_test'), requireMemory: false })
    record({
      name: 'clock_test',
      group: 'test',
      relevant: true,
      reason: 'Interactive clock server',
      result: 'pass',
      detail
    })
    expect(detail).toContain('GET_DAQ_CLOCK')
  }, 30000)

  it('test cal_test --server', async () => {
    const detail = await driveSlave({
      bin: bin('cal_test'),
      args: ['--server'],
      readonly: true,
      waitForSelfCheck: true,
      requireMemory: false
    })
    record({
      name: 'cal_test --server',
      group: 'test',
      relevant: true,
      reason:
        'Calibration stress test that also serves XCP; master stays read-only so the self-check can finish',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DISCONNECT')
  }, 40000)

  it('test sr_a2l_test', async () => {
    const detail = await driveSlave({ bin: bin('sr_a2l_test'), requireMemory: true })
    record({
      name: 'sr_a2l_test',
      group: 'test',
      relevant: true,
      reason:
        'hello_xcp-style server shipped in test/ but not a CMake target; built by tools/xcplite/build.sh',
      result: 'pass',
      detail
    })
    expect(detail).toContain('DAQ data')
  }, 60000)
})
