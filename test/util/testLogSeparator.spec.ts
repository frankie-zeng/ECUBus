import { describe, expect, it } from 'vitest'
import {
  beginTestLogEntry,
  createTestLogSeparatorState,
  currentTestLogIndent,
  endTestLogEntry,
  isTestLogFrameHidden,
  isTestLogStdoutHidden,
  openTestLogFrames,
  resetTestLogSeparator,
  restartTestLogIds
} from '../../src/renderer/src/views/uds/testLogSeparator'

function start(
  state: ReturnType<typeof createTestLogSeparatorState>,
  name: string,
  nesting: number,
  line = 1
) {
  return beginTestLogEntry(state, { name, nesting, line, column: 1 })
}

function finish(
  state: ReturnType<typeof createTestLogSeparatorState>,
  name: string,
  nesting: number,
  line = 1
) {
  return endTestLogEntry(state, { name, nesting, line, column: 1 })
}

describe('test log case grouping', () => {
  it('bands sibling cases and keeps nested subtests inside the parent', () => {
    const state = createTestLogSeparatorState()

    expect(start(state, 'CAN Test', 0)).toMatchObject({ nesting: 0, indent: 0 })
    expect(openTestLogFrames(state).map((frame) => frame.band)).toEqual([false])

    expect(start(state, 'pass case', 1)).toMatchObject({ indent: 8 })
    expect(openTestLogFrames(state).map((frame) => frame.band)).toEqual([false, true])
    finish(state, 'pass case', 1)

    expect(start(state, 'fail case', 1).indent).toBe(8)
    expect(openTestLogFrames(state).at(-1)?.band).toBe(true)
    finish(state, 'fail case', 1)

    start(state, 'parent with subtests', 1)
    expect(start(state, 'nested one', 2)).toMatchObject({ indent: 16 })
    expect(currentTestLogIndent(state)).toBe(16)
    expect(openTestLogFrames(state).at(-1)?.band).toBe(false)
    expect(openTestLogFrames(state).at(-2)?.band).toBe(true)
    finish(state, 'nested one', 2)
    expect(currentTestLogIndent(state)).toBe(8)

    start(state, 'nested two', 2)
    expect(currentTestLogIndent(state)).toBe(16)
    finish(state, 'nested two', 2)
    finish(state, 'parent with subtests', 1)
    expect(finish(state, 'CAN Test', 0)).toEqual({
      id: 'CAN Test:0',
      promoteBand: false
    })
  })

  it('promotes a top-level test with no children into its own band', () => {
    const state = createTestLogSeparatorState()
    start(state, 'lone', 0, 4)
    expect(openTestLogFrames(state)[0].band).toBe(false)
    expect(finish(state, 'lone', 0, 4)).toEqual({ id: 'lone:0', promoteBand: true })
  })

  it('indents deeper subtests further', () => {
    const state = createTestLogSeparatorState()
    start(state, 'suite', 0)
    start(state, 'parent', 1)
    expect(start(state, 'child', 2).indent).toBe(16)
    expect(start(state, 'grandchild', 3).indent).toBe(24)
    finish(state, 'grandchild', 3)
    finish(state, 'child', 2)
    expect(currentTestLogIndent(state)).toBe(8)
  })

  it('keeps a file root open when a same-nesting suite finishes', () => {
    const state = createTestLogSeparatorState()
    start(state, '/tmp/suite.js', 0, 1)
    start(state, 'CAN Test', 0, 3)
    start(state, 'pass case', 1, 5)
    finish(state, 'pass case', 1, 5)
    finish(state, 'CAN Test', 0, 3)
    expect(openTestLogFrames(state).map((frame) => frame.name)).toEqual(['/tmp/suite.js'])
    expect(openTestLogFrames(state)[0].band).toBe(false)
    expect(openTestLogFrames(state)[0].hadChild).toBe(true)
  })

  it('matches tree node ids when a config prefix is set', () => {
    const state = createTestLogSeparatorState()
    state.prefix = 'cfg'
    expect(start(state, 'CAN Test', 0).id).toBe('cfg:0')
    expect(start(state, 'pass case', 1).id).toBe('cfg:1')
  })

  it('restarts ids on the next run so a single-case filter still matches the tree', () => {
    const state = createTestLogSeparatorState()
    state.prefix = 'cfg'
    const script = [
      { name: 'live logs', nesting: 0 },
      { name: 'case one', nesting: 1 },
      { name: 'case two', nesting: 1 },
      { name: 'case three', nesting: 1 }
    ]

    const play = () => {
      const ids: string[] = []
      const suite = start(state, script[0].name, script[0].nesting)
      ids.push(suite.id)
      for (const node of script.slice(1)) {
        const begun = start(state, node.name, node.nesting)
        ids.push(begun.id)
        finish(state, node.name, node.nesting)
      }
      finish(state, script[0].name, script[0].nesting)
      return ids
    }

    const treeIds = play()
    expect(treeIds).toEqual(['cfg:0', 'cfg:1', 'cfg:2', 'cfg:3'])
    const caseTwo = treeIds[2]

    // The per-case play button keeps the terminal text, so it must restart ids
    // itself. Leaving seq where the previous run stopped hides the next case.
    const stale = play()
    expect(stale.includes(caseTwo)).toBe(false)
    expect(isTestLogFrameHidden([caseTwo], stale[2])).toBe(true)
    expect(isTestLogStdoutHidden([caseTwo], [stale[0], stale[2]])).toBe(true)

    restartTestLogIds(state)
    expect(state.prefix).toBe('cfg')
    const again = play()
    expect(again).toEqual(treeIds)
    expect(isTestLogFrameHidden([caseTwo], again[2])).toBe(false)
    expect(isTestLogStdoutHidden([caseTwo], [again[0], again[2]])).toBe(false)
    expect(isTestLogFrameHidden(undefined, again[2])).toBe(false)
  })

  it('resets grouping after the log is cleared', () => {
    const state = createTestLogSeparatorState()
    start(state, 'CAN Test', 0)
    resetTestLogSeparator(state)
    expect(start(state, 'CAN Test', 0).indent).toBe(0)
    expect(openTestLogFrames(state)).toEqual([
      {
        id: 'CAN Test:0',
        name: 'CAN Test',
        nesting: 0,
        band: false,
        hadChild: false
      }
    ])
  })
})
