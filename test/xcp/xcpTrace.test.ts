import { describe, expect, it } from 'vitest'
import { ByteOrder, XcpCommand } from 'src/main/xcp/xcpProtocol'
import {
  daqIdFieldFromKeyByte,
  describeXcpRx,
  describeXcpTx,
  xcpCommandName,
  xcpErrorName,
  xcpEventName,
  xcpServiceName
} from 'src/main/xcp/xcpTrace'

describe('XCP trace names', () => {
  it('decodes command PIDs, including the GET_VERSION sub-command', () => {
    expect(xcpCommandName(XcpCommand.CONNECT)).toBe('CONNECT')
    expect(xcpCommandName(XcpCommand.SHORT_UPLOAD)).toBe('SHORT_UPLOAD')
    expect(xcpCommandName(XcpCommand.GET_VERSION, 0x00)).toBe('GET_VERSION')
    expect(xcpCommandName(0xab)).toBe('CMD_AB')
  })

  it('decodes standard and unknown error codes', () => {
    expect(xcpErrorName(0x20)).toBe('ERR_CMD_UNKNOWN')
    expect(xcpErrorName(0x00)).toBe('ERR_CMD_SYNCH')
    expect(xcpErrorName(0x3f)).toBe('ERR_3F')
  })

  it('decodes event and service codes', () => {
    expect(xcpEventName(0x06)).toBe('EV_DAQ_OVERLOAD')
    expect(xcpEventName(0xfe)).toBe('EV_USER')
    expect(xcpEventName(0x1c)).toBe('EV_1C')
    expect(xcpServiceName(0x00)).toBe('SERV_RESET')
    expect(xcpServiceName(0x01)).toBe('SERV_TEXT')
    expect(xcpServiceName(0x09)).toBe('SERV_09')
  })

  it('labels a command with the connection name', () => {
    const row = describeXcpTx(Buffer.from([XcpCommand.CONNECT, 0x00]), 'can-sim')
    expect(row).toMatchObject({
      method: 'xcpSent',
      kind: 'cmd',
      name: 'can-sim.CONNECT',
      symbol: 'CONNECT',
      dir: 'out'
    })
  })

  it('labels RES, ERR, EV and SERV', () => {
    expect(describeXcpRx(Buffer.from([0xff, 0x1d]), 'can-sim')).toMatchObject({
      method: 'xcpRecv',
      kind: 'res',
      name: 'can-sim.RES'
    })
    const err = describeXcpRx(Buffer.from([0xfe, 0x20]), 'can-sim')
    expect(err).toMatchObject({
      method: 'xcpErr',
      kind: 'err',
      name: 'can-sim.ERR_CMD_UNKNOWN',
      detail: 'unknown command or not implemented'
    })
    expect(describeXcpRx(Buffer.from([0xfd, 0x06]), 'ecu')).toMatchObject({
      method: 'xcpEvent',
      kind: 'ev',
      name: 'ecu.EV_DAQ_OVERLOAD'
    })
    expect(describeXcpRx(Buffer.from([0xfc, 0x01]), 'ecu')).toMatchObject({
      method: 'xcpEvent',
      kind: 'serv',
      name: 'ecu.SERV_TEXT'
    })
  })

  it('reads the DAQ identification field from DAQ_KEY_BYTE', () => {
    expect(daqIdFieldFromKeyByte(0x00)).toBe('absolute-odt')
    expect(daqIdFieldFromKeyByte(0x40)).toBe('relative-byte')
    expect(daqIdFieldFromKeyByte(0x80)).toBe('relative-word')
    expect(daqIdFieldFromKeyByte(0xc0)).toBe('relative-word-aligned')
  })

  it('labels DAQ and STIM with the list and ODT', () => {
    expect(describeXcpRx(Buffer.from([0x03, 0x11]), 'can-sim')).toMatchObject({
      method: 'xcpDaq',
      kind: 'daq',
      name: 'can-sim.DAQ.ODT3',
      odt: 3,
      detail: 'ODT 3'
    })
    expect(
      describeXcpRx(Buffer.from([0x01, 0x04, 0xaa]), 'can-sim', 'relative-byte')
    ).toMatchObject({
      name: 'can-sim.DAQ4.ODT1',
      daqList: 4,
      odt: 1,
      detail: 'DAQ list 4, ODT 1'
    })
    expect(
      describeXcpRx(Buffer.from([0x02, 0x34, 0x12]), 'can-sim', 'relative-word', ByteOrder.INTEL)
    ).toMatchObject({
      name: 'can-sim.DAQ4660.ODT2',
      daqList: 0x1234
    })
    expect(
      describeXcpRx(
        Buffer.from([0x02, 0x00, 0x12, 0x34]),
        'can-sim',
        'relative-word-aligned',
        ByteOrder.MOTOROLA
      )
    ).toMatchObject({
      name: 'can-sim.DAQ4660.ODT2',
      daqList: 0x1234
    })
    expect(describeXcpTx(Buffer.from([0x01, 0x02]), 'can-sim', 'relative-byte')).toMatchObject({
      method: 'xcpDaq',
      kind: 'stim',
      name: 'can-sim.STIM2.ODT1',
      dir: 'out'
    })
  })
})
