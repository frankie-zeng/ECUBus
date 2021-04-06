/* eslint-disable no-unused-vars */
<template>
  <div class="subservice">
    <div class="subheader">{{ config.name }}</div>
    <div v-for="(item, key) in config.service" :key="key">
      <Service
        :config="item"
        :input="change ? input.subtable[key] : config.table[key]"
        group
        @groupitem="groupChange"
        ref="service"
      />
    </div>
    <div style="text-align: right; margin-top: 10px">
      <el-button type="primary" @click="addGroup" size="small" v-if="!change"
        >Add Group</el-button
      >
      <el-button type="warning" @click="addGroup" size="small" v-else
        >Change Group</el-button
      >
    </div>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
import Service from "./service.vue";
export default {
  data() {
    return {
      cnt: 0,
      group: [],
      inputData: {},
      error: "",
      refresh: true,
    };
  },
  components: {
    Service,
  },
  computed: {
    rules: function () {
      var a = {};
      for (var i in this.config.input) {
        if (this.config.input[i].rule) {
          a[i + "-" + this.config.input[i].name] = this.config.input[i].rule;
        }
      }
      return a;
    },
  },
  props: {
    config: {
      type: Object,
      default: function () {
        return {};
      },
    },
    type: {
      type: String,
      default: function () {
        return "uds";
      },
    },
    change: {
      type: Boolean,
      default: function () {
        return false;
      },
    },
    input: {
      type: Object,
      default: function () {
        return undefined;
      },
    },
  },
  methods: {
    groupChange(val) {
      this.group[this.cnt].func = val.func;
      this.group[this.cnt].preFunc = val.preFunc;
      for (var i in val.payload) {
        for (var j in this.group[this.cnt].payload) {
          let name = this.group[this.cnt].payload[j].name;
          if (val.payload[i].name == name) {
            this.group[this.cnt].payload[j][name] = val.payload[i][name];
            if (this.group[this.cnt].payload[j].type == "subfunction") {
              this.group[this.cnt].payload[j].suppress =
                val.payload[i].suppress;
            }
            break;
          }
        }
      }
      this.cnt++;
      if (this.cnt == this.group.length) {
        this.$emit(this.change ? "edititem" : "additem", {
          service: {
            name: this.config.name,
          },
          type: "group",
          subtable: this.group,
        });
      }
    },
    suppressChange(key) {
      var index = this.config.changeslot[key].split(",")[0];
      this.config.table[index].payload[0].suppress = this.inputData[
        key + "-suppress"
      ];
    },
    uploadFIle(name) {
      this.inputData[name] = {
        name: ipcRenderer.sendSync("saveFilePath"),
        size: 0,
      };
      this.refresh = false;
      this.$nextTick(() => {
        this.refresh = true;
      });
    },
    downloadFIle(name) {
      var val = ipcRenderer.sendSync("downloadFilePath");
      this.inputData[name] = {
        name: val.path,
        size: val.size,
      };
      this.refresh = false;
      this.$nextTick(() => {
        this.refresh = true;
      });
    },
    addGroup() {
      this.group = JSON.parse(JSON.stringify(this.config.table));
      for (var i in this.$refs.service) {
        this.$refs.service[i].addService("groupitem");
      }
      this.cnt = 0;
    },
    generateData() {
      var group = JSON.parse(JSON.stringify(this.config.table));
      for (var z in this.$refs.service) {
        let val = this.$refs.service[z].generateData();
        group[z].func = val.func;
        group[z].preFunc = val.preFunc;
        for (var i in val.payload) {
          for (var j in group[z].payload) {
            let name = group[z].payload[j].name;
            if (val.payload[i].name == name) {
              group[z].payload[j][name] = val.payload[i][name];
              if (group[z].payload[j].type == "subfunction") {
                group[z].payload[j].suppress = val.payload[i].suppress;
              }
              break;
            }
          }
        }
      }
      return group;
    },
    // addService() {
    //   var data = {};
    //   this.$refs.groupForm.validate(valid => {
    //     if (valid) {
    //       data.type = "group";

    //       data.service = {
    //         name: this.config.name
    //       };
    //       for (var i in this.config.input) {
    //         var name = this.config.input[i].name;
    //         var key = i + "-" + name;
    //         var slot = this.config.changeslot[i].split(",");
    //         var tableIndex = slot[0];
    //         var payloadIndex = slot[1];
    //         if (
    //           this.config.input[i].type === "downloadFile" ||
    //           this.config.input[i].type === "uploadFile"
    //         ) {
    //           if (!this.inputData[key]) {
    //             this.error = "Please chhose a file";
    //             return;
    //           }
    //           this.error = "";
    //           this.config.table[tableIndex].payload[payloadIndex][
    //             name
    //           ] = this.inputData[key];
    //         } else if (this.config.input[i].type === "subfunction") {
    //           this.config.table[tableIndex].payload[payloadIndex][
    //             name
    //           ] = parseInt(this.inputData[key]);
    //         } else {
    //           this.config.table[tableIndex].payload[payloadIndex][
    //             name
    //           ] = this.inputData[key];
    //         }
    //       }
    //       data.subtable = this.config.table;
    //       this.$emit("additem", data);
    //     }
    //   });
    // }
  },
};
</script>

<style>
.subservice {
  overflow: hidden;
}
.serviceImg {
  width: 50px;
  height: 50px;
  position: absolute;
  bottom: 0px;
  right: 0px;
}
.subheader {
  font-family: Impact, Haettenschweiler, "Arial Narrow Bold", sans-serif;
  font-size: 25px;
  margin-bottom: 10px;
}
.fnInput {
  color: darkblue;
}
#JSLINT_ fieldset {
  background-color: gainsboro;
  border: 0;
  clear: both;
  margin-bottom: 1em;
  margin-left: 1%;
  margin-right: 1%;
  margin-top: 5px;
  padding: 0;
  width: auto;
}
#JSLINT_ legend {
  background-color: darkslategray;
  border: 0;
  color: white;
  font-size: 100%;
  font-style: normal;
  font-weight: normal;
  margin: 0;
  padding-bottom: 0.25em;
  padding-left: 0;
  padding-right: 0;
  padding-top: 0.25em;
  text-align: center;
  width: 100%;
}
#JSLINT_ address {
  color: black;
  display: block;
  float: right;
  font-family: serif;
  font-size: 90%;
  margin-left: 1em;
}
#JSLINT_WARNINGS > legend {
  background-color: indianred;
}
#JSLINT_WARNINGS > div {
  background-color: pink;
  padding: 1px;
}
#JSLINT_WARNINGS cite {
  color: black;
  display: block;
  font-family: serif;
  font-size: 100%;
  font-style: normal;
  margin-bottom: 4pt;
  margin-left: 20pt;
  margin-right: 20pt;
  margin-top: 4pt;
  overflow-x: hidden;
}
#JSLINT_WARNINGS samp {
  background-color: lavenderblush;
  color: black;
  display: block;
  font-family: Programma, monospace;
  font-size: 100%;
  font-style: normal;
  font-weight: bold;
  padding: 4pt;
  margin-bottom: 0;
  margin-left: 16pt;
  margin-right: 16pt;
  margin-top: 0;
  white-space: pre-wrap;
}
#JSLINT_WARNINGS dl address {
  color: black;
  display: inline;
  float: none;
  font-size: 80%;
  margin: 0;
}
</style>
