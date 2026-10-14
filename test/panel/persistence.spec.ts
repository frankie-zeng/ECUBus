import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'
import { createSSRApp } from 'vue'
import { renderToString } from 'vue/server-renderer'
import path from 'node:path'
import type { PanelItem } from '../../src/preload/data'
import type { PanelDocument } from '../../src/preload/panel'
import PanelEditor from '../../src/renderer/src/views/uds/panel/panelEditor.vue'
import PanelView from '../../src/renderer/src/views/uds/panel/panelView.vue'
import { createControl, createDocument } from '../../src/renderer/src/views/uds/panel/free/model'

const state = vi.hoisted(() => ({
  panels: {} as Record<string, PanelItem>,
  save: undefined as { name: string; document: PanelDocument } | undefined,
  editor: undefined as PanelDocument | undefined,
  view: undefined as PanelDocument | undefined
}))
vi.mock('@r/stores/data', () => ({ useDataStore: () => state }))
vi.mock('element-plus', () => ({ ElMessage: { error: vi.fn(), success: vi.fn() } }))
vi.mock('../../src/renderer/src/views/uds/panel/free/FreePanelEditor.vue', () => ({
  default: {
    props: ['initialDocument', 'initialName', 'height', 'dialogTarget'],
    emits: ['save', 'dirty'],
    setup(props: { initialDocument: PanelDocument }, { emit }: { emit: (...args: any[]) => void }) {
      state.editor = props.initialDocument
      if (state.save) emit('save', state.save.name, state.save.document)
      return () => null
    }
  }
}))
vi.mock('../../src/renderer/src/views/uds/panel/free/FreePanelView.vue', () => ({
  default: {
    props: ['document', 'height'],
    setup(props: { document: PanelDocument }) {
      state.view = props.document
      return () => null
    }
  }
}))

async function render(editIndex: string, preview = false) {
  const app = createSSRApp(preview ? PanelView : PanelEditor, { editIndex, height: 550 })
  for (const name of ['el-button', 'el-dialog', 'el-table', 'el-table-column'])
    app.component(name, { render: () => null })
  app.provide('layout', { setWinModified: vi.fn(), changeWinName: vi.fn() })
  return renderToString(app)
}

const fileInvoke = vi.fn()
beforeEach(() => {
  fileInvoke.mockReset()
  vi.stubGlobal('window', {
    path,
    electron: {
      ipcRenderer: {
        invoke: fileInvoke
      }
    }
  })
  state.panels = {}
  state.save = undefined
  state.editor = undefined
  state.view = undefined
})
afterEach(() => vi.unstubAllGlobals())

describe('panel persistence entry points', () => {
  it('saves the document into the project without writing files or opening dialogs', async () => {
    const original = { id: 'main', name: 'Main', rule: [], options: {}, document: createDocument() }
    state.panels.main = original
    state.save = { name: 'Main', document: { ...original.document, width: 1200 } }
    await render('main')
    await vi.waitFor(() => expect(state.panels.main.document?.width).toBe(1200))
    expect(Object.keys(state.panels.main).sort()).toEqual([
      'document',
      'id',
      'name',
      'options',
      'rule'
    ])
    expect(fileInvoke).not.toHaveBeenCalled()
  })
  it('restores a newly saved document using the persisted editor and runtime window ids', async () => {
    const document = createDocument()
    document.controls = [
      createControl('tabs', 'tabs', 'Pages'),
      createControl('image', 'image', 'Image')
    ]
    const tabs = document.controls[0]
    document.controls[1].parentId = tabs.id
    document.controls[1].tabId = tabs.tabs![0].id
    document.controls[1].imageSrc = 'data:image/png;base64,aGVsbG8='
    state.save = { name: 'Saved panel', document }
    await render('new-panel-id')
    await vi.waitFor(() => expect(state.panels['new-panel-id']?.document).toEqual(document))
    expect(state.panels['new-panel-id'].document).not.toBe(document)

    state.panels = JSON.parse(JSON.stringify(state.panels))
    state.save = undefined
    await render('new-panel-id')
    expect(state.editor).toEqual(document)
    await render('pnew-panel-id', true)
    expect(state.view).toEqual(document)
  })

  it('updates the same panel on repeated saves without creating duplicates', async () => {
    state.save = { name: 'First', document: createDocument() }
    await render('stable-id')
    await vi.waitFor(() => expect(state.panels['stable-id']?.name).toBe('First'))
    state.save = { name: 'Renamed', document: createDocument() }
    await render('stable-id')
    await vi.waitFor(() => expect(state.panels['stable-id']?.name).toBe('Renamed'))
    expect(Object.keys(state.panels)).toEqual(['stable-id'])
    expect(state.panels['stable-id'].name).toBe('Renamed')
    expect(fileInvoke).not.toHaveBeenCalled()
  })

  it('keeps legacy rules when saving an automatically converted embedded panel', async () => {
    const document = createDocument()
    state.panels.old = { id: 'old', name: 'Old', document, rule: [{ type: 'TText' }], options: {} }
    state.save = { name: 'Old', document: { ...document, width: 1200 } }
    await render('old')
    await vi.waitFor(() => expect(state.panels.old.document?.width).toBe(1200))
    expect(state.panels.old.rule).toEqual([{ type: 'TText' }])
    expect(fileInvoke).not.toHaveBeenCalled()
  })

  it('shows the migration report for legacy panels without rewriting them', async () => {
    const legacy = { id: 'old', name: 'Old', rule: [{ type: 'input' }], options: {} }
    state.panels.old = legacy
    expect(await render('old')).toContain('migration-report')
    expect(state.editor).toBeUndefined()
    expect(await render('pold', true)).toContain('Legacy Panel')
    expect(state.view).toBeUndefined()
    expect(state.panels.old).toEqual(legacy)
    expect(state.panels.old.document).toBeUndefined()
  })
})
