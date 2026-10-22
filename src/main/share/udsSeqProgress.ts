/**
 * Progress reported to a worker script while `runUdsSeq` executes a sequence.
 *
 * @category UDS
 */
export interface UdsSeqProgress {
  /**
   * 0-based index among enabled sequence items that will run.
   * Disabled items are omitted and do not advance this index.
   */
  index: number
  /** Number of enabled sequence items that will run. */
  total: number
  /**
   * Overall percent complete in the range 0–100.
   * `start` is the percent before this item runs; `finish` includes this item.
   * The first item starts at 0 and the last item finishes at 100.
   */
  pct: number
  /** Service name configured on the sequence item. */
  name: string
  /** UDS service id, for example `0x10`, when the service is known. */
  serviceId?: string
  /**
   * `start` immediately before the item runs, `finish` after it completes.
   * A failing item that stops the sequence emits `start` only.
   */
  phase: UdsSeqProgressPhase
}

/** @category UDS */
export type UdsSeqProgressPhase = 'start' | 'finish'

/** Callback invoked by the sequence runner. Throws are ignored. */
export type UdsSeqProgressHandler = (info: UdsSeqProgress) => void | Promise<void>

export function udsSeqItemPct(index: number, total: number, phase: UdsSeqProgressPhase): number {
  if (!Number.isFinite(total) || total <= 0 || !Number.isFinite(index)) return 0
  const done = phase === 'finish' ? index + 1 : index
  const ratio = Math.min(Math.max(done / total, 0), 1)
  return Math.round(ratio * 10000) / 100
}

/**
 * Enabled items that have a resolved service. This is the set `runSequence`
 * actually executes and the set `index` / `total` are counted against.
 */
export function countRunnableUdsSeqItems(
  services: Array<{ enable: boolean; serviceId: string }>,
  hasService: (serviceId: string) => boolean
): number {
  let total = 0
  for (const service of services) {
    if (service.enable && hasService(service.serviceId)) total++
  }
  return total
}

function progressError(err: unknown): string {
  return err instanceof Error ? err.message : String(err)
}

/**
 * Invoke a progress callback. Synchronous throws and rejected promises are
 * logged and do not propagate, so a script callback cannot abort the sequence.
 * The callback is not awaited.
 */
export function invokeUdsSeqProgressCallback(
  onProgress: UdsSeqProgressHandler | undefined,
  info: UdsSeqProgress
): void {
  if (!onProgress) return
  try {
    const ret = onProgress(info)
    if (ret && typeof (ret as Promise<void>).then === 'function') {
      void (ret as Promise<void>).catch((err) => {
        console.error(`runUdsSeq onProgress error: ${progressError(err)}`)
      })
    }
  } catch (err) {
    console.error(`runUdsSeq onProgress error: ${progressError(err)}`)
  }
}

/**
 * Emit `start`, run the item, then emit `finish`.
 * `finish` is skipped when `run` throws. Callback failures never reject `run`.
 */
export async function withUdsSeqItemProgress(
  onProgress: UdsSeqProgressHandler | undefined,
  item: { index: number; total: number; name: string; serviceId?: string },
  run: () => Promise<void>
): Promise<void> {
  const base = {
    index: item.index,
    total: item.total,
    name: item.name,
    serviceId: item.serviceId
  }
  invokeUdsSeqProgressCallback(onProgress, {
    ...base,
    pct: udsSeqItemPct(item.index, item.total, 'start'),
    phase: 'start'
  })
  await run()
  invokeUdsSeqProgressCallback(onProgress, {
    ...base,
    pct: udsSeqItemPct(item.index, item.total, 'finish'),
    phase: 'finish'
  })
}

/**
 * Parent-side bridge from the sequence runner to the worker that called `runUdsSeq`.
 * `progressId` correlates the event with that call. `0` is a valid id.
 */
export function forwardUdsSeqProgress(
  pool: { workerEmitNoWait(event: string, data: unknown): void } | undefined,
  progressId: number | undefined
): UdsSeqProgressHandler | undefined {
  if (typeof progressId !== 'number' || !pool) return undefined
  return (info) => {
    try {
      pool.workerEmitNoWait('__udsSeqProgress', { id: progressId, info })
    } catch (err) {
      console.error(`runUdsSeq onProgress error: ${progressError(err)}`)
    }
  }
}
