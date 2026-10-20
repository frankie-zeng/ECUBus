<template>
  <div style="padding: 20px; min-width: 600px">
    <el-form
      ref="ruleFormRef"
      :model="data"
      label-width="150px"
      :rules="rules"
      size="small"
      hide-required-asterisk
      :disabled="globalStart"
    >
      <el-divider content-position="left">{{ i18next.t('uds.xcp.sections.general') }}</el-divider>
      <el-form-item :label="i18next.t('uds.xcp.labels.name')" prop="name">
        <el-input v-model="data.name" />
      </el-form-item>
      <el-form-item :label="i18next.t('uds.xcp.labels.transport')">
        <el-input :model-value="data.transport.toUpperCase()" disabled />
      </el-form-item>
      <el-form-item :label="i18next.t('uds.xcp.labels.timeout')" prop="timeout">
        <el-input-number v-model="data.timeout" :min="1" :max="600000" controls-position="right" />
      </el-form-item>

      <template v-if="data.transport === 'can'">
        <el-divider content-position="left">{{ i18next.t('uds.xcp.sections.can') }}</el-divider>
        <el-form-item :label="i18next.t('uds.xcp.labels.device')" prop="device">
          <el-select
            v-model="data.device"
            clearable
            :placeholder="i18next.t('uds.xcp.placeholders.selectDevice')"
          >
            <el-option
              v-for="item in canDevices"
              :key="item.id"
              :label="item.name"
              :value="item.name"
            >
              <span style="float: left">{{ item.name }}</span>
              <span style="float: right; color: var(--el-text-color-secondary); font-size: 13px">
                {{ item.vendor.toUpperCase() }}
              </span>
            </el-option>
          </el-select>
        </el-form-item>
        <el-form-item label-width="0">
          <el-col :span="12">
            <el-form-item :label="i18next.t('uds.xcp.labels.canIdCmd')" prop="canIdCmd">
              <el-input :model-value="hex(data.canIdCmd)" @change="setHex('canIdCmd', $event)" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="i18next.t('uds.xcp.labels.canIdResp')" prop="canIdResp">
              <el-input :model-value="hex(data.canIdResp)" @change="setHex('canIdResp', $event)" />
            </el-form-item>
          </el-col>
        </el-form-item>
        <el-form-item label-width="0">
          <el-col :span="6">
            <el-form-item :label="i18next.t('uds.xcp.labels.extended')" prop="extended">
              <el-checkbox v-model="data.extended" />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="CAN-FD" prop="canfd">
              <el-checkbox v-model="data.canfd" />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item label="BRS" prop="brs">
              <el-checkbox v-model="data.brs" :disabled="!data.canfd" />
            </el-form-item>
          </el-col>
          <el-col :span="6">
            <el-form-item :label="i18next.t('uds.xcp.labels.padding')" prop="padding">
              <el-checkbox v-model="data.padding" />
            </el-form-item>
          </el-col>
        </el-form-item>
        <el-form-item label-width="0">
          <el-col :span="12">
            <el-form-item :label="i18next.t('uds.xcp.labels.paddingValue')" prop="paddingValue">
              <el-input
                :model-value="hex(data.paddingValue)"
                :disabled="!data.padding"
                @change="setHex('paddingValue', $event)"
              />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="DLC" prop="dlc">
              <el-input-number v-model="data.dlc" :min="1" :max="64" controls-position="right" />
            </el-form-item>
          </el-col>
        </el-form-item>
      </template>

      <template v-else>
        <el-divider content-position="left">{{
          i18next.t('uds.xcp.sections.ethernet')
        }}</el-divider>
        <el-form-item label-width="0">
          <el-col :span="12">
            <el-form-item label="IP" prop="ip">
              <el-input v-model="data.ip" />
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item label="Port" prop="port">
              <el-input-number
                v-model="data.port"
                :min="1"
                :max="65535"
                controls-position="right"
              />
            </el-form-item>
          </el-col>
        </el-form-item>
      </template>

      <el-divider content-position="left">{{ i18next.t('uds.xcp.sections.a2l') }}</el-divider>
      <el-form-item :label="i18next.t('uds.xcp.labels.a2l')">
        <el-input v-model="data.a2lFile" clearable />
        <div class="lr">
          <el-button size="small" plain @click="pickA2l">
            <Icon :icon="folderIcon" class="icon" />
            {{ i18next.t('uds.xcp.actions.browse') }}
          </el-button>
        </div>
      </el-form-item>
      <el-table :data="data.a2lObjects" size="small" max-height="220" border>
        <el-table-column prop="name" :label="i18next.t('uds.xcp.columns.name')" min-width="140" />
        <el-table-column prop="kind" :label="i18next.t('uds.xcp.columns.kind')" width="120" />
        <el-table-column :label="i18next.t('uds.xcp.columns.address')" width="110">
          <template #default="{ row }">{{ hex(row.address) }}</template>
        </el-table-column>
        <el-table-column prop="extension" label="Ext" width="60" />
        <el-table-column prop="size" :label="i18next.t('uds.xcp.columns.size')" width="70" />
        <el-table-column prop="event" :label="i18next.t('uds.xcp.columns.event')" width="70" />
      </el-table>

      <el-divider content-position="left">{{ i18next.t('uds.xcp.sections.security') }}</el-divider>
      <el-form-item :label="i18next.t('uds.xcp.labels.seedKey')">
        <el-input v-model="data.seedKeyDll" clearable />
        <div class="lr">
          <el-button size="small" plain @click="pickDll">
            <Icon :icon="folderIcon" class="icon" />
            {{ i18next.t('uds.xcp.actions.browse') }}
          </el-button>
        </div>
      </el-form-item>
      <el-form-item :label="i18next.t('uds.xcp.labels.resource')">
        <el-select v-model="resource">
          <el-option label="CAL/PAG" :value="1" />
          <el-option label="DAQ" :value="4" />
          <el-option label="STIM" :value="8" />
          <el-option label="PGM" :value="16" />
        </el-select>
      </el-form-item>

      <el-divider content-position="left">{{ i18next.t('uds.xcp.sections.daq') }}</el-divider>
      <el-form-item :label="i18next.t('uds.xcp.labels.daqEvent')" prop="daqEvent">
        <el-select v-model="data.daqEvent" clearable filterable>
          <el-option
            v-for="ev in data.daqEvents"
            :key="`${ev.source}-${ev.channel}`"
            :label="`${ev.channel}: ${ev.name}`"
            :value="ev.channel"
          />
        </el-select>
      </el-form-item>
    </el-form>

    <div class="actions">
      <el-button type="primary" size="small" :loading="busy" @click="probe">
        {{ i18next.t('uds.xcp.actions.connect') }}
      </el-button>
      <el-button size="small" :loading="busy" @click="probe">
        {{ i18next.t('uds.xcp.actions.readEvents') }}
      </el-button>
      <el-button size="small" :loading="busy" @click="unlock">
        {{ i18next.t('uds.xcp.actions.unlock') }}
      </el-button>
    </div>
    <el-alert v-if="resultText" type="success" :closable="false">
      <pre class="probe-detail">{{ resultText }}</pre>
    </el-alert>
  </div>
</template>

<script lang="ts" setup>
import { computed, onBeforeMount, onUnmounted, ref, watch } from 'vue'
import type { FormInstance, FormRules } from 'element-plus'
import { ElMessage } from 'element-plus'
import { Icon } from '@iconify/vue'
import folderIcon from '@iconify/icons-ep/folder-opened'
import { cloneDeep } from 'lodash'
import i18next from 'i18next'
import { useDataStore } from '@r/stores/data'
import { useGlobalStart } from '@r/stores/runtime'
import { parseA2lEvents, parseA2lObjects } from 'nodeCan/a2l'
import {
  defaultXcpConnection,
  mergeDaqEvents,
  validateXcpConnection,
  type XcpConnectionConfig,
  type XcpDaqEvent
} from 'nodeCan/xcpConfig'

const props = defineProps<{ index: string; height: number }>()
const emit = defineEmits<{ change: [id: string, name: string] }>()

const db = useDataStore()
const globalStart = useGlobalStart()
const ruleFormRef = ref<FormInstance>()
const data = ref<XcpConnectionConfig>(defaultXcpConnection({ id: props.index, name: '' }))
const resource = ref(1)
const busy = ref(false)
const resultText = ref('')
let stopWatch: (() => void) | undefined

const canDevices = computed(() => {
  const list: { id: string; name: string; vendor: string }[] = []
  for (const device of Object.values(db.devices)) {
    if (device.type === 'can' && device.canDevice?.name) {
      list.push({
        id: device.canDevice.id,
        name: device.canDevice.name,
        vendor: device.canDevice.vendor
      })
    }
  }
  return list
})

function hex(value: number): string {
  return '0x' + (value >>> 0).toString(16)
}

function setHex(key: 'canIdCmd' | 'canIdResp' | 'paddingValue', text: string) {
  const raw = String(text).trim()
  const value = raw.startsWith('0x') || raw.startsWith('0X') ? parseInt(raw, 16) : parseInt(raw, 10)
  if (!Number.isInteger(value) || value < 0) return
  data.value[key] = value
}

const rules = computed<FormRules>(() => ({
  name: [{ required: true, trigger: 'blur', validator: nameCheck }],
  timeout: [{ required: true, trigger: 'change', validator: timeoutCheck }],
  device: [
    {
      required: data.value.transport === 'can',
      trigger: 'change',
      validator: deviceCheck
    }
  ],
  canIdCmd: [{ trigger: 'change', validator: idCheck }],
  canIdResp: [{ trigger: 'change', validator: idCheck }],
  paddingValue: [{ trigger: 'change', validator: paddingCheck }],
  dlc: [{ trigger: 'change', validator: dlcCheck }],
  brs: [{ trigger: 'change', validator: brsCheck }],
  ip: [{ trigger: 'blur', validator: ipCheck }],
  port: [{ trigger: 'change', validator: portCheck }],
  daqEvent: [{ trigger: 'change', validator: daqCheck }]
}))

function nameCheck(_rule: unknown, value: string, callback: (err?: Error) => void) {
  if (!value || !value.trim()) {
    callback(new Error(i18next.t('uds.xcp.validation.nameRequired')))
    return
  }
  const dup = Object.values(db.xcp ?? {}).some(
    (item) => item.name === value && item.id !== data.value.id
  )
  if (dup) callback(new Error(i18next.t('uds.xcp.validation.nameExists')))
  else callback()
}

function timeoutCheck(_rule: unknown, value: number, callback: (err?: Error) => void) {
  if (!Number.isInteger(value) || value < 1 || value > 600000) {
    callback(new Error(i18next.t('uds.xcp.validation.timeout')))
  } else callback()
}

function deviceCheck(_rule: unknown, value: string, callback: (err?: Error) => void) {
  if (data.value.transport === 'can' && (!value || !value.trim())) {
    callback(new Error(i18next.t('uds.xcp.validation.deviceRequired')))
  } else callback()
}

function idCheck(_rule: { field?: string }, value: number, callback: (err?: Error) => void) {
  if (data.value.transport !== 'can') {
    callback()
    return
  }
  const maxId = data.value.extended ? 0x1fffffff : 0x7ff
  if (!Number.isInteger(value) || value < 0 || value > maxId) {
    callback(
      new Error(i18next.t('uds.xcp.validation.canIdRange', { max: '0x' + maxId.toString(16) }))
    )
    return
  }
  if (data.value.canIdCmd === data.value.canIdResp) {
    callback(new Error(i18next.t('uds.xcp.validation.canIdDiffer')))
    return
  }
  callback()
}

function paddingCheck(_rule: unknown, value: number, callback: (err?: Error) => void) {
  if (data.value.transport !== 'can') {
    callback()
    return
  }
  if (!Number.isInteger(value) || value < 0 || value > 255) {
    callback(new Error(i18next.t('uds.xcp.validation.paddingValue')))
  } else callback()
}

function dlcCheck(_rule: unknown, value: number, callback: (err?: Error) => void) {
  if (data.value.transport !== 'can') {
    callback()
    return
  }
  if (!Number.isInteger(value) || value < 1 || value > 64) {
    callback(new Error(i18next.t('uds.xcp.validation.dlc')))
  } else callback()
}

function brsCheck(_rule: unknown, _value: boolean, callback: (err?: Error) => void) {
  if (data.value.brs && !data.value.canfd) callback(new Error(i18next.t('uds.xcp.validation.brs')))
  else callback()
}

function ipCheck(_rule: unknown, value: string, callback: (err?: Error) => void) {
  if (data.value.transport === 'can') {
    callback()
    return
  }
  if (!value || !/^[A-Za-z0-9._:-]+$/.test(value)) {
    callback(new Error(i18next.t('uds.xcp.validation.ip')))
  } else callback()
}

function portCheck(_rule: unknown, value: number, callback: (err?: Error) => void) {
  if (data.value.transport === 'can') {
    callback()
    return
  }
  if (!Number.isInteger(value) || value < 1 || value > 65535) {
    callback(new Error(i18next.t('uds.xcp.validation.port')))
  } else callback()
}

function daqCheck(_rule: unknown, value: number | undefined, callback: (err?: Error) => void) {
  if (value != null && (!Number.isInteger(value) || value < 0)) {
    callback(new Error(i18next.t('uds.xcp.validation.daqEvent')))
  } else callback()
}

function save() {
  const errors = validateXcpConnection(data.value)
  const duplicate = Object.values(db.xcp ?? {}).some(
    (item) => item.name === data.value.name && item.id !== data.value.id
  )
  if (errors.length || duplicate || !db.xcp?.[data.value.id]) return
  db.xcp[data.value.id] = cloneDeep(data.value)
  emit('change', data.value.id, data.value.name)
}

async function pickFile(extensions: string[], title: string): Promise<string | null> {
  const picked = await window.electron.ipcRenderer.invoke('ipc-show-open-dialog', {
    title,
    properties: ['openFile'],
    filters: [
      { name: title, extensions },
      { name: i18next.t('uds.xcp.dialogs.allFiles'), extensions: ['*'] }
    ]
  })
  return picked.filePaths?.[0] ?? null
}

async function pickA2l() {
  const file = await pickFile(['a2l'], i18next.t('uds.xcp.dialogs.a2lFile'))
  if (!file) return
  const text = await window.electron.ipcRenderer.invoke('ipc-fs-readFile', file, 'utf-8')
  data.value.a2lFile = file
  data.value.a2lObjects = parseA2lObjects(text)
  const fromFile: XcpDaqEvent[] = parseA2lEvents(text).map((event) => ({
    channel: event.channel,
    name: event.name,
    source: 'a2l'
  }))
  const fromObjects: XcpDaqEvent[] = data.value.a2lObjects
    .filter((item) => item.event != null)
    .map((item) => ({ channel: item.event as number, name: `event ${item.event}`, source: 'a2l' }))
  data.value.daqEvents = mergeDaqEvents(
    data.value.daqEvents.filter((event) => event.source === 'slave'),
    [...fromFile, ...fromObjects]
  )
}

async function pickDll() {
  const file = await pickFile(['dll'], i18next.t('uds.xcp.dialogs.dllFile'))
  if (file) data.value.seedKeyDll = file
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
    i18next.t('uds.xcp.connected'),
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
  const errors = validateXcpConnection(data.value)
  if (errors.length) {
    ElMessage.error(errors[0])
    return
  }
  busy.value = true
  resultText.value = ''
  try {
    const payload = await window.electron.ipcRenderer.invoke('ipc-xcp-probe', cloneDeep(data.value))
    data.value.daqEvents = mergeDaqEvents(
      data.value.daqEvents.filter((event) => event.source === 'a2l'),
      payload.events
    )
    if (data.value.daqEvent == null && payload.events.length) {
      data.value.daqEvent = payload.events[0].channel
    }
    resultText.value = formatProbe(payload)
  } catch (err) {
    ElMessage.error(err instanceof Error ? err.message : String(err))
  } finally {
    busy.value = false
  }
}

async function unlock() {
  if (!data.value.seedKeyDll) {
    ElMessage.error(i18next.t('uds.xcp.errors.noDll'))
    return
  }
  const errors = validateXcpConnection(data.value)
  if (errors.length) {
    ElMessage.error(errors[0])
    return
  }
  busy.value = true
  try {
    const flags = await window.electron.ipcRenderer.invoke(
      'ipc-xcp-unlock',
      cloneDeep(data.value),
      resource.value
    )
    resultText.value = `UNLOCK ${JSON.stringify(flags)}`
  } catch (err) {
    ElMessage.error(err instanceof Error ? err.message : String(err))
  } finally {
    busy.value = false
  }
}

onBeforeMount(() => {
  const saved = db.xcp?.[props.index]
  if (saved) data.value = cloneDeep(saved)
  if (data.value.transport === 'can' && data.value.device) {
    const known = canDevices.value.some((item) => item.name === data.value.device)
    if (!known) data.value.device = ''
  }
  stopWatch = watch(data, () => save(), { deep: true })
})

onUnmounted(() => {
  stopWatch?.()
})
</script>

<style scoped>
.lr {
  margin-top: 5px;
}
.icon {
  margin-right: 5px;
}
.actions {
  display: flex;
  gap: 8px;
  margin: 4px 0 12px;
}
.probe-detail {
  margin: 0;
  white-space: pre-wrap;
  font-family: inherit;
  line-height: 1.5;
}
</style>
