<template>
  <div>
    <div v-for="item in progress" :key="item.name">
      <el-row>
        <el-col :span="2" style="text-align: center">
          <el-tag size="mini" style="width: 100%">{{
            item.name
          }}</el-tag></el-col
        >
        <el-col :span="21" :offset="1">
          <el-progress
            :stroke-width="20"
            :text-inside="true"
            :percentage="item.percent"
            v-if="item.show"
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
      var letters = "0123456789ABCDEF";
      var color = "#";
      for (var i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
      }
      return color;
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