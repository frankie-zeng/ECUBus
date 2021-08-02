<template>
  <div>
    <el-button
      icon="el-icon-folder"
      @click="importConfig"
      size="mini"
      type="info"
      >Import</el-button
    >
    <el-button
      icon="el-icon-share"
      @click="exportConfig"
      size="mini"
      type="info"
      >Export</el-button
    >
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {};
  },
  computed: {
    udsTable: function () {
      return this.$store.state[this.mode + "Table"];
    },
  },
  props: ["mode"],
  methods: {
    importConfig() {
      var file = ipcRenderer.sendSync("readFile");
      if (file) {
        var table = JSON.parse(file);
        /* compatible old verison config file*/
        if (table.length > 0) {
          if ("type" in table[0]) {
            table = [
              {
                name: "sch1",
                services: table,
              },
            ];
          }
        }
        if (this.udsTable.length > 0 && this.udsTable[0].services.length > 0) {
          this.$confirm(
            `Overwrite or append services into schedule tables?`,
            "Action",
            {
              confirmButtonText: "Overwrite",
              cancelButtonText: "Append",
              type: "success",
              distinguishCancelAndClose: true,
            }
          )
            .then(() => {
              this.$store.commit(this.mode + "TableLoad", table);
              this.$emit("imported");
            })
            .catch((action) => {
              if (action == "cancel") {
                for (let i in this.udsTable) {
                  if (table[i] != undefined) {
                    table[i].services = this.udsTable[i].services.concat(
                      table[i].services
                    );
                  } else {
                    table[i] = this.udsTable[i];
                  }
                }
                this.$store.commit(this.mode + "TableLoad", table);
                this.$emit("imported");
              }
            });
        } else {
          this.$store.commit(this.mode + "TableLoad", table);
          this.$emit("imported");
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
          type: "success",
        });
      }
    },
  },
};
</script>

<style>
</style>