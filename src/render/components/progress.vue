<template>
  <div>
    <div v-for="item in progress" :key="item.name">
      <el-row v-if="item.show">
        <el-col :span="2" style="text-align: center">
          <el-tag size="mini" style="width: 100%">{{
            item.name
          }}</el-tag></el-col
        >
        <el-col :span="20" :offset="1">
          <el-progress
            :stroke-width="20"
            :text-inside="true"
            :percentage="item.percent"
            :show-text="true"
            :color="item.color"
            class="mypro"
          ></el-progress>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      progress: [],
    };
  },
  mounted() {
    ipcRenderer.on("progress", (event, val) => {
      let found = false;
      for (let i in this.progress) {
        if (this.progress[i].name == val.name) {
          this.progress[i].show = val.show;
          this.progress[i].percent = val.percent;
          found = true;
          break;
        }
      }
      if (!found) {
        val.color = this.randomColor(val.name);
        this.progress.push(val);
      }
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("progress");
  },
  methods: {
    randomColor() {
      var letters = "89ABCDEF";
      var color = "#59a";
      for (var i = 0; i < 3; i++) {
        color += letters[Math.floor(Math.random() * 8)];
      }
      return color;
    },
    resetProgress() {
      this.progress = [];
    },
  },
};
</script>

<style>
.mypro : {
  text-align: center;
  width: 100%;
  float: right;
}
</style>