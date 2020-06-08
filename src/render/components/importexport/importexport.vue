<template>
  <div>
    <el-button icon="el-icon-folder" @click="importConfig" size="mini" type="info">Import</el-button>
    <el-button icon="el-icon-share" @click="exportConfig" size="mini" type="info">Export</el-button>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {};
  },
  computed: {
    udsTable: function() {
      if (this.mode === "can") {
        return this.$store.state.canTable;
      } else if (this.mode === "doip") {
        return this.$store.state.doipTable;
      } else if (this.mode === "lp") {
        return this.$store.state.lpTable;
      } else {
        return [];
      }
    }
  },
  props: ["mode"],
  methods: {
    importConfig() {
      var file = ipcRenderer.sendSync("readFile");
      if (file) {
        var table = JSON.parse(file);
        if (this.mode === "doip") {
          this.$store.commit("doipTableLoad", table);
        } else if (this.mode === "can") {
          this.$store.commit("canTableLoad", table);
        } else if (this.mode === "lp") {
          this.$store.commit("lpTableLoad", table);
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
      var file = ipcRenderer.sendSync(
        "saveFile",
        JSON.stringify(val, null, "\t")
      );
      if (file != null) {
        this.$notify({
          title: "Success",
          message: "Saved：" + file,
          type: "success"
        });
      }
    }
  }
};
</script>

<style>
</style>