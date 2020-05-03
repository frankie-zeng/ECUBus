<template>
  <div>
    
    <el-button icon="el-icon-folder" @click="importConfig" size="mini" type="info">导入配置</el-button>
    <el-button icon="el-icon-share" @click="exportConfig" size="mini" type="info">导出配置</el-button>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      
    };
  },
  computed: {
    udsTable: function() {
      if (this.mode === "can") {
        return this.$store.state.canTable;
      } else if (this.mode === "doip") {
        return this.$store.state.doipTable;
      } else {
        return [];
      }
    },
  },
  props: ["mode"],
  methods: {
    importConfig() {
      var file = ipcRenderer.sendSync("readFile");
      if(file){
        var table = JSON.parse(file);
        if (this.mode === "doip") {
          this.$store.commit("doipTableLoad", table);
        } else if (this.mode === "can") {
          this.$store.commit("canTableLoad", table);
        } else {
          return;
        }
      }
    },
    exportConfig() {
      var val = JSON.parse(JSON.stringify(this.udsTable));
      for (var i in val) {
        delete val[i]["addr"];
      }
      var file = ipcRenderer.sendSync("saveFile", JSON.stringify(val));
      if(file!=null){
        this.$notify({
          title: "成功",
          message: "保存成功：" + file,
          type: "success"
        });
      }
    }
  }
};
</script>

<style>
</style>