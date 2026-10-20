/**
 * XCP GET_SEED / UNLOCK using the same GenerateKeyEx DLL as UDS secure access.
 */
import { SecureAccessDll } from '../worker/secureAccess'
import { XcpMaster } from './xcpMaster'
import type { ResourceFlags } from './xcpProtocol'

export interface SeedKeyProvider {
  /** Compute the key for a seed. `resource` is the XCP resource byte (CAL=1, DAQ=4, ...). */
  generateKey(seed: Buffer, resource: number): Buffer
}

/** Load a UDS GenerateKeyEx DLL. The security level argument is the XCP resource byte. */
export function dllSeedKeyProvider(dllPath: string): SeedKeyProvider {
  const sa = new SecureAccessDll(dllPath)
  return {
    generateKey(seed: Buffer, resource: number): Buffer {
      return sa.GenerateKeyEx(seed, resource, Buffer.from([0]), Buffer.alloc(64))
    }
  }
}

/**
 * GET_SEED (and continuation parts) then UNLOCK.
 * @param resource - Resource protection bit to unlock.
 */
export async function unlockWithSeedKey(
  master: XcpMaster,
  resource: number,
  provider: SeedKeyProvider
): Promise<ResourceFlags> {
  const first = await master.getSeed(0, resource)
  const parts = [...first.seed]
  let remaining = first.length - first.seed.length
  while (remaining > 0) {
    const more = await master.getSeed(1, resource)
    parts.push(...more.seed)
    remaining -= more.seed.length
    if (more.seed.length === 0) break
  }
  const key = provider.generateKey(Buffer.from(parts), resource)
  return master.unlock(key.length, key)
}
