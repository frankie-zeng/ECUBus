import { afterEach, describe, expect, it } from 'vitest'
import {
  listXcpWorkerPresence,
  noteXcpWorkerClose,
  noteXcpWorkerOpen,
  onXcpWorkerPresence,
  resetXcpWorkerPresence
} from 'src/main/xcp/presence'

describe('XCP worker presence', () => {
  afterEach(() => {
    resetXcpWorkerPresence()
    onXcpWorkerPresence(() => undefined)
  })

  it('tracks saved connections a worker opens and closes', () => {
    const seen: string[][] = []
    onXcpWorkerPresence((rows) => {
      seen.push(rows.filter((row) => row.open).map((row) => row.id))
    })
    noteXcpWorkerOpen('inline', 'temp')
    noteXcpWorkerOpen('can-sim', 'can-sim')
    noteXcpWorkerOpen('can-sim', 'can-sim')
    expect(listXcpWorkerPresence()).toEqual([{ id: 'can-sim', name: 'can-sim', open: true }])
    noteXcpWorkerClose('can-sim')
    expect(listXcpWorkerPresence()[0].open).toBe(true)
    noteXcpWorkerClose('can-sim')
    expect(listXcpWorkerPresence()).toEqual([])
    expect(seen).toEqual([['can-sim'], ['can-sim'], ['can-sim'], []])
  })

  it('clears every handle when the project stops', () => {
    noteXcpWorkerOpen('ecu', 'ecu')
    resetXcpWorkerPresence()
    expect(listXcpWorkerPresence()).toEqual([])
  })
})
