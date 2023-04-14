<template>
  <el-dialog
    v-model="dialogVisible"
    title="Create New UDS Project"
    width="80%"
    destroy-on-close
  >
    <div :style="{ height: Math.round(winH * 2 / 3) - 100 + 'px', overflow: 'auto', marginRight:'30px',marginLeft:'30px'}">
      <el-form
        :model="udsData"
        label-width="120px"
        size="small"
      >
        <el-form-item
          label="Vendor"
          required
          prop="vendor"
        >
          <el-select
            v-model="udsData.vendor"
            placeholder="PEAK?"
            style="width: 100%;"
          >
            <el-option
              label="PEAK"
              value="PEAK"
            />
            <el-option
              label="KVASER"
              value="KVASER"
              disabled
            />
            <el-option
              label="ZLG"
              value="ZLG"
              disabled
            />
          </el-select>
        </el-form-item>
        <el-form-item
          label="Type"
          required
          prop="type"
        >
          <el-select
            v-model="udsData.type"
            placeholder="TYPE"
            style="width: 100%;"
          >
            <el-option
              label="CAN Networks"
              value="CAN"
            />
            <el-option
              label="LIN Networks"
              value="LIN"
            />
            <el-option
              label="ETH Networks"
              value="ETH"
            />
            <el-option
              label="Loopback Simulation"
              value="LP"
            />
          </el-select>
        </el-form-item>
        <el-divider content-position="left">
          Device Config
        </el-divider>
        <el-form-item
          :label="'Device-'+udsData.vendor"
          required
          prop="device"
        >
          <el-select v-model="udsData.device">
            <el-option
              v-for="device in deviceList"
              :key="device.value"
              :label="device.label"
              :value="device.value"
            />
          </el-select>
          <el-button
            text
            type="success"
            :icon="Refresh"
          />
        </el-form-item>
        <el-divider content-position="left">
          CAN Transport Layer Config
        </el-divider>
      </el-form>
    </div>
    <template #footer>
      <span class="dialog-footer">
        <el-button @click="dialogVisible = false">Cancel</el-button>
        <el-button
          type="primary"
          @click="dialogVisible = false"
        >
          Create
        </el-button>
      </span>
    </template>
  </el-dialog>
</template>
  
<script lang="ts" setup>
import { ref, inject, Ref } from 'vue'
import { useUdsStore } from '@/stores/uds';
import {Refresh } from '@element-plus/icons-vue'


const winH = inject('winH') as Ref<number>
const dialogVisible = ref(false)
const udsData = useUdsStore()

interface Device {
    
    label: string,
    value: string
}

const deviceList=ref<Device[]>([])

function open() {
    dialogVisible.value = true
}

defineExpose({
    open
})

</script>
<style scoped>
.dialog-footer button:first-child {
    margin-right: 10px;
}
</style>