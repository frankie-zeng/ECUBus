import { readFileSync, writeFileSync } from 'node:fs'

const file = new URL('./HtmlControl.ecb', import.meta.url)
const text = readFileSync(file, 'utf8')
const eol = text.includes('\r\n') ? '\r\n' : '\n'
const project = JSON.parse(text)
const control = project.data.panels['html-demo'].document.controls.find(
  (item) => item.id === 'html'
)
const source = (name) => readFileSync(new URL(name, import.meta.url), 'utf8').replace(/\r\n/g, '\n')
control.htmlContent = source('./control.html')
control.scriptContent = source('./control.js')
writeFileSync(file, JSON.stringify(project, null, 2).replace(/\n/g, eol) + eol)
