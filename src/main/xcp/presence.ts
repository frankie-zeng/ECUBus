/**
 * Which saved XCP connections a worker currently has open.
 * Project Start does not open them. `XcpCreateConnection` does.
 * Project Stop closes any handle a worker still holds and clears this list.
 */
import type { XcpConnectionStatus } from '../share/xcpConfig'

const counts = new Map<string, { name: string; count: number }>()
let listener: ((rows: XcpConnectionStatus[]) => void) | undefined

export function onXcpWorkerPresence(fn: (rows: XcpConnectionStatus[]) => void) {
  listener = fn
}

export function listXcpWorkerPresence(): XcpConnectionStatus[] {
  return [...counts.entries()].map(([id, item]) => ({
    id,
    name: item.name,
    open: item.count > 0
  }))
}

function publish() {
  listener?.(listXcpWorkerPresence())
}

/** A worker opened a saved connection. Inline addresses are not tracked. */
export function noteXcpWorkerOpen(id: string, name: string) {
  if (!id || id === 'inline') return
  const current = counts.get(id) ?? { name, count: 0 }
  current.count += 1
  current.name = name
  counts.set(id, current)
  publish()
}

/** A worker closed one handle. The connection stays open while another handle remains. */
export function noteXcpWorkerClose(id?: string) {
  if (!id || id === 'inline') return
  const current = counts.get(id)
  if (!current) return
  current.count -= 1
  if (current.count <= 0) counts.delete(id)
  publish()
}

/** Drop every tracked handle. Call after project Stop has closed the transports. */
export function resetXcpWorkerPresence() {
  counts.clear()
  publish()
}
