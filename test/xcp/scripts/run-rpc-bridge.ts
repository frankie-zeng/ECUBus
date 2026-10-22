/**
 * External SIMULATE_CAN node for a live XCPlite slave.
 *
 * The EcuBus project owns one simulate handle and, once started, listens for
 * JSON-RPC. This process joins a free handle and forwards XCP between that bus
 * and the slave's UDP port. It does not open SIMULATE_CAN itself.
 *
 *   RPC_HOST RPC_PORT RPC_HANDLE SLAVE_PORT CAN_ID_CMD CAN_ID_RESP CAN_BRS=0|1
 */
import { XcpliteCanBridge } from '../xcpliteBridge'

function envNum(name: string, fallback?: number): number {
  const raw = process.env[name]
  if (raw == null || raw === '') {
    if (fallback != null) return fallback
    throw new Error(`missing ${name}`)
  }
  const value = Number(raw)
  if (!Number.isFinite(value)) throw new Error(`bad ${name}`)
  return value
}

const bridge = new XcpliteCanBridge({
  host: process.env.RPC_HOST || '127.0.0.1',
  port: envNum('RPC_PORT', 17320),
  handle: envNum('RPC_HANDLE', 1),
  slavePort: envNum('SLAVE_PORT'),
  canIdCmd: envNum('CAN_ID_CMD', 0x7e0),
  canIdResp: envNum('CAN_ID_RESP', 0x7e1),
  brs: process.env.CAN_BRS === '1'
})

bridge
  .start()
  .then(() => {
    console.log(
      `XCPLITE_RPC_BRIDGE host=${bridge.host} port=${bridge.port} handle=${bridge.handle} hth=${bridge.hth}`
    )
  })
  .catch((err) => {
    console.error(err instanceof Error ? err.message : err)
    process.exit(1)
  })

async function shutdown() {
  await bridge.stop()
  process.exit(0)
}

process.on('SIGINT', () => void shutdown())
process.on('SIGTERM', () => void shutdown())
