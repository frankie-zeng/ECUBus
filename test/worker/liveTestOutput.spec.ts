import { Worker } from 'node:worker_threads'
import { pathToFileURL } from 'node:url'
import fs from 'node:fs'
import os from 'node:os'
import path from 'node:path'
import { describe, expect, it } from 'vitest'
import { testSentinelFooterJs } from '../../src/main/docan/testSentinelFooter'

const bundlePath = path.resolve(__dirname, '../../resources/lib/js/index.js')
const reportPath = path.resolve(__dirname, '../../resources/lib/js/report.js')

interface Observed {
  at: number
  type: string
  name: string
  nesting?: number
  suite: boolean
}

/**
 * Drive the shipped worker `test()` and custom reporter inside `worker_threads`.
 *
 * The script loads a copy of the worker bundle. The reporter loads
 * `resources/lib/js/index.js`. Those are two module instances, matching a
 * compiled test script (esbuild inlines the library) plus `--test-reporter`.
 */
function runWorkerTest(
  body: string,
  enabled: Record<number, boolean>,
  doneName: string,
  options?: { only?: boolean; footer?: string }
) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'ecb-live-test-'))
  const workerCopy = path.join(dir, 'worker-copy.js')
  fs.copyFileSync(bundlePath, workerCopy)
  const file = path.join(dir, options?.footer ? 'live.cjs' : 'live.mjs')
  const setup = `worker.Util.start(undefined, {}, undefined, ${JSON.stringify(enabled)})
worker.Util.event.emit('__varFc')
const delay = (ms) => new Promise((resolve) => setTimeout(resolve, ms))
${body}
${options?.footer ?? ''}`
  fs.writeFileSync(
    file,
    options?.footer
      ? `const worker = require(${JSON.stringify(workerCopy)})\n${setup}\n`
      : `import { createRequire } from 'module'
const require = createRequire(import.meta.url)
const worker = require(${JSON.stringify(workerCopy)})
${setup}
`
  )
  const started = Date.now()
  const execArgv = ['--enable-source-maps', `--test-reporter=${pathToFileURL(reportPath).href}`]
  if (options?.only) execArgv.push('--test-only')
  return new Promise<Observed[]>((resolve, reject) => {
    const events: Observed[] = []
    const worker = new Worker(file, {
      stdout: true,
      stderr: true,
      env: {
        ...process.env,
        MODE: 'test',
        ONLY: options?.only ? 'true' : 'false'
      },
      execArgv
    })
    let settled = false
    const finish = (error?: Error) => {
      if (settled) return
      settled = true
      clearTimeout(timer)
      void worker.terminate()
      if (error) reject(error)
      else resolve(events)
    }
    const timer = setTimeout(() => {
      const detail = events.map((event) => `${event.at} ${event.type} ${event.name}`).join('\n')
      finish(new Error(`timed out waiting for ${doneName}\n${detail}`))
    }, 8000)
    worker.on('message', (msg: { payload?: { data?: Record<string, unknown> } }) => {
      const data = msg?.payload?.data
      if (!data || typeof data.type !== 'string') return
      const payload = (data.data ?? {}) as {
        name?: string
        message?: string
        nesting?: number
        details?: { type?: string }
      }
      const name = String(payload.name ?? payload.message ?? '')
      events.push({
        at: Date.now() - started,
        type: data.type,
        name,
        nesting: payload.nesting,
        suite: payload.details?.type === 'suite'
      })
      if (
        data.type === 'test:pass' &&
        payload.name === doneName &&
        payload.details?.type !== 'suite'
      ) {
        finish()
      }
    })
    worker.on('error', (error) => finish(error))
  })
}

function at(events: Observed[], type: string, name: string, nesting?: number) {
  return events.find(
    (event) =>
      event.type === type &&
      event.name === name &&
      !event.suite &&
      (nesting === undefined || event.nesting === nesting)
  )
}

describe('live test console output', () => {
  it('emits console.log while a long test is still running', async () => {
    const events = await runWorkerTest(
      `worker.describe('slow', () => {
  worker.test('slow', async () => {
    console.log('LIVE_EARLY')
    await delay(600)
    console.log('LIVE_MID')
    await delay(600)
  })
})
worker.test('after', () => {
  console.log('AFTER')
})`,
      { 0: true, 1: true },
      'after'
    )

    const dequeue = at(events, 'test:dequeue', 'slow', 1)
    const early = at(events, 'test:stdout', 'LIVE_EARLY')
    const mid = at(events, 'test:stdout', 'LIVE_MID')
    const pass = at(events, 'test:pass', 'slow', 1)
    const after = at(events, 'test:stdout', 'AFTER')
    expect(dequeue, JSON.stringify(events)).toBeTruthy()
    expect(early).toBeTruthy()
    expect(mid).toBeTruthy()
    expect(pass).toBeTruthy()
    expect(after).toBeTruthy()

    const order = [dequeue, early, mid, pass, after].map((event) => events.indexOf(event!))
    expect(order).toEqual([...order].sort((a, b) => a - b))
    expect(events.filter((event) => event.name === 'LIVE_EARLY')).toHaveLength(1)
    expect(events.filter((event) => event.name === 'LIVE_MID')).toHaveLength(1)

    // The synchronous prefix follows dequeue immediately. The mid-test line is
    // logged 600ms later and must not wait for the case to finish.
    expect(early!.at - dequeue!.at).toBeLessThan(250)
    expect(pass!.at - mid!.at).toBeGreaterThanOrEqual(350)
    expect(after!.at).toBeGreaterThanOrEqual(pass!.at)
  }, 10000)

  it('emits a failing test log before test:fail and keeps the next case aligned', async () => {
    const events = await runWorkerTest(
      `worker.test('fails', () => {
  console.log('FAIL_LOG')
  throw new Error('boom')
})
worker.test('next', () => {
  console.log('NEXT_LOG')
})`,
      { 0: true, 1: true },
      'next'
    )

    const failLog = at(events, 'test:stdout', 'FAIL_LOG')
    const fail = at(events, 'test:fail', 'fails')
    const nextLog = at(events, 'test:stdout', 'NEXT_LOG')
    const nextPass = at(events, 'test:pass', 'next')
    expect(failLog).toBeTruthy()
    expect(fail).toBeTruthy()
    expect(nextLog).toBeTruthy()
    expect(nextPass).toBeTruthy()
    const order = [failLog, fail, nextLog, nextPass].map((event) => events.indexOf(event!))
    expect(order).toEqual([...order].sort((a, b) => a - b))
    expect(events.filter((event) => event.name === 'FAIL_LOG')).toHaveLength(1)
  })

  it('still completes the discovery sentinel without capturing prints', async () => {
    const events = await runWorkerTest(
      `worker.test('seen', () => {
  console.log('SHOULD_NOT_BE_CAPTURED')
})`,
      { 0: true },
      '____ecubus_pro_test___',
      { only: true, footer: testSentinelFooterJs }
    )

    expect(events.some((event) => event.type === 'test:pass' && event.name === 'seen')).toBe(true)
    expect(
      events.some((event) => event.type === 'test:pass' && event.name === '____ecubus_pro_test___')
    ).toBe(true)
    expect(events.some((event) => event.name === 'SHOULD_NOT_BE_CAPTURED')).toBe(false)
  })
})
