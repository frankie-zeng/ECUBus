<template>
  <div>
    <el-page-header
      style="margin: 0px"
      @back="goBack"
      content="HSE Helper"
      class="header"
      title=""
    ></el-page-header>
    <!-- hse fw load -->
    <div>
      <div v-if="!isFw" style="margin: 20px">
        <el-button type="primary" plain style="width: 100%" @click="uploadFw"
          >Upload Frimware</el-button
        >
      </div>
      <div v-else>
        <FwStatus :fwInfo="fwInfo"/>
        <div class="fwDel">
          <el-button
            type="danger"
            icon="el-icon-delete"
            class="fwDel"
            @click="isFw = false"
            size="mini"
          ></el-button>
        </div>
      </div>
    </div>
    <el-row>
      <HseFormat />
    </el-row>
    <el-row>
      <HseSMR :isFw="isFw" :fwInfo="fwInfo"/>
    </el-row>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
const hexParser = require("./../../hse_helper/intel_hex.js");
const ivtTAG = Buffer.from("a55aa55a", "hex");

import FwStatus from "./../components/fwStatus.vue";
import HseSMR from "./../components/hsesmr.vue";
import HseFormat from "./../components/hseformat.vue";
export default {
  components: {
    FwStatus,
    HseSMR,
    HseFormat,
  },
  data() {
    return {
      isFw: false,
      fw: {},
      fwInfo: {
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
      },
    };
  },

  created() {},
  methods: {
    goBack() {
      this.$router.push("/");
    },
    uploadFw() {
      var file = ipcRenderer.sendSync("readFile", [
        { name: "HEX", extensions: ["hex"] },
      ]);
      if (file !== " ") {
        try {
          this.fw = hexParser.parse(file);
          //find ivt header in fw
          this.fwInfo.size = this.fw.data.length;
          this.fwInfo.ivtAddr = this.fw.data.indexOf(ivtTAG);

          this.fwInfo.ivtInfo.bcw = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 4
          );
          this.fwInfo.ivtInfo.core0 = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0xc
          );
          this.fwInfo.ivtInfo.core1 = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0x14
          );
          this.fwInfo.ivtInfo.xrdcAddr = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0x20
          );
          this.fwInfo.ivtInfo.lcwAddr = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0x24
          );
          this.fwInfo.ivtInfo.hseFwAddr = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0x2c
          );
          this.fwInfo.ivtInfo.appBlAddr = this.fw.data.readUInt32LE(
            this.fwInfo.ivtAddr + 0x30
          );
          this.fwInfo.ivtInfo.gmac = this.fw.data.slice(
            this.fwInfo.ivtAddr + 0xf0,
            this.fwInfo.ivtAddr + 0x100
          );
          if (this.fwInfo.ivtInfo.xrdcAddr != 0) {
            //find xrdc
            let txrdcTag = this.fw.data.readUInt32LE(
              this.fwInfo.ivtInfo.xrdcAddr - this.fwInfo.startAddr
            );
            if (txrdcTag != 0xcc5577cc) {
              this.$notify.error({
                title: "Error",
                message: `XRDC TAG unmatch, hope 0xCC5577CC, got ${txrdcTag.toString(
                  16
                )}`,
                duration: 0,
              });
              return;
            } else {
              this.fwInfo.xrdcInfo.config = this.fw.data.slice(
                this.fwInfo.ivtInfo.xrdcAddr - this.fwInfo.startAddr + 0x4,
                this.fwInfo.ivtInfo.xrdcAddr - this.fwInfo.startAddr + 0x90
              );
              this.fwInfo.xrdcInfo.gmac = this.fw.data.slice(
                this.fwInfo.ivtInfo.xrdcAddr - this.fwInfo.startAddr + 0xf0,
                this.fwInfo.ivtInfo.xrdcAddr - this.fwInfo.startAddr + 0x100
              );
            }
          }
          if (this.fwInfo.ivtInfo.appBlAddr != 0) {
            //find boot image
            let bootTag = this.fw.data.readUInt8(
              this.fwInfo.ivtInfo.appBlAddr - this.fwInfo.startAddr
            );
            let version = this.fw.data.readUInt8(
              this.fwInfo.ivtInfo.appBlAddr - this.fwInfo.startAddr + 3
            );
            if (bootTag == 0xd5 && version == 0x60) {
              this.fwInfo.appBlHeaderInfo.startAddr = this.fw.data.readUInt32LE(
                this.fwInfo.ivtInfo.appBlAddr - this.fwInfo.startAddr + 8
              );
              this.fwInfo.appBlHeaderInfo.size = this.fw.data.readUInt32LE(
                this.fwInfo.ivtInfo.appBlAddr - this.fwInfo.startAddr + 0xc
              );
              this.fwInfo.appBlHeaderInfo.coreId = this.fw.data.readUInt8(
                this.fwInfo.ivtInfo.appBlAddr - this.fwInfo.startAddr + 0x10
              );
              this.fwInfo.appBlHeaderInfo.gmac = this.fw.data.slice(
                this.fwInfo.ivtInfo.appBlAddr -
                  this.fwInfo.startAddr +
                  0x40 +
                  this.fwInfo.appBlHeaderInfo.size,
                this.fwInfo.ivtInfo.appBlAddr -
                  this.fwInfo.startAddr +
                  0x50 +
                  this.fwInfo.appBlHeaderInfo.size
              );
            } else {
              this.$notify.error({
                title: "Error",
                message: `App image TAG unmatch, hope 0xd5,0x60, got ${bootTag.toString(
                  16
                )},${version.toString(16)}`,
                duration: 0,
              });
              return;
            }
          }
          this.isFw = true;
        } catch (e) {
          this.$notify.error({
            title: "Error",
            message: e,
            duration: 0,
          });
        }
      }
    },
  },
};
</script>

<style>
.fwDel {
  text-align: right;
  border-radius: 20px;
  margin: 5px;
}
.smr {
  border-style: solid;
  border-width: 1px;
  border-radius: 10px;
  border-color: #409eff;
  margin: 5px;
  height: 300px;
  width: 100%;
}
</style>