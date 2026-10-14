import { readFileSync } from 'node:fs'
import { expect, it } from 'vitest'

const legacy = JSON.parse(readFileSync('test/panel/fixtures/legacy-panels.json', 'utf8'))
function leaves(rules: any[]): any[] {
  return rules.flatMap((rule) => (rule.type === 'grid' ? leaves(rule.children) : [rule]))
}

for (const [name, original] of Object.entries(legacy) as [string, any][]) {
  it(`migrates ${name} without losing bindings or button behavior`, () => {
    const filename = `resources/examples/${name}.ecb`
    const project = JSON.parse(readFileSync(filename, 'utf8')).data
    project.database = { can: {}, lin: {}, orti: {}, ...project.database }
    project.database.can = Object.fromEntries(
      Object.entries(project.database.can).filter(
        ([, db]: [string, any]) => db.version === 'canmartix'
      )
    )
    for (const [id, oldPanel] of Object.entries(original.panels) as [string, any][]) {
      const panel = project.panels[id]
      expect(panel.filePath).toBeUndefined()
      expect(panel.rule).toEqual([])
      const controls = panel.document.controls
      const rules = leaves(oldPanel.rule)
      expect(controls).toHaveLength(rules.length)
      controls.forEach((control, index) => {
        const old = rules[index]
        const node = old.props?.variable || old.props?.signal
        if (node) {
          const target = control.binding!.node.bindValue as any
          expect(target.variableFullName || target.signalName).toBe(
            node.bindValue.variableFullName || node.bindValue.signalName
          )
          const binding = control.binding!
          if (binding.kind === 'signal')
            expect(
              project.database.can[target.dbKey] || project.database.lin[target.dbKey]
            ).toBeDefined()
          else if (binding.node.bindValue.variableType === 'user')
            expect(project.vars[target.variableId]).toBeDefined()
        }
        if (old.type === 'BButton') {
          expect(control.pressValue).toBe(old.props.pressValue)
          expect(control.releaseValue).toBe(old.props.releaseValue)
          expect(control.toggle).toBe(!!old.props.toggleMode)
        }
        if (old.type === 'input') {
          expect(control.editorMode).toBe('text')
          expect(
            control.binding?.kind === 'variable' && control.binding.node.bindValue.variableValueType
          ).toBe('string')
        }
        expect(control.x + control.width).toBeLessThanOrEqual(panel.document.width)
        expect(control.y + control.height).toBeLessThanOrEqual(panel.document.height)
      })
    }
  })
}

it('ships the standalone HTML control with matching editable source and isolated simulate channels', () => {
  const directory = 'resources/examples/panel_html'
  const project = JSON.parse(readFileSync(`${directory}/HtmlControl.ecb`, 'utf8')).data
  const source = (name: string) =>
    readFileSync(`${directory}/${name}`, 'utf8').replace(/\r\n/g, '\n')
  const html = project.panels['html-demo'].document.controls.find(
    (control: any) => control.id === 'html'
  )
  expect(html.htmlContent).toBe(source('control.html'))
  expect(html.scriptContent).toBe(source('control.js'))
  expect(project.nodes['html-ecu'].channel).toEqual(['html-ecu-can'])
  expect(project.ia['html-command'].action[0].channel).toBe('html-panel-can')
  expect(Object.values(project.devices).map((device: any) => device.canDevice.vendor)).toEqual([
    'simulate',
    'simulate'
  ])
})
