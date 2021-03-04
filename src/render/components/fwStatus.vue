<template>
  <div>
    <div style="text-align: right">
      <el-tag type="danger">IVT</el-tag>
      <el-tag type="success">XRDC</el-tag>
      <el-tag type="warning">APP-BL</el-tag>
      <el-tag>SMR</el-tag>
    </div>
    <el-divider
      >Firmware Size: 0x{{ fwInfo.size.toString(16) }} Bytes</el-divider
    >
    <div class="fw">
      <div style="width: 100%">
        <el-popover placement="bottom" trigger="hover">
          <div style="text-align: center">
            <el-tag type="danger">IVT-Header</el-tag>
            <div style="margin: 2px">
              0x{{ fwInfo.ivtAddr.toString(16) }}~0x{{
                (fwInfo.ivtAddr + 256).toString(16)
              }}
            </div>
            <div v-for="(val, key) in fwInfo.ivtInfo" :key="key">
              <div v-if="key != 'gmac'">
                <strong>{{ key }}</strong
                >:0x{{ val.toString(16) }}
              </div>
              <div v-else>
                <strong>{{ key }}</strong
                >:0x{{ val.toString("hex") }}
              </div>
            </div>
          </div>
          <el-button
            slot="reference"
            class="ivt"
            :style="{
              width: (256 / fwInfo.size) * 100 + '%',
              left: (fwInfo.ivtInfo.ivtAddr / fwInfo.size) * 100 + '%',
            }"
          ></el-button>
        </el-popover>
        <el-popover
          placement="bottom"
          trigger="hover"
          v-if="fwInfo.ivtInfo.appBlAddr != 0"
        >
          <div style="text-align: center">
            <el-tag type="warning">APP-Header</el-tag>
            <div style="margin: 2px">
              0x{{ fwInfo.ivtInfo.appBlAddr.toString(16) }}~0x{{
                (
                  fwInfo.ivtInfo.appBlAddr +
                  64 +
                  fwInfo.appBlHeaderInfo.size +
                  16
                ).toString(16)
              }}
            </div>
            <div v-for="(val, key) in fwInfo.appBlHeaderInfo" :key="key">
              <div v-if="key != 'gmac'">
                <strong>{{ key }}</strong
                >:0x{{ val.toString(16) }}
              </div>
              <div v-else>
                <strong>{{ key }}</strong
                >:0x{{ val.toString("hex") }}
              </div>
            </div>
          </div>
          <el-button
            slot="reference"
            class="appbl"
            :style="{
              width:
                ((64 + fwInfo.appBlHeaderInfo.size + 16) / fwInfo.size) * 100 +
                '%',
              left:
                ((fwInfo.ivtInfo.appBlAddr - fwInfo.startAddr) / fwInfo.size) *
                  100 +
                '%',
            }"
          ></el-button>
        </el-popover>
        <el-popover
          placement="bottom"
          trigger="hover"
          v-if="fwInfo.ivtInfo.appBlAddr != 0"
        >
          <div style="text-align: center">
            <el-tag type="warning">APP-Header</el-tag>
            <div style="margin: 2px">
              0x{{ fwInfo.ivtInfo.appBlAddr.toString(16) }}~0x{{
                (
                  fwInfo.ivtInfo.appBlAddr +
                  64 +
                  fwInfo.appBlHeaderInfo.size +
                  16
                ).toString(16)
              }}
            </div>
            <div v-for="(val, key) in fwInfo.appBlHeaderInfo" :key="key">
              <div v-if="key != 'gmac'">
                <strong>{{ key }}</strong
                >:0x{{ val.toString(16) }}
              </div>
              <div v-else>
                <strong>{{ key }}</strong
                >:0x{{ val.toString("hex") }}
              </div>
            </div>
          </div>
          <el-button
            slot="reference"
            class="appbl"
            :style="{
              width:
                ((64 + fwInfo.appBlHeaderInfo.size + 16) / fwInfo.size) * 100 +
                '%',
              left:
                ((fwInfo.ivtInfo.appBlAddr - fwInfo.startAddr) / fwInfo.size) *
                  100 +
                '%',
            }"
          ></el-button>
        </el-popover>
        <el-popover
          placement="bottom"
          trigger="hover"
          v-if="fwInfo.ivtInfo.xrdcAddr != 0"
        >
          <div style="text-align: center">
            <el-tag type="success">XRDC</el-tag>
            <div style="margin: 2px">
              0x{{ fwInfo.ivtInfo.xrdcAddr.toString(16) }}~0x{{
                (fwInfo.ivtInfo.xrdcAddr + 256).toString(16)
              }}
            </div>
            <div v-for="(val, key) in fwInfo.xrdcInfo" :key="key">
              <div v-if="typeof key == 'number'">
                <strong>{{ key }}</strong
                >:0x{{ val.toString(16) }}
              </div>
              <div v-else>
                <strong>{{ key }}</strong
                >:0x{{ val.toString("hex") }}
              </div>
            </div>
          </div>
          <el-button
            slot="reference"
            class="xrdc"
            :style="{
              width: (256 / fwInfo.size) * 100 + '%',
              left:
                ((fwInfo.ivtInfo.xrdcAddr - fwInfo.startAddr) / fwInfo.size) *
                  100 +
                '%',
            }"
          ></el-button>
        </el-popover>

        <div v-for="item in smr" :key="item.index">
          <el-popover placement="bottom" trigger="hover">
            <div style="text-align: center">
              <el-tag type="success">SMR#{{ item.index }}</el-tag>
              <div style="margin: 2px">
                0x{{ item.startAddr.toString(16) }}~0x{{
                  (item.startAddr + item.length).toString(16)
                }}
              </div>
            </div>
            <el-button
              slot="reference"
              class="smr"
              :style="{
                width: (item.length / fwInfo.size) * 100 + '%',
                left:
                  ((item.startAddr - fwInfo.startAddr) / fwInfo.size) * 100 +
                  '%',
              }"
            ></el-button>
          </el-popover>

          <el-popover placement="bottom" trigger="hover" v-if="item.proof">
            <div style="text-align: center">
              <el-tag type="success">Proof#{{ item.index }}</el-tag>
              <div style="margin: 2px">0x{{ item.proofAddr.toString(16) }}</div>
            </div>
            <el-button
              slot="reference"
              class="proof"
              :style="{
                width: (100 / fwInfo.size) * 100 + '%',
                left:
                  ((item.proofAddr - fwInfo.startAddr) / fwInfo.size) * 100 +
                  '%',
              }"
            ></el-button>
          </el-popover>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {};
  },
  computed: {
    smr: function () {
      return this.$store.state.hseConfig.smr;
    },
  },
  props: {
    fwInfo: {
      type: Object,
      default: function () {
        return {
          startAddr: 0x400000,
          ivtAddr: 0,
          ivtInfo: {
            bcw: 0,
            core0: 0,
            core1: 0,
            xrdcAddr: 0,
            lcwAddr: 0,
            hseFwAddr: 0,
            appBlAddr: 0,
            gmac: Buffer.from([]),
          },
          xrdcInfo: {
            config: Buffer.from([]),
            gmac: Buffer.from([]),
          },
          appBlHeaderInfo: {
            startAddr: 0,
            size: 0,
            coreId: 0,
            gmac: Buffer.from([]),
          },
          size: 0,
        };
      },
    },
  },
};
</script>

<style>
.fw {
  width: 100%;
  height: 50px;
  background-color: lightgray;
  border-radius: 10px;
  margin-top: 10px;
  z-index: 10;
}
.fw .ivt {
  height: 50px;
  background-color: #f56c6c;
  border-radius: 1px;
  position: absolute;
  padding: 0px;
  min-width: 10px;
  z-index: 10;
  margin-left: 20px;
}
.fw .appbl {
  height: 50px;
  background-color: #e6a23c;
  border-radius: 1px;
  position: absolute;
  padding: 0px;
  min-width: 10px;
  z-index: 10;
  margin-left: 20px;
}

.fw .xrdc {
  padding: 0px;
  height: 50px;
  background-color: #67c23a;
  border-radius: 1px;
  position: absolute;
  min-width: 10px;
  z-index: 10;
  margin-left: 20px;
}

.fw .smr {
  padding: 0px;
  height: 50px;
  background-color: #409EFF;
  border-radius: 1px;
  position: absolute;
  min-width: 10px;
  z-index: 2;
  margin-left: 20px;
}
.fw .proof {
  padding: 0px;
  height: 50px;
  background-color: black;
  border-radius: 1px;
  position: absolute;
  min-width: 10px;
  z-index: 3;
  margin-left: 20px;
}
</style>