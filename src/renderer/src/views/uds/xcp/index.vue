<template>
  <div class="main">
    <div class="left">
      <el-scrollbar :height="h + 'px'">
        <el-tree
          ref="treeRef"
          node-key="id"
          default-expand-all
          :data="tData"
          highlight-current
          :expand-on-click-node="false"
          @node-click="nodeClick"
        >
          <template #default="{ node, data }">
            <div class="tree-node">
              <span
                :class="{
                  isTop: node.level === 1,
                  treeLabel: true
                }"
                >{{ node.label }}
              </span>
              <el-button
                v-if="data.append"
                type="primary"
                link
                :disabled="globalStart"
                @click.stop="addConnection(data)"
              >
                <Icon :icon="circlePlusFilled" />
              </el-button>
              <el-button
                v-else-if="node.parent?.data.append"
                type="danger"
                link
                :disabled="globalStart"
                @click.stop="removeConnection(data)"
              >
                <Icon :icon="removeIcon" />
              </el-button>
            </div>
          </template>
        </el-tree>
      </el-scrollbar>
    </div>
    <div :id="`${winKey}Shift`" class="shift" />
    <div class="right">
      <xcpConfig v-if="activeTree" :index="activeTree.id" :height="h" @change="nodeChange" />
    </div>
  </div>
</template>

<script lang="ts" setup>
import { nextTick, onMounted, ref, toRef, watch } from 'vue'
import { Icon } from '@iconify/vue'
import { ElMessageBox } from 'element-plus'
import circlePlusFilled from '@iconify/icons-ep/circle-plus-filled'
import removeIcon from '@iconify/icons-ep/remove'
import { v4 } from 'uuid'
import { useDataStore } from '@r/stores/data'
import { useGlobalStart } from '@r/stores/runtime'
import { defaultXcpConnection, type XcpTransportKind } from 'nodeCan/xcpConfig'
import i18next from 'i18next'
import xcpConfig from './config.vue'

const props = defineProps<{ height: number; width: number; editIndex?: string }>()
const h = toRef(props, 'height')
const w = toRef(props, 'width')
const winKey = 'xcp'
const leftWidth = ref(200)
const treeRef = ref()
const db = useDataStore()
const globalStart = useGlobalStart()
if (!db.xcp) db.xcp = {}

interface TreeNode {
  label: string
  type: XcpTransportKind
  append: boolean
  id: string
  children?: TreeNode[]
}

const tData = ref<TreeNode[]>([])
const activeTree = ref<TreeNode>()

function nodeClick(data: TreeNode, node: { parent?: { data?: { append?: boolean } } }) {
  if (activeTree.value?.id === data.id) return
  activeTree.value = undefined
  nextTick(() => {
    if (node.parent?.data?.append === true) activeTree.value = data
  })
}

function nodeChange(id: string, name: string) {
  const node = treeRef.value?.getNode(id)
  if (node) node.data.label = name
}

function removeConnection(data: TreeNode) {
  ElMessageBox.confirm(
    i18next.t('uds.xcp.dialogs.deleteConfirm'),
    i18next.t('uds.xcp.dialogs.warning'),
    {
      confirmButtonText: i18next.t('uds.xcp.dialogs.ok'),
      cancelButtonText: i18next.t('uds.xcp.dialogs.cancel'),
      type: 'warning',
      buttonSize: 'small',
      appendTo: `#win${winKey}`
    }
  )
    .then(() => {
      delete db.xcp![data.id]
      treeRef.value?.remove(data.id)
      if (activeTree.value?.id === data.id) activeTree.value = undefined
    })
    .catch(() => {
      null
    })
}

function uniqueName(type: XcpTransportKind): string {
  let index = 0
  let name = `XCP_${type}_${index}`
  while (Object.values(db.xcp ?? {}).some((item) => item.name === name)) {
    index += 1
    name = `XCP_${type}_${index}`
  }
  return name
}

function addConnection(node: TreeNode) {
  const id = v4()
  const name = uniqueName(node.type)
  db.xcp![id] = defaultXcpConnection({ id, name, transport: node.type })
  treeRef.value?.append(
    {
      label: name,
      append: false,
      id,
      type: node.type
    },
    node.id
  )
  nextTick(() => {
    activeTree.value = treeRef.value?.getNode(id)?.data
    treeRef.value?.setCurrentKey(id)
  })
}

function buildTree() {
  const groups: XcpTransportKind[] = ['can', 'udp', 'tcp']
  const nodes: TreeNode[] = groups.map((type) => ({
    label: i18next.t(`uds.xcp.tree.${type}`),
    type,
    append: true,
    id: type,
    children: []
  }))
  for (const item of Object.values(db.xcp ?? {})) {
    const group = nodes.find((node) => node.type === item.transport)
    group?.children?.push({
      label: item.name,
      type: item.transport,
      append: false,
      id: item.id
    })
  }
  tData.value = nodes
}

function selectFirst() {
  for (const group of tData.value) {
    const first = group.children?.[0]
    if (!first) continue
    treeRef.value?.setCurrentKey(first.id)
    activeTree.value = first
    return
  }
}

function selectId(id?: string) {
  if (!id) return false
  const node = treeRef.value?.getNode(id)
  if (!node) return false
  treeRef.value.setCurrentKey(id)
  activeTree.value = node.data
  return true
}

onMounted(() => {
  window.jQuery(`#${winKey}Shift`).resizable({
    handles: 'e',
    resize: (_e: unknown, ui: { size: { width: number } }) => {
      leftWidth.value = ui.size.width
    },
    maxWidth: 400,
    minWidth: 150
  })
  buildTree()
  nextTick(() => {
    if (!selectId(props.editIndex)) selectFirst()
  })
})

watch(
  () => props.editIndex,
  (id) => {
    nextTick(() => selectId(id))
  }
)

watch(
  () =>
    Object.values(db.xcp ?? {})
      .map((item) => `${item.id}:${item.name}:${item.transport}`)
      .join('|'),
  () => {
    const current = activeTree.value?.id
    buildTree()
    nextTick(() => {
      if (!selectId(current)) selectId(props.editIndex)
    })
  }
)
</script>

<style scoped>
.isTop {
  font-weight: bold;
}

.left {
  position: absolute;
  top: 0px;
  left: 0px;
  width: v-bind(leftWidth + 'px');
  z-index: 1;
}

.shift {
  position: absolute;
  top: 0px;
  left: 0px;
  width: v-bind(leftWidth + 1 + 'px');
  height: v-bind(h + 'px');
  z-index: 0;
  border-right: solid 1px var(--el-border-color);
}

.shift:hover {
  border-right: solid 4px var(--el-color-primary);
}

.shift:active {
  border-right: solid 4px var(--el-color-primary);
}

.tree-node {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 12px;
  padding-right: 5px;
}

.right {
  position: absolute;
  left: v-bind(leftWidth + 5 + 'px');
  width: v-bind(w - leftWidth - 6 + 'px');
  height: v-bind(h + 'px');
  z-index: 0;
  overflow: auto;
}

.main {
  position: relative;
  height: v-bind(h + 'px');
  width: v-bind(w + 'px');
}

.treeLabel {
  display: inline-block;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  width: v-bind(leftWidth - 80 + 'px') !important;
}
</style>
