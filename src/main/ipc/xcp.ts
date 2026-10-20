/**
 * GUI probe: connect with a project XCP config, read status and DAQ events, then disconnect.
 */
import { ipcMain } from 'electron'
import { XcpMaster } from '../xcp/xcpMaster'
import { openTransportFromConfig, readSlaveDaqEvents } from '../xcp/openConfig'
import { unlockWithSeedKey, dllSeedKeyProvider } from '../xcp/xcpSeedKey'
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

ipcMain.handle('ipc-xcp-unlock', async (_event, raw: XcpConnectionConfig, resource: number) => {
  if (!raw.seedKeyDll) throw new Error('seed & key DLL path is empty')
  return withMaster(raw, async (master) => {
    await master.connect(0)
    const flags = await unlockWithSeedKey(master, resource, dllSeedKeyProvider(raw.seedKeyDll))
    await master.disconnect().catch(() => undefined)
    return flags
  })
})
