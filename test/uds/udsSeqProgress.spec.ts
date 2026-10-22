import { describe, expect, it, vi } from 'vitest'
import {
  countRunnableUdsSeqItems,
  forwardUdsSeqProgress,
  invokeUdsSeqProgressCallback,
  udsSeqItemPct,
  withUdsSeqItemProgress,
  type UdsSeqProgress
} from '../../src/main/share/udsSeqProgress'

describe('uds sequence progress', () => {
  it('computes start and finish percentages', () => {
    expect(udsSeqItemPct(0, 4, 'start')).toBe(0)
    expect(udsSeqItemPct(0, 4, 'finish')).toBe(25)
    expect(udsSeqItemPct(1, 4, 'start')).toBe(25)
    expect(udsSeqItemPct(3, 4, 'finish')).toBe(100)
    expect(udsSeqItemPct(0, 3, 'finish')).toBe(33.33)
    expect(udsSeqItemPct(1, 3, 'finish')).toBe(66.67)
    expect(udsSeqItemPct(2, 3, 'finish')).toBe(100)
    expect(udsSeqItemPct(0, 1, 'start')).toBe(0)
    expect(udsSeqItemPct(0, 1, 'finish')).toBe(100)
    expect(udsSeqItemPct(0, 0, 'finish')).toBe(0)
  })

  it('counts only enabled items that resolve to a service', () => {
    const services = [
      { enable: true, serviceId: 'a' },
      { enable: false, serviceId: 'b' },
      { enable: true, serviceId: 'missing' },
      { enable: true, serviceId: 'c' }
    ]
    expect(countRunnableUdsSeqItems(services, (id) => id === 'a' || id === 'c')).toBe(2)
    expect(countRunnableUdsSeqItems([], () => true)).toBe(0)
  })

  it('emits start then finish in order with index, total and pct', async () => {
    const seen: UdsSeqProgress[] = []
    const ran: string[] = []
    const items = [
      { index: 0, total: 2, name: 'Session', serviceId: '0x10' },
      { index: 1, total: 2, name: 'Reset', serviceId: '0x11' }
    ]
    for (const item of items) {
      await withUdsSeqItemProgress(
        (info) => {
          seen.push(info)
        },
        item,
        async () => {
          ran.push(item.name)
        }
      )
    }

    expect(ran).toEqual(['Session', 'Reset'])
    expect(seen.map((info) => `${info.phase}:${info.index}:${info.pct}:${info.name}`)).toEqual([
      'start:0:0:Session',
      'finish:0:50:Session',
      'start:1:50:Reset',
      'finish:1:100:Reset'
    ])
    expect(seen.every((info) => info.total === 2)).toBe(true)
    expect(seen[0].serviceId).toBe('0x10')
  })

  it('does not emit finish when the item throws, and still rejects', async () => {
    const seen: string[] = []
    await expect(
      withUdsSeqItemProgress(
        (info) => {
          seen.push(info.phase)
        },
        { index: 0, total: 1, name: 'Read', serviceId: '0x22' },
        async () => {
          throw new Error('negative response')
        }
      )
    ).rejects.toThrow('negative response')
    expect(seen).toEqual(['start'])
  })

  it('keeps running when the callback throws or rejects', async () => {
    const error = vi.spyOn(console, 'error').mockImplementation(() => {})
    const seen: string[] = []
    const ran: number[] = []
    try {
      await withUdsSeqItemProgress(
        (info) => {
          seen.push(info.phase)
          throw new Error(`boom ${info.phase}`)
        },
        { index: 0, total: 2, name: 'Session', serviceId: '0x10' },
        async () => {
          ran.push(1)
        }
      )
      await withUdsSeqItemProgress(
        () => Promise.reject(new Error('async boom')),
        { index: 1, total: 2, name: 'Reset', serviceId: '0x11' },
        async () => {
          ran.push(2)
        }
      )
      await new Promise((resolve) => setTimeout(resolve, 0))
      expect(ran).toEqual([1, 2])
      expect(seen).toEqual(['start', 'finish'])
      expect(error).toHaveBeenCalled()
    } finally {
      error.mockRestore()
    }
  })

  it('ignores a missing callback', async () => {
    let ran = false
    await withUdsSeqItemProgress(undefined, { index: 0, total: 1, name: 'Session' }, async () => {
      ran = true
    })
    expect(ran).toBe(true)
    expect(() =>
      invokeUdsSeqProgressCallback(undefined, {
        index: 0,
        total: 1,
        pct: 0,
        name: 'Session',
        phase: 'start'
      })
    ).not.toThrow()
  })

  it('forwards progress to the calling worker, including id 0', () => {
    const sent: unknown[] = []
    const pool = {
      workerEmitNoWait(_event: string, data: unknown) {
        sent.push(data)
      }
    }
    const onProgress = forwardUdsSeqProgress(pool, 0)
    onProgress?.({
      index: 1,
      total: 3,
      pct: 33.33,
      name: 'Read',
      serviceId: '0x22',
      phase: 'start'
    })
    expect(sent).toEqual([
      {
        id: 0,
        info: {
          index: 1,
          total: 3,
          pct: 33.33,
          name: 'Read',
          serviceId: '0x22',
          phase: 'start'
        }
      }
    ])
    expect(forwardUdsSeqProgress(undefined, 1)).toBeUndefined()
    expect(forwardUdsSeqProgress(pool, undefined)).toBeUndefined()
  })

  it('does not throw when the worker bridge throws', () => {
    const error = vi.spyOn(console, 'error').mockImplementation(() => {})
    try {
      const onProgress = forwardUdsSeqProgress(
        {
          workerEmitNoWait() {
            throw new Error('port closed')
          }
        },
        4
      )
      expect(() =>
        onProgress?.({
          index: 0,
          total: 1,
          pct: 0,
          name: 'Session',
          phase: 'start'
        })
      ).not.toThrow()
    } finally {
      error.mockRestore()
    }
  })
})
