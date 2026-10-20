import { readFileSync } from 'node:fs'
import path from 'node:path'
import vm from 'node:vm'
import ts from 'typescript'
import { expect, it, vi } from 'vitest'

function execute(source: string, context: object) {
  vm.runInNewContext(
    ts.transpileModule(source, {
      compilerOptions: { target: ts.ScriptTarget.ES2022, module: ts.ModuleKind.CommonJS }
    }).outputText,
    context
  )
}

it.each(['Example.app', 'Example.APP', 'alias'])(
  'rejects application directories before shell.openPath: %s',
  async (name) => {
    const file = 'src/main/ipc/fs.ts'
    const source = ts.createSourceFile(
      file,
      readFileSync(file, 'utf8'),
      ts.ScriptTarget.Latest,
      true
    )
    const handler = source.statements.find((node) =>
      node.getText(source).startsWith("ipcMain.handle('ipc-panel-open-path'")
    )!
    let open!: (event: unknown, target: string) => Promise<boolean>
    const shell = { openPath: vi.fn(async () => '') }
    execute(handler.getText(source), {
      ipcMain: {
        handle: (_channel: string, callback: typeof open) => {
          open = callback
        }
      },
      fsP: {
        realpath: async () =>
          name === 'alias' ? '/Applications/Example.app' : `/Applications/${name}`,
        stat: async () => ({ isDirectory: () => true, isFile: () => false })
      },
      path,
      shell
    })
    await expect(open(null, name)).rejects.toThrow('Panel can only open')
    expect(shell.openPath).not.toHaveBeenCalled()
  }
)
