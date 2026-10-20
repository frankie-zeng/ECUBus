/**
 * Test-only ASAM A2L reader. XCPlite publishes measurement addresses in an A2L
 * file next to the slave. The product does not parse A2L.
 */

export interface A2lObject {
  name: string
  kind: 'measurement' | 'characteristic'
  address: number
  extension: number
  size: number
  event?: number
}

export interface A2lEvent {
  channel: number
  name: string
}

const TYPE_SIZE: Record<string, number> = {
  UBYTE: 1,
  SBYTE: 1,
  UWORD: 2,
  SWORD: 2,
  ULONG: 4,
  SLONG: 4,
  A_UINT64: 8,
  A_INT64: 8,
  FLOAT32_IEEE: 4,
  FLOAT64_IEEE: 8,
  U8: 1,
  U16: 2,
  U32: 4,
  S8: 1,
  S16: 2,
  S32: 4,
  F32: 4,
  F64: 8
}

function parseNumber(text: string): number {
  return text.startsWith('0x') || text.startsWith('0X') ? parseInt(text, 16) : parseInt(text, 10)
}

/** Pull measurement and characteristic addresses out of an A2L file. */
export function parseA2lObjects(text: string): A2lObject[] {
  const objects: A2lObject[] = []
  const meas =
    /\/begin MEASUREMENT\s+(\S+)\s+"[^"]*"\s+(\S+)[\s\S]*?ECU_ADDRESS\s+(0x[0-9A-Fa-f]+|\d+)\s+ECU_ADDRESS_EXTENSION\s+(\d+)([\s\S]*?)\/end MEASUREMENT/g
  for (const match of text.matchAll(meas)) {
    const size = TYPE_SIZE[match[2]] ?? 0
    const eventMatch = /EVENT\s+(0x[0-9A-Fa-f]+|\d+)/.exec(match[5])
    objects.push({
      name: match[1],
      kind: 'measurement',
      address: parseNumber(match[3]),
      extension: parseNumber(match[4]),
      size,
      event: eventMatch ? parseNumber(eventMatch[1]) : undefined
    })
  }
  const chars = /\/begin CHARACTERISTIC\s+(\S+)\s+"[^"]*"\s+\S+\s+(0x[0-9A-Fa-f]+|\d+)\s+(\S+)/g
  for (const match of text.matchAll(chars)) {
    const size = TYPE_SIZE[match[3]] ?? 0
    const address = parseNumber(match[2])
    objects.push({
      name: match[1],
      kind: 'characteristic',
      address,
      extension: address >= 0x80000000 ? 0 : 1,
      size
    })
  }
  const typedefSize = new Map<string, number>()
  const typedefRe = /\/begin TYPEDEF_CHARACTERISTIC\s+(\S+)\s+"[^"]*"\s+\S+\s+(\S+)/g
  for (const match of text.matchAll(typedefRe)) {
    const size = TYPE_SIZE[match[2]]
    if (size) typedefSize.set(match[1], size)
  }
  const structs = new Map<
    string,
    { size: number; components: { name: string; type: string; offset: number }[] }
  >()
  const structRe =
    /\/begin TYPEDEF_STRUCTURE\s+(\S+)\s+"[^"]*"\s+(0x[0-9A-Fa-f]+|\d+)([\s\S]*?)\/end TYPEDEF_STRUCTURE/g
  for (const match of text.matchAll(structRe)) {
    const components: { name: string; type: string; offset: number }[] = []
    const compRe = /\/begin STRUCTURE_COMPONENT\s+(\S+)\s+(\S+)\s+(0x[0-9A-Fa-f]+|\d+)/g
    for (const comp of match[3].matchAll(compRe)) {
      components.push({ name: comp[1], type: comp[2], offset: parseNumber(comp[3]) })
    }
    structs.set(match[1], { size: parseNumber(match[2]), components })
  }
  const instRe =
    /\/begin INSTANCE\s+(\S+)\s+"[^"]*"\s+(\S+)\s+(0x[0-9A-Fa-f]+|\d+)(?:\s+ECU_ADDRESS_EXTENSION\s+(\d+))?/g
  for (const match of text.matchAll(instRe)) {
    const layout = structs.get(match[2])
    if (!layout) continue
    const base = parseNumber(match[3])
    const extension = match[4] != null ? parseNumber(match[4]) : base >= 0x80000000 ? 0 : 1
    if (!(base >= 0x80000000 || extension === 0 || extension === 1)) continue
    const components = [...layout.components].sort((a, b) => a.offset - b.offset)
    components.forEach((comp, index) => {
      const next = index + 1 < components.length ? components[index + 1].offset : layout.size
      const stride = next - comp.offset
      const typed = typedefSize.get(comp.type)
      const size = typed != null && typed <= stride ? typed : stride
      if (size !== 1 && size !== 2 && size !== 4) return
      objects.push({
        name: `${match[1]}.${comp.name}`,
        kind: 'characteristic',
        address: (base + comp.offset) >>> 0,
        extension,
        size
      })
    })
  }
  return objects
}

/** EVENT blocks: `/begin EVENT "name" "short" <channel>`. */
export function parseA2lEvents(text: string): A2lEvent[] {
  const events: A2lEvent[] = []
  const re = /\/begin EVENT\s+"([^"]+)"\s+"[^"]*"\s+(0x[0-9A-Fa-f]+|\d+)/g
  for (const match of text.matchAll(re)) {
    events.push({ name: match[1], channel: parseNumber(match[2]) })
  }
  return events
}
