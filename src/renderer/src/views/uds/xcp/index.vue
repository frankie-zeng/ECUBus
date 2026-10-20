<template>
  <div class="main">
    <div class="left">
      <el-scrollbar :height="h + 'px'">
        <el-tree
          node-key="id"
          default-expand-all
          :data="tree"
          highlight-current
          :expand-on-click-node="false"
          @node-click="onNode"
        >
          <template #default="{ node, data }">
            <div class="tree-node">
              <span class="treeLabel" :class="{ isTop: node.level === 1 }">{{ node.label }}</span>
              <el-button
                v-if="data.append"
                type="primary"
                link
                :disabled="globalStart"
                @click.stop="addConnection"
              >
                <Icon :icon="plusIcon" />
              </el-button>
              <el-button
                v-else
                type="danger"
                link
                :disabled="globalStart"
                @click.stop="removeConnection(data.id)"
              >
                <Icon :icon="removeIcon" />
              </el-button>
            </div>
          </template>
        </el-tree>
      </el-scrollbar>
    </div>
    <div class="right">
      <div v-if="cfg" class="form">
        <el-form :model="cfg" label-width="150px" size="small" :disabled="globalStart">
          <el-divider content-position="left">{{ t('uds.xcp.sections.general') }}</el-divider>
          <el-form-item :label="t('uds.xcp.labels.name')" required>
            <el-input v-model="cfg.name" />
          </el-form-item>
          <el-form-item :label="t('uds.xcp.labels.transport')">
            <el-select v-model="cfg.transport">
              <el-option label="CAN" value="can" />
              <el-option label="UDP" value="udp" />
              <el-option label="TCP" value="tcp" />
            </el-select>
          </el-form-item>
          <el-form-item :label="t('uds.xcp.labels.timeout')">
            <el-input-number
              v-model="cfg.timeout"
              :min="1"
              :max="600000"
              controls-position="right"
            />
          </el-form-item>

          <template v-if="cfg.transport === 'can'">
            <el-divider content-position="left">{{ t('uds.xcp.sections.can') }}</el-divider>
            <el-form-item :label="t('uds.xcp.labels.device')" required>
              <el-select v-model="cfg.device" filterable allow-create default-first-option>
                <el-option v-for="name in canDevices" :key="name" :label="name" :value="name" />
              </el-select>
            </el-form-item>
            <el-form-item :label="t('uds.xcp.labels.canIdCmd')" required>
              <el-input :model-value="hex(cfg.canIdCmd)" @change="setHex('canIdCmd', $event)" />
            </el-form-item>
            <el-form-item :label="t('uds.xcp.labels.canIdResp')" required>
              <el-input :model-value="hex(cfg.canIdResp)" @change="setHex('canIdResp', $event)" />
            </el-form-item>
            <el-form-item label-width="0">
              <el-checkbox v-model="cfg.extended">{{ t('uds.xcp.labels.extended') }}</el-checkbox>
              <el-checkbox v-model="cfg.canfd">CAN-FD</el-checkbox>
              <el-checkbox v-model="cfg.brs" :disabled="!cfg.canfd">BRS</el-checkbox>
              <el-checkbox v-model="cfg.padding">{{ t('uds.xcp.labels.padding') }}</el-checkbox>
            </el-form-item>
            <el-form-item :label="t('uds.xcp.labels.paddingValue')">
              <el-input
                :model-value="hex(cfg.paddingValue)"
                :disabled="!cfg.padding"
                @change="setHex('paddingValue', $event)"
              />
            </el-form-item>
            <el-form-item label="DLC">
              <el-input-number v-model="cfg.dlc" :min="1" :max="64" controls-position="right" />
            </el-form-item>
          </template>

          <template v-else>
            <el-divider content-position="left">{{ t('uds.xcp.sections.ethernet') }}</el-divider>
            <el-form-item label="IP" required>
              <el-input v-model="cfg.ip" />
            </el-form-item>
            <el-form-item label="Port" required>
              <el-input-number v-model="cfg.port" :min="1" :max="65535" controls-position="right" />
            </el-form-item>
          </template>

          <el-divider content-position="left">{{ t('uds.xcp.sections.a2l') }}</el-divider>
          <el-form-item :label="t('uds.xcp.labels.a2l')">
            <el-input v-model="cfg.a2lFile" readonly>
              <template #append>
                <el-button @click="pickA2l">{{ t('uds.xcp.actions.browse') }}</el-button>
              </template>
            </el-input>
          </el-form-item>
          <el-table :data="cfg.a2lObjects" size="small" max-height="220" border>
            <el-table-column prop="name" :label="t('uds.xcp.columns.name')" min-width="140" />
            <el-table-column prop="kind" :label="t('uds.xcp.columns.kind')" width="120" />
            <el-table-column :label="t('uds.xcp.columns.address')" width="110">
              <template #default="{ row }">{{ hex(row.address) }}</template>
            </el-table-column>
            <el-table-column prop="extension" label="Ext" width="60" />
            <el-table-column prop="size" :label="t('uds.xcp.columns.size')" width="70" />
            <el-table-column prop="event" :label="t('uds.xcp.columns.event')" width="70" />
          </el-table>

          <el-divider content-position="left">{{ t('uds.xcp.sections.security') }}</el-divider>
          <el-form-item :label="t('uds.xcp.labels.seedKey')">
            <el-input v-model="cfg.seedKeyDll" readonly>
              <template #append>
                <el-button @click="pickDll">{{ t('uds.xcp.actions.browse') }}</el-button>
              </template>
            </el-input>
          </el-form-item>
          <el-form-item :label="t('uds.xcp.labels.resource')">
            <el-select v-model="resource">
              <el-option label="CAL/PAG" :value="1" />
              <el-option label="DAQ" :value="4" />
              <el-option label="STIM" :value="8" />
              <el-option label="PGM" :value="16" />
            </el-select>
            <el-button style="margin-left: 8px" :loading="busy" @click="unlock">
              {{ t('uds.xcp.actions.unlock') }}
            </el-button>
          </el-form-item>

          <el-divider content-position="left">{{ t('uds.xcp.sections.daq') }}</el-divider>
          <el-form-item :label="t('uds.xcp.labels.daqEvent')">
            <el-select v-model="cfg.daqEvent" clearable filterable>
              <el-option
                v-for="ev in cfg.daqEvents"
                :key="ev.channel"
                :label="`${ev.channel}: ${ev.name}`"
                :value="ev.channel"
              />
            </el-select>
          </el-form-item>
          <el-form-item>
            <el-button type="primary" :loading="busy" @click="probe">
              {{ t('uds.xcp.actions.connect') }}
            </el-button>
            <el-button :loading="busy" @click="probe">{{
              t('uds.xcp.actions.readEvents')
            }}</el-button>
          </el-form-item>
        </el-form>
        <el-alert v-if="errors.length" type="error" :closable="false" :title="errors.join('; ')" />
        <el-alert v-if="resultText" type="success" :closable="false">
          <pre class="probe-detail">{{ resultText }}</pre>
        </el-alert>
      </div>
      <el-empty v-else :description="t('uds.xcp.empty')" />
    </div>
  </div>
</template>

<script lang="ts" setup>
import { computed, ref, toRef, watch } from 'vue'
import { Icon } from '@iconify/vue'
import plusIcon from '@iconify/icons-ep/circle-plus-filled'
import removeIcon from '@iconify/icons-ep/remove'
import { v4 } from 'uuid'
import { cloneDeep } from 'lodash'
import { ElMessage } from 'element-plus'
import i18next from 'i18next'
import { useDataStore } from '@r/stores/data'
import { useGlobalStart } from '@r/stores/runtime'
import { parseA2lEvents, parseA2lObjects } from 'nodeCan/a2l'
import {
  defaultXcpConnection,
  mergeDaqEvents,
  validateXcpConnection,
  type XcpDaqEvent
} from 'nodeCan/xcpConfig'

const props = defineProps<{ height: number; width: number }>()
const h = toRef(props, 'height')
const w = toRef(props, 'width')
const leftWidth = 220
const t = i18next.t.bind(i18next)
const db = useDataStore()
const globalStart = useGlobalStart()
if (!db.xcp) db.xcp = {}

const activeId = ref('')
const busy = ref(false)
const resultText = ref('')
const resource = ref(1)

const tree = computed(() => [
  {
    id: 'root',
    label: t('uds.xcp.root'),
    append: true,
    children: Object.values(db.xcp ?? {}).map((item) => ({
      id: item.id,
      label: item.name,
      append: false
    }))
  }
])

watch(
  () => Object.keys(db.xcp ?? {}),
  (ids) => {
    if (!activeId.value || !(db.xcp ?? {})[activeId.value]) {
      activeId.value = ids[0] ?? ''
    }
  },
  { immediate: true }
)

const cfg = computed(() => (db.xcp ?? {})[activeId.value])

const canDevices = computed(() => {
  const names: string[] = []
  for (const device of Object.values(db.devices)) {
    if (device.type === 'can' && device.canDevice?.name) names.push(device.canDevice.name)
  }
  return names
})

const errors = computed(() => (cfg.value ? validateXcpConnection(cfg.value) : []))

function onNode(data: { id: string; append?: boolean }) {
  if (data.append) return
  activeId.value = data.id
  resultText.value = ''
}

function addConnection() {
  const names = new Set(Object.values(db.xcp ?? {}).map((item) => item.name))
  let n = Object.keys(db.xcp ?? {}).length + 1
  let name = `XCP${n}`
  while (names.has(name)) {
    n += 1
    name = `XCP${n}`
  }
  const id = v4()
  db.xcp![id] = defaultXcpConnection({ id, name })
  activeId.value = id
}

function removeConnection(id: string) {
  delete db.xcp![id]
  if (activeId.value === id) activeId.value = ''
}

function hex(value: number): string {
  return '0x' + (value >>> 0).toString(16)
}

function setHex(key: 'canIdCmd' | 'canIdResp' | 'paddingValue', text: string) {
  if (!cfg.value) return
  const raw = String(text).trim()
  const value = raw.startsWith('0x') || raw.startsWith('0X') ? parseInt(raw, 16) : parseInt(raw, 10)
  if (!Number.isInteger(value) || value < 0) return
  cfg.value[key] = value
}

async function pickFile(extensions: string[]): Promise<string | null> {
  const picked = await window.electron.ipcRenderer.invoke('ipc-show-open-dialog', {
    title: t('uds.xcp.actions.browse'),
    properties: ['openFile'],
    filters: [{ name: extensions[0], extensions }]
  })
  return picked.filePaths?.[0] ?? null
}

async function pickA2l() {
  if (!cfg.value) return
  const file = await pickFile(['a2l'])
  if (!file) return
  const text = await window.electron.ipcRenderer.invoke('ipc-fs-readFile', file, 'utf-8')
  cfg.value.a2lFile = file
  cfg.value.a2lObjects = parseA2lObjects(text)
  const fromFile: XcpDaqEvent[] = parseA2lEvents(text).map((event) => ({
    channel: event.channel,
    name: event.name,
    source: 'a2l'
  }))
  const fromObjects: XcpDaqEvent[] = cfg.value.a2lObjects
    .filter((item) => item.event != null)
    .map((item) => ({ channel: item.event as number, name: `event ${item.event}`, source: 'a2l' }))
  cfg.value.daqEvents = mergeDaqEvents(
    cfg.value.daqEvents.filter((e) => e.source === 'slave'),
    [...fromFile, ...fromObjects]
  )
}

async function pickDll() {
  if (!cfg.value) return
  const file = await pickFile(['dll'])
  if (file) cfg.value.seedKeyDll = file
}

function formatProbe(payload: {
  connect: {
    maxCto: number
    maxDto: number
    protocolLayerVersion: number
    transportLayerVersion: number
    resource: Record<string, boolean>
  }
  status: { sessionStatus: { daqRunning: boolean } }
  events: XcpDaqEvent[]
}): string {
  const version = (value: number) => `${(value >> 4) & 0xf}.${value & 0xf}`
  const resources = Object.entries(payload.connect.resource)
    .filter(([, on]) => on)
    .map(([name]) => name)
    .join(', ')
  const events = payload.events.map((event) => `${event.channel} ${event.name}`).join(', ')
  return [
    t('uds.xcp.connected'),
    `MAX_CTO ${payload.connect.maxCto}  MAX_DTO ${payload.connect.maxDto}`,
    `protocol ${version(payload.connect.protocolLayerVersion)}  transport ${version(payload.connect.transportLayerVersion)}`,
    `resources ${resources || '-'}`,
    `daqRunning ${payload.status.sessionStatus.daqRunning}`,
    events ? `events ${events}` : ''
  ]
    .filter(Boolean)
    .join('\n')
}

async function probe() {
  if (!cfg.value) return
  if (errors.value.length) {
    ElMessage.error(errors.value[0])
    return
  }
  busy.value = true
  resultText.value = ''
  try {
    const payload = await window.electron.ipcRenderer.invoke('ipc-xcp-probe', cloneDeep(cfg.value))
    cfg.value.daqEvents = mergeDaqEvents(
      cfg.value.daqEvents.filter((event) => event.source === 'a2l'),
      payload.events
    )
    if (cfg.value.daqEvent == null && payload.events.length) {
      cfg.value.daqEvent = payload.events[0].channel
    }
    resultText.value = formatProbe(payload)
  } catch (err) {
    ElMessage.error(err instanceof Error ? err.message : String(err))
  } finally {
    busy.value = false
  }
}

async function unlock() {
  if (!cfg.value) return
  if (!cfg.value.seedKeyDll) {
    ElMessage.error(t('uds.xcp.errors.noDll'))
    return
  }
  busy.value = true
  try {
    const flags = await window.electron.ipcRenderer.invoke(
      'ipc-xcp-unlock',
      cloneDeep(cfg.value),
      resource.value
    )
    resultText.value = `UNLOCK ${JSON.stringify(flags)}`
  } catch (err) {
    ElMessage.error(err instanceof Error ? err.message : String(err))
  } finally {
    busy.value = false
  }
}
</script>

<style scoped>
.main {
  position: relative;
  height: v-bind(h + 'px');
  width: v-bind(w + 'px');
}
.left {
  position: absolute;
  left: 0;
  width: v-bind(leftWidth + 'px');
  top: 0;
  bottom: 0;
}
.right {
  position: absolute;
  left: v-bind(leftWidth + 8 + 'px');
  right: 0;
  top: 0;
  bottom: 0;
  overflow: auto;
}
.form {
  padding: 8px 12px 16px 0;
}
.tree-node {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 12px;
  padding-right: 4px;
}
.treeLabel {
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
}
.isTop {
  font-weight: bold;
}
.probe-detail {
  margin: 0;
  white-space: pre-wrap;
  font-family: inherit;
  line-height: 1.5;
}
</style>
