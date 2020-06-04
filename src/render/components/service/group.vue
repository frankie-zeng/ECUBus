/* eslint-disable no-unused-vars */
<template>
  <div class="subservice">
    <div class="subheader">{{config.name}}</div>
    <el-form
      :model="inputData"
      :rules="rules"
      ref="groupForm"
      label-position="top"
      size="small"
      :validate-on-rule-change="true"
      v-if="refresh"
    >
      <el-form-item
        :label="item.name"
        :prop="key+'-'+item.name"
        v-for="(item,key) in config.input"
        :key="key"
      >
        <el-input v-model="inputData[key+'-'+item.name]" v-if="item.type==='input'">
          <template slot="prepend">0x</template>
        </el-input>
        <el-select
          v-model="inputData[key+'-'+item.name]"
          v-else-if="item.type==='select'"
          style="width:100%"
          allow-create
          filterable
        >
          <el-option
            v-for="child in item.options"
            :key="child.value"
            :label="child.name"
            :value="child.value"
          >
            <span style="float: left">{{ child.name }}</span>
            <span style="float: right; color: #8492a6; font-size: 13px">{{ child.value }}</span>
          </el-option>
        </el-select>
        <div v-else-if="item.type==='subfunction'">
          <el-col :span="16">
            <el-select
              v-model="inputData[key+'-'+item.name]"
              style="width:100%"
              allow-create
              filterable
            >
              <el-option
                v-for="child in item.options"
                :key="child.value"
                :label="child.name"
                :value="child.value"
              >
                <span style="float: left">{{ child.name }}</span>
                <span
                  style="float: right; color: #8492a6; font-size: 13px"
                >{{ '0x'+child.value.toString(16) }}</span>
              </el-option>
            </el-select>
          </el-col>
          <el-col :span="7" :offset="1" style="text-align:right">
            <el-checkbox
              v-model="inputData[key+'-suppress']"
              label="Suppress"
              border
              @change="suppressChange(key)"
            ></el-checkbox>
          </el-col>
        </div>
        <el-input
          v-model="inputData[key+'-'+item.name]"
          v-else-if="item.type==='text'"
          type="textarea"
          :autosize="{ minRows: 3, maxRows: 6}"
        ></el-input>
        <div v-else-if="item.type==='downloadFile'">
          <el-button @click="downloadFIle(key+'-'+item.name)" type="primary">Choose File</el-button>
          <span style="font-size:12px" v-if="inputData[key+'-'+item.name]">
            {{inputData[key+'-'+item.name].name}}
            <strong>{{'0x'+inputData[key+'-'+item.name].size.toString(16)}}</strong>
          </span>
        </div>
        <div v-else-if="item.type==='uploadFile'">
          <el-button @click="uploadFIle(key+'-'+item.name)" type="primary">Choose File</el-button>
          <span style="font-size:12px" v-if="inputData[key+'-'+item.name]">
            <strong>{{inputData[key+'-'+item.name].name}}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
      </el-form-item>
      <el-table
        size="mini"
        ref="basictable"
        row-key="date"
        border
        :data="config.table"
        style="width: 100%;text-align: center"
      >
        <el-table-column prop="service" label="Service Info" width="300">
          <template
            slot-scope="scope"
          >{{ scope.row.service.name}} (0X{{ scope.row.service.value.toString(16)}})</template>
        </el-table-column>
        <el-table-column label="Suppress" width="76" align="center">
          <template slot-scope="scope">
            <i
              class="el-icon-circle-check"
              v-if="scope.row.payload[0].suppress"
              style="color:green"
            ></i>
            <i class="el-icon-circle-close" v-else style="color:red"></i>
          </template>
        </el-table-column>
        <el-table-column prop="payload" label="Payload">
          <template slot-scope="scope">
            <div v-if="scope.row.payload">
              <div v-for="(item, key) in scope.row.payload" :key="key">
                <div v-if="config.changeslot.indexOf(scope.$index+','+key)!==-1">
                  <el-tag size="mini" type="danger">{{item.name}}</el-tag>
                  : {{inputData[config.changeslot.indexOf(scope.$index+','+key)+'-'+item.name]}}
                </div>
                <div v-else>
                  <el-tag size="mini">{{item.name}}</el-tag>
                  : {{item[item.name]}}
                </div>
              </div>
            </div>
            <div v-else>NULL</div>
          </template>
        </el-table-column>
      </el-table>
      <el-form-item style="text-align:right;margin-top:10px">
        <span style="color:red;margin-right:5px;">{{error}}</span>
        <el-button type="primary" @click="addService">Add Group</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      inputData: {},
      error: "",
      refresh: true
    };
  },
  computed: {
    rules: function() {
      var a = {};
      for (var i in this.config.input) {
        if (this.config.input[i].rule) {
          a[i + "-" + this.config.input[i].name] = this.config.input[i].rule;
        }
      }
      return a;
    }
  },
  props: {
    config: {
      type: Object,
      default: function() {
        return {};
      }
    },
    type: {
      type: String,
      default: function() {
        return "uds";
      }
    }
  },
  methods: {
    suppressChange(key) {
      var index = this.config.changeslot[key].split(",")[0];
      this.config.table[index].payload[0].suppress = this.inputData[
        key + "-suppress"
      ];
    },
    uploadFIle(name) {
      this.inputData[name] = {
        name: ipcRenderer.sendSync("saveFilePath"),
        size: 0
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
        size: val.size
      };
      this.refresh = false;
      this.$nextTick(() => {
        this.refresh = true;
      });
    },
    addService() {
      var data = {};
      this.$refs.groupForm.validate(valid => {
        if (valid) {
          data.type = "group";

          data.service = {
            name: this.config.name
          };
          for (var i in this.config.input) {
            var name = this.config.input[i].name;
            var key = i + "-" + name;
            var slot = this.config.changeslot[i].split(",");
            var tableIndex = slot[0];
            var payloadIndex = slot[1];
            if (
              this.config.input[i].type === "downloadFile" ||
              this.config.input[i].type === "uploadFile"
            ) {
              if (!this.inputData[key]) {
                this.error = "Please chhose a file";
                return;
              }
              this.error = "";
              this.config.table[tableIndex].payload[payloadIndex][
                name
              ] = this.inputData[key];
            } else if (this.config.input[i].type === "subfunction") {
              this.config.table[tableIndex].payload[payloadIndex][
                name
              ] = parseInt(this.inputData[key]);
            } else {
              this.config.table[tableIndex].payload[payloadIndex][
                name
              ] = this.inputData[key];
            }
          }
          data.subtable = this.config.table;
          this.$emit("additem", data);
        }
      });
    }
  }
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
