/**
 * GUI probe: connect with a project XCP config, read status and DAQ events, then disconnect.
 */
import { BrowserWindow, ipcMain } from 'electron'
import { listXcpWorkerPresence, onXcpWorkerPresence } from '../xcp/presence'
import { XcpMaster } from '../xcp/xcpMaster'
import { openTransportFromConfig, readSlaveDaqEvents } from '../xcp/openConfig'
import { XcpConnectionConfig, XcpDaqEvent, validateXcpConnection } from '../share/xcpConfig'
import { findOpenCanDevice } from './uds'

async function withMaster<T>(
  raw: XcpConnectionConfig,
  run: (master: XcpMaster) => Promise<T>
): Promise<T> {
  const errors = validateXcpConnection(raw)
  if (errors.length) throw new Error(errors.join('\n'))
  const transport = openTransportFromConfig(raw, (device) => findOpenCanDevice(device))
  const master = new XcpMaster(transport, raw.timeout)
  try {
    return await run(master)
  } finally {
    master.close()
  }
}

onXcpWorkerPresence((rows) => {
  for (const win of BrowserWindow.getAllWindows()) {
    win.webContents.send('xcp-connection-status', rows)
  }
})

ipcMain.handle('ipc-xcp-get-status', () => listXcpWorkerPresence())

ipcMain.handle('ipc-xcp-probe', async (_event, raw: XcpConnectionConfig) => {
  return withMaster(raw, async (master) => {
    const connect = await master.connect(0)
    const status = await master.getStatus()
    let events: XcpDaqEvent[] = []
    if (connect.resource.daq) {
      try {
        events = await readSlaveDaqEvents(master)
      } catch {
        events = []
      }
    }
    await master.disconnect().catch(() => undefined)
    return { connect, status, events }
  })
})
