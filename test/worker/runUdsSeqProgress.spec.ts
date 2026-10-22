import { Worker } from 'node:worker_threads'
import fs from 'node:fs'
import os from 'node:os'
import path from 'node:path'
import { describe, expect, it } from 'vitest'
import { withUdsSeqItemProgress, type UdsSeqProgress } from '../../src/main/share/udsSeqProgress'

const bundlePath = path.resolve(__dirname, '../../resources/lib/js/index.js')
const typingsPath = path.resolve(__dirname, '../../src/main/share/index.d.ts.html')

async function sampleProgress(): Promise<UdsSeqProgress[]> {
  const events: UdsSeqProgress[] = []
  const items = [
    { index: 0, total: 3, name: 'Session', serviceId: '0x10' },
    { index: 1, total: 3, name: 'Read', serviceId: '0x22' },
    { index: 2, total: 3, name: 'Reset', serviceId: '0x11' }
  ]
  for (const item of items) {
    await withUdsSeqItemProgress(
      (info) => {
        events.push({ ...info })
      },
      item,
      async () => {}
    )
  }
  return events
}

interface RunRequest {
  id: number
  data: { name: string; device?: string; progressId?: number }
}

interface ScriptResult {
  requests: RunRequest['data'][]
  progress: UdsSeqProgress[]
  deviceProgress: string[]
  optionsDevice: string | undefined
  plainOk: boolean
  deviceOk: boolean
  progressOk: boolean
  progressError?: string
  failError?: string
  failPhases: string[]
  afterFailOk: boolean
}

/**
 * Load the shipped worker bundle in a real worker thread and drive `runUdsSeq`
 * the way the main process does: progress RPCs, then `__eventDone`.
 */
function runScript(progress: UdsSeqProgress[]) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'ecb-uds-progress-'))
  const workerCopy = path.join(dir, 'worker-copy.js')
  fs.copyFileSync(bundlePath, workerCopy)
  const file = path.join(dir, 'script.cjs')
  fs.writeFileSync(
    file,
    `const { parentPort } = require('worker_threads')
const api = require(${JSON.stringify(workerCopy)})

function ok(label, promise) {
  return promise.then(
    () => ({ label, ok: true }),
    (err) => ({ label, ok: false, error: String(err && err.message ? err.message : err) })
  )
}

const progress = []
const deviceProgress = []
const failPhases = []

;(async () => {
  const plain = await ok('plain', api.runUdsSeq('Tester.Seq0'))
  const device = await ok('device', api.runUdsSeq('Tester.Seq0', 'Device1'))
  const progressed = await ok('progress', api.runUdsSeq('Tester.Seq0', {
    onProgress(info) {
      progress.push(info)
      if (info.phase === 'start' && info.index === 1) {
        throw new Error('callback boom')
      }
    }
  }))
  let optionsDevice
  const withDevice = await ok('withDevice', api.runUdsSeq('Tester.Seq0', 'Device2', {
    onProgress(info) {
      deviceProgress.push(info.name)
      optionsDevice = 'Device2'
    }
  }))
  const failed = await ok('fail', api.runUdsSeq('Tester.Fail', {
    onProgress(info) {
      failPhases.push(info.phase)
    }
  }))
  const afterFail = await ok('after', api.runUdsSeq('Tester.Seq0'))
  parentPort.postMessage({
    type: 'result',
    plain,
    device,
    progressed,
    withDevice,
    failed,
    afterFail,
    progress,
    deviceProgress,
    failPhases,
    optionsDevice
  })
})().catch((err) => {
  parentPort.postMessage({ type: 'script-error', error: String(err && err.stack ? err.stack : err) })
})
`
  )
  return new Promise<ScriptResult>((resolve, reject) => {
    const requests: RunRequest['data'][] = []
    const stderr: string[] = []
    const worker = new Worker(file, { stderr: true })
    let settled = false
    const finish = (error?: Error, result?: ScriptResult) => {
      if (settled) return
      settled = true
      clearTimeout(timer)
      void worker.terminate()
      if (error) reject(error)
      else resolve(result as ScriptResult)
    }
    const timer = setTimeout(() => {
      finish(
        new Error(`timed out\nstderr:\n${stderr.join('')}\nrequests:${JSON.stringify(requests)}`)
      )
    }, 8000)
    worker.stderr.on('data', (chunk) => {
      stderr.push(chunk.toString())
    })
    worker.on('message', (msg: any) => {
      if (msg?.type === 'event' && msg.payload?.event === 'runUdsSeq') {
        const id = msg.payload.id as number
        const data = msg.payload.data as RunRequest['data']
        requests.push(data)
        if (typeof data.progressId === 'number') {
          const steps = data.name === 'Tester.Fail' ? progress.slice(0, 1) : progress
          for (const info of steps) {
            worker.postMessage({
              type: 'rpc',
              id: -1,
              method: '__on',
              params: ['__udsSeqProgress', { id: data.progressId, info }]
            })
          }
        }
        if (data.name === 'Tester.Fail') {
          worker.postMessage({
            type: 'rpc',
            id: id + 1000,
            method: '__eventDone',
            params: [id, { err: 'negative response' }]
          })
        } else {
          worker.postMessage({
            type: 'rpc',
            id: id + 1000,
            method: '__eventDone',
            params: [id]
          })
        }
        return
      }
      if (msg?.type === 'result') {
        finish(undefined, {
          requests,
          progress: msg.progress,
          deviceProgress: msg.deviceProgress,
          optionsDevice: msg.optionsDevice,
          plainOk: msg.plain.ok,
          deviceOk: msg.device.ok,
          progressOk: msg.progressed.ok,
          progressError: msg.progressed.error,
          failError: msg.failed.error,
          failPhases: msg.failPhases,
          afterFailOk: msg.afterFail.ok && msg.withDevice.ok
        })
      }
      if (msg?.type === 'script-error') {
        finish(new Error(`${msg.error}\nstderr:\n${stderr.join('')}`))
      }
    })
    worker.on('error', (error) => finish(error))
  })
}

describe('runUdsSeq progress callback', () => {
  it('includes the callback in generated script typings', () => {
    expect(fs.existsSync(typingsPath)).toBe(true)
    const typings = fs.readFileSync(typingsPath, 'utf-8')
    expect(typings).toContain('interface UdsSeqProgress')
    expect(typings).toContain('onProgress?: (info: UdsSeqProgress) => void')
    expect(typings).toContain('declare function runUdsSeq')
    const bundle = fs.readFileSync(bundlePath, 'utf-8')
    expect(bundle).toContain('__udsSeqProgress')
  })

  it('reports item order from a worker script and keeps old calls working', async () => {
    const expected = await sampleProgress()
    const result = await runScript(expected)

    expect(result.plainOk).toBe(true)
    expect(result.deviceOk).toBe(true)
    expect(result.progressOk, result.progressError).toBe(true)
    expect(result.afterFailOk).toBe(true)
    expect(result.failError).toContain('negative response')
    expect(result.failPhases).toEqual(['start'])

    expect(
      result.requests.map((req) => ({
        name: req.name,
        device: req.device,
        progress: req.progressId
      }))
    ).toEqual([
      { name: 'Tester.Seq0', device: undefined, progress: undefined },
      { name: 'Tester.Seq0', device: 'Device1', progress: undefined },
      { name: 'Tester.Seq0', device: undefined, progress: expect.any(Number) },
      { name: 'Tester.Seq0', device: 'Device2', progress: expect.any(Number) },
      { name: 'Tester.Fail', device: undefined, progress: expect.any(Number) },
      { name: 'Tester.Seq0', device: undefined, progress: undefined }
    ])

    const progressIds = result.requests
      .map((req) => req.progressId)
      .filter((id): id is number => typeof id === 'number')
    expect(new Set(progressIds).size).toBe(progressIds.length)

    expect(result.progress).toEqual(expected)
    expect(result.deviceProgress).toEqual(['Session', 'Session', 'Read', 'Read', 'Reset', 'Reset'])
    expect(result.optionsDevice).toBe('Device2')
  }, 10000)
})
