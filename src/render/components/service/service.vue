/* eslint-disable no-unused-vars */
<template>
  <div class="subservice">
    <div class="subheader" v-if="!group">{{config.name}}</div>
    <el-form
      :model="inputData"
      :rules="rules"
      ref="ruleForm"
      label-position="top"
      size="small"
      :validate-on-rule-change="true"
    >
      <el-form-item
        :label="item.name"
        :prop="item.name"
        v-for="(item,key) in config.input"
        :key="key"
      >
        <el-input v-model="inputData[item.name]" v-if="item.type==='input'">
          <template slot="prepend">0x</template>
        </el-input>
        <el-select
          v-model="inputData[item.name]"
          v-else-if="item.type==='select'"
          style="width:100%"
          allow-create
          filterable
          placeholder="Choose"
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
              v-model="inputData[item.name]"
              style="width:100%"
              allow-create
              filterable
              placeholder="Sub-Function"
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
            <el-checkbox v-model="inputData['suppress']" label="Suppress" border></el-checkbox>
          </el-col>
        </div>
        <el-input
          v-model="inputData[item.name]"
          v-else-if="item.type==='text'"
          type="textarea"
          :autosize="{ minRows: 3, maxRows: 6}"
        ></el-input>
        <div v-else-if="item.type==='downloadFile'">
          <el-button @click="downloadFIle(item.name)" type="primary">Choose File</el-button>
          <span style="font-size:12px" v-if="inputData[item.name]">
            {{inputData[item.name].name}}
            <strong>{{'0x'+inputData[item.name].size.toString(16)}}</strong>
          </span>
        </div>
        <div v-else-if="item.type==='uploadFile'">
          <el-button @click="uploadFIle(item.name)" type="primary">Choose File</el-button>
          <span style="font-size:12px" v-if="inputData[item.name]">
            <strong>{{inputData[item.name].name}}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
      </el-form-item>
    </el-form>
    <el-collapse v-model="activeNames" @change="colChange">
      <el-collapse-item name="1">
        <template slot="title">
          User function
          <el-popover
            placement="top-start"
            title="Tips"
            width="600"
            trigger="hover"
            style="margin:10px;font-size:16px;"
          >
            <el-table :data="tipsData" height="300px">
              <el-table-column width="150" property="func" label="Function"></el-table-column>
              <el-table-column width="100" property="params" label="Params"></el-table-column>
              <el-table-column width="300" property="desc" label="Description"></el-table-column>
            </el-table>
            <i class="el-icon-warning-outline" slot="reference"></i>
            <!-- <el-button slot="reference" icon="el-icon-warning-outline"></el-button> -->
          </el-popover>
        </template>

        <el-row>
          <div class="fn">function(writeData,readData){</div>
          <codemirror v-model="jsFn" @blur="jsCheck" ref="cmEditor" v-if="showCode"/>
          <div class="fn">}</div>
        </el-row>
        <div id="JSLINT_" v-if="jsError!=''">
          <fieldset id="JSLINT_WARNINGS" class="none">
            <legend>Warnings</legend>
            <div id="JSLINT_WARNINGS_LIST">
              <p v-html="jsError"></p>
            </div>
          </fieldset>
        </div>
      </el-collapse-item>
    </el-collapse>
    <div style="text-align:right;margin-top:10px" v-if="!group">
      <span style="color:red;margin-right:5px">{{error}}</span>
      <el-button
        type="primary"
        @click="addService('additem')"
        v-if="!change"
        size="small"
      >Add Service</el-button>
      <el-button type="warning" @click="addService('edititem')" size="small" v-else>Change Service</el-button>
    </div>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
import jslint from "./../../JSLint/jslint.js";
import report from "./../../JSLint/report.js";

//import "codemirror/mode/javascript/javascript.js";
export default {
  data() {
    return {
      jsFn: "return true;",
      showCode:true,
      activeNames: ["1"],
      inputData: {
        suppress: false,
        subFunction: "",
        "AccessData / securityKey": "",
        communicationType: "",
        TimingParameterRequestRecord: "",
        securityDataRequestRecord: "",
        eventWindowTime: "",
        eventTypeRecord: "",
        "baudrateIdentifier/linkBaudrateRecord": "",
        dataIdentifier: "",
        addressAndLengthFormatIdentifier: "",
        memoryAddress: "",
        memorySize: "",
        transmissionMode: "",
        periodicDataIdentifier: "",
        dynamicallyDefinedDataIdentifier: "",
        dataRecord: "",
        groupOfDTC: "",
        controlOptionRecord: "",
        controlEnableMaskRecord: "",
        routineIdentifier: "",
        routineControlOptionRecord: "",
        "routineControlOptionRecord-ascii": "",
        "Download File": "",
        dataFormatIdentifier: "",
        "Upload File": "",
        transferRequestParameterRecord: "",
        modeOfOperation: "",
        filePathAndNameLength: "",
        filePathAndName: "",
        fileSizeParameterLength: "",
        fileSizeUnCompressed: "",
        fileSizeCompressed: ""
      },
      error: "",
      jsError: "",
      tipsData: [
        {
          func: "this.log(msg)",
          params: "msg can be any type",
          desc: "Ouput log information"
        },
        {
          func: "this.delay(ms)",
          params: "ms,type int",
          desc: "Insert a new delay and wait another new reponse"
        },
        {
          func: "this.openFile(filename,flag)",
          params: "filename is a absolute file name path,flag:default is 'r'",
          desc:
            "Open a file,this function must be call before using readFile,WriteFile and CloseFile"
        },
        {
          func: "this.readFile(size)",
          params: "size,type int",
          desc:
            "Read data from a file,return type is a array, the length of array maybe less than size"
        },
        {
          func: "this.writeFile(data)",
          params: "data,type array of buffer",
          desc: "Write data to a file"
        },
        {
          func: "this.closeFile()",
          params: "null",
          desc: "Close a file"
        },
        {
          func: "this.changeNextFrame(name,value)",
          params: "name:should be payload name,value:the change value",
          desc: "Change the next service data in the schedule table"
        }
      ]
    };
  },
  mounted() {
    if (!this.group) {
      this.codemirror.setSize("100%", 200);
    }
  },
  created() {
    if (typeof this.input != "undefined") {
      var val = this.input;
      for (var j in val.payload) {
        this.inputData[val.payload[j].name] =
          val.payload[j][val.payload[j].name];
        if (val.payload[j].type == "subfunction") {
          this.inputData["suppress"] = val.payload[j]["suppress"];
        }
      }
      this.jsFn = val.func;
      if (this.group) {
        // this.$nextTick(() => {
        //   this.activeNames = [];
        // });
        this.activeNames = [];
      }
    }
  },
  computed: {
    rules: function() {
      var a = {};
      for (var i in this.config.input) {
        if (this.config.input[i].rule) {
          a[this.config.input[i].name] = this.config.input[i].rule;
        }
      }
      return a;
    },
    codemirror() {
      return this.$refs.cmEditor.codemirror;
    }
  },
  props: {
    group: {
      type: Boolean,
      default: function() {
        return false;
      }
    },
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
    },
    change: {
      type: Boolean,
      default: function() {
        return false;
      }
    },
    input: {
      type: Object,
      default: function() {
        return undefined;
      }
    }
  },
  methods: {
    colChange(val){
      if(val=="1"){
        this.showCode=false;
        this.$nextTick(() => {
          this.showCode = true;
        });
      }else{
        this.showCode=false;
      }
    },
    jsCheck() {
      var option = {
        white: true,
        bitwise: true,
        convert: true,
        for: true,
        single: true,
        this: true,
        node: true
      };
      /* workaroud unused arg */
      var result = jslint(
        "function check(writeData=[],readData=[]){\r\nif((writeData.length===0)&&(readData.length===0)){\r\nreturn true;\r\n}\r\n" +
          this.jsFn +
          "\r\n}\r\nmodule.exports=check;",
        option,
        undefined
      );
      this.jsError = report.error(result);
    },
    uploadFIle(name) {
      this.inputData[name] = {
        name: ipcRenderer.sendSync("saveFilePath"),
        size: 0
      };
    },
    downloadFIle(name) {
      var val = ipcRenderer.sendSync("downloadFilePath");
      this.inputData[name] = {
        name: val.path,
        size: val.size
      };
    },
    addService(event) {
      var data = {};
      this.$refs.ruleForm.validate(valid => {
        if (valid) {
          data.type = this.type;
          data.func = this.jsFn;
          data.service = {
            name: this.config.name,
            value: this.config.value
          };
          data.payload = [];
          for (var i in this.config.input) {
            var item = {};
            item.type = this.config.input[i].type;
            item.name = this.config.input[i].name;
            if (
              this.config.input[i].type === "downloadFile" ||
              this.config.input[i].type === "uploadFile"
            ) {
              if (
                !this.inputData[this.config.input[i].name] &&
                this.config.input[i].require
              ) {
                this.error = "Please chhose a file";
                return;
              }
              if (parseInt(this.inputData.memorySize, 16) <= 0) {
                this.error = "MemorySize should more than 0";
                return;
              }
              if (this.config.input[i].type === "downloadFile") {
                if (this.inputData[this.config.input[i].name]) {
                  if (
                    parseInt(this.inputData.memorySize, 16) >
                    this.inputData[this.config.input[i].name].size
                  ) {
                    this.error = "MemorySize should less than file size";
                    return;
                  }
                }
              }
              this.error = "";
              item[this.config.input[i].name] = this.inputData[
                this.config.input[i].name
              ];
            } else if (this.config.input[i].type === "subfunction") {
              item.subFunction = parseInt(this.inputData.subFunction);
              if (this.inputData.suppress) {
                item.suppress = true;
              } else {
                item.suppress = false;
              }
            } else {
              item[this.config.input[i].name] = this.inputData[
                this.config.input[i].name
              ];
            }
            data.payload.push(item);
          }
          this.$emit(event, data);
        }
      });
    }
  }
};
</script>

<style>
.fn {
  margin: 5px;
  font-size: 16px;
  color: gray;
}
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
