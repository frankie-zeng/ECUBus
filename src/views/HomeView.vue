<template>
  <div>
    <div>
      <el-header class="header">
        <div class="logo-container">
          <img
            :src="Logo"
            alt="Logo"
            class="logo"
          >
        </div>
        <div class="github-container">
          <el-link
            href="https://github.com/frankie-zeng/ECUBus"
            target="_blank"
            :icon="Link"
          >
            GitHub
          </el-link>
          <el-dropdown trigger="click">
            <el-button
              size="small"
              text
              :icon="ChatDotRound"
            />
            <template #dropdown>
              <el-dropdown-menu>
                <el-dropdown-item>
                  <img
                    :src="Group"
                    alt="QR Code"
                    class="qrcode"
                  >
                </el-dropdown-item>
              </el-dropdown-menu>
            </template>
          </el-dropdown>
        </div>
      </el-header>
    </div>
    <div>
      <el-card>
        <div
          style="text-align: center;"
          class="card"
        >
          <img
            :src="UDSIMG"
            class="image"
          >
          <span>
            <div style="width: 200px;text-align: left;">
              <h4>Support Devices:</h4>
              <el-divider />
              <div style="margin-top: 5px;">
                <el-button
                  @click="activeDevice='ZLG'"
                  disabled
                  plain
                  type="primary"
                  style="width: 100%;display: block;"
                >ZLG (TODO)</el-button>
              </div>
              <div style="margin-top: 5px;"> 
                <el-button
                  @click="activeDevice='PEAK'"
                  plain
                  type="primary"
                  style="width: 100%;display: block;"
                >PEAK</el-button>
              </div>
              <div style="margin-top: 5px;">
                <el-button
                  @click="activeDevice='KVASER'"
                  plain
                  type="primary"
                  style="width: 100%;display: block;"
                >KVASER (TODO)</el-button>
              </div>
            </div>
          </span>
          <div>
            <el-image
              v-if="activeDevice=='PEAK'"
              style="width: 400px; height:250px"
              src="https://cdn.shopifycdn.net/s/files/1/0076/2449/1123/products/4295e0fb6d9cb4922144c9d3352f256b_444x256.jpg?v=1536869030"
              fit="fill"
            />
            <el-image
              v-if="activeDevice=='ZLG'"
              style="width: 400px; height:250px"
              src="https://cdn.shopifycdn.net/s/files/1/0076/2449/1123/products/4295e0fb6d9cb4922144c9d3352f256b_444x256.jpg?v=1536869030"
              fit="fill"
            />
            <el-image
              v-if="activeDevice=='KVASER'"
              style="width: 400px; height:250px"
              src="https://www.kvaser.cn/wp-content/uploads/product/73-30130-00965-3/product-73-30130-00965-3.jpg"
              fit="fill"
            />
          </div>
        </div>
        <div style="padding: 14px;text-align: right;">
          <h3>Unified Diagnostic Services UDS</h3>
          <div class="bottom">
            <div>
              <el-button
                class="button"
                type="primary"
                @click="openCreateUds"
              >
                Create UDS Project
              </el-button>
              <el-button
                class="button"
                type="success"
              >
                Open Exist Project
              </el-button>
            </div>
          </div>
        </div>
      </el-card>
    </div>
    <createUds ref="createUdsRef" />
  </div>
</template>
<script setup lang="ts">
import Logo from '@/assets/logo.png'
import Group from '@/assets/group.png'
import UDSIMG from '@/assets/uds.png'
import { Link, ChatDotRound } from '@element-plus/icons-vue'
import { ref, onMounted, onBeforeUnmount, provide, onUnmounted } from "vue";
import createUds from '@/components/createUds.vue';

const createUdsRef = ref();
const activeDevice=ref('PEAK')



function openCreateUds() {
  createUdsRef.value?.open();
}
</script>

<style scoped>
.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0 20px;
  height: 60px;
}

.logo-container {
  display: flex;
  align-items: center;
}

.logo {
  width: 150px;
  height: auto;
}

.github-container {
  display: flex;
  align-items: center;
}

.el-link {
  /* color: white; */
}

.image{
  height: auto;
  width: 300px;
}

.card {
  margin-top: 13px;
  line-height: 12px;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.qrcode {
  width: 250px;
  height: auto;
}
</style>