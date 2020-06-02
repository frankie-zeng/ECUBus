<template>
  <div v-if="show" class="progress">
    <el-progress :percentage="percentage"></el-progress>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      show: false,
      percentage: 50
    };
  },
  mounted() {
    ipcRenderer.on("progress", (event, val) => {
      this.show=val.show
      this.percentage=val.percent
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("progress");
  }
};
</script>

<style>
.progress: {
  text-align: center;
  width: 100%;
}
</style>