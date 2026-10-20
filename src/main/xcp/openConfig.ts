/**
 * Open the transport described by a saved XCP connection.
 */
import { CanBase } from '../docan/base'
import { XcpConnectionConfig, XcpDaqEvent } from '../share/xcpConfig'
import { XcpCanTransport } from './xcpCan'
import { XcpMaster, XcpTransport } from './xcpMaster'
import { XcpTcpTransport } from './xcpTcp'
import { XcpUdpTransport } from './xcpUdp'

export function openTransportFromConfig(
  cfg: XcpConnectionConfig,
  findCan?: (device?: string) => CanBase
): XcpTransport {
  if (cfg.transport === 'udp') return new XcpUdpTransport(cfg.ip, cfg.port)
  if (cfg.transport === 'tcp') return new XcpTcpTransport(cfg.ip, cfg.port)
  if (!findCan) throw new Error('CAN device is not available')
  const base = findCan(cfg.device)
  return new XcpCanTransport(base, {
    name: cfg.name,
    canIdCmd: cfg.canIdCmd,
    canIdResp: cfg.canIdResp,
    extended: cfg.extended,
    canfd: cfg.canfd,
    brs: cfg.brs,
    padding: cfg.padding,
    paddingValue: cfg.paddingValue,
    dlc: cfg.dlc
  })
}

/** CONNECT is already done. Read GET_DAQ_EVENT_INFO and the event name via UPLOAD. */
export async function readSlaveDaqEvents(master: XcpMaster): Promise<XcpDaqEvent[]> {
  const info = await master.getDaqProcessorInfo()
  const count = Math.min(info.maxEvent, 32)
  const events: XcpDaqEvent[] = []
  for (let channel = 0; channel < count; channel++) {
    const ev = await master.getDaqEventInfo(channel)
    let name = `event ${channel}`
    if (ev.nameLength > 0) {
      try {
        const raw = await master.uploadBytes(ev.nameLength)
        const text = raw.toString('utf8').replace(/\0+$/g, '').trim()
        if (text) name = text
      } catch {
        /* the channel number is still usable */
      }
    }
    events.push({
      channel,
      name,
      timeCycle: ev.timeCycle,
      timeUnit: ev.timeUnit,
      priority: ev.priority,
      source: 'slave'
    })
  }
  return events
}
