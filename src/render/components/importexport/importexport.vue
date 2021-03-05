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
      return this.$store.state[this.mode+'Table'];
    }
  },
  props: ["mode"],
  methods: {
    importConfig() {
      var file = ipcRenderer.sendSync("readFile");
      if (file) {
        var table = JSON.parse(file.data);
        /* compatible old verison config file*/
        if(table.length>0){
          if('type' in table[0]){
            table=[{
              name:'sch1',
              services: table,
            }]
          }
        }
        this.$store.commit(this.mode+"TableLoad", table);
      }
      this.$emit("imported");
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