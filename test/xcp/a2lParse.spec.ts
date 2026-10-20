import { describe, expect, it } from 'vitest'
import { parseA2lEvents, parseA2lObjects } from './a2lParse'

const A2L = `
/begin MEASUREMENT counter ""
  UBYTE 0 0 0 255
  ECU_ADDRESS 0x1000
  ECU_ADDRESS_EXTENSION 1
  /begin IF_DATA XCP
    /begin DAQ_EVENT VARIABLE
      /begin AVAILABLE_EVENT_LIST
        EVENT 1
      /end AVAILABLE_EVENT_LIST
    /end DAQ_EVENT
  /end IF_DATA
/end MEASUREMENT
/begin CHARACTERISTIC gain ""
  VALUE 0x80010000 UWORD 0 100
/end CHARACTERISTIC
/begin EVENT "mainloop" "mainloop" 0x1 DAQ 0xFF 0 0 0 CONSISTENCY EVENT /end EVENT
`

describe('XCPlite A2L test helper', () => {
  it('parses measurements, characteristics and events', () => {
    const objects = parseA2lObjects(A2L)
    expect(objects.find((item) => item.name === 'counter')).toMatchObject({
      kind: 'measurement',
      address: 0x1000,
      extension: 1,
      size: 1,
      event: 1
    })
    expect(objects.find((item) => item.name === 'gain')).toMatchObject({
      kind: 'characteristic',
      address: 0x80010000,
      size: 2
    })
    expect(parseA2lEvents(A2L)).toEqual([{ name: 'mainloop', channel: 1 }])
  })
})
