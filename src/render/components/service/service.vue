/* eslint-disable no-unused-vars */
<template>
  <div class="subservice">
    <div class="subheader" v-if="!group">{{ config.name }}</div>
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
        v-for="(item, key) in config.input"
        :key="key"
      >
        <el-input v-model="inputData[item.name]" v-if="item.type === 'input'">
          <template slot="prepend">0x</template>
        </el-input>
        <el-select
          v-model="inputData[item.name]"
          v-else-if="item.type === 'select'"
          style="width: 100%"
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
            <span style="float: right; color: #8492a6; font-size: 13px">{{
              child.value
            }}</span>
          </el-option>
        </el-select>
        <div v-else-if="item.type === 'subfunction'">
          <el-col :span="16">
            <el-select
              v-model="inputData[item.name]"
              style="width: 100%"
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
                <span style="float: right; color: #8492a6; font-size: 13px">{{
                  "0x" + child.value.toString(16)
                }}</span>
              </el-option>
            </el-select>
          </el-col>
          <el-col :span="7" :offset="1" style="text-align: right">
            <el-checkbox
              v-model="inputData['suppress']"
              label="Suppress"
              border
            ></el-checkbox>
          </el-col>
        </div>
        <el-input
          v-model="inputData[item.name]"
          v-else-if="item.type === 'text'"
          type="textarea"
          :autosize="{ minRows: 3, maxRows: 6 }"
        ></el-input>
        <div v-else-if="item.type === 'downloadFile'">
          <el-button @click="downloadFIle(item.name)" type="primary"
            >Choose File</el-button
          >
          <span style="font-size: 12px" v-if="inputData[item.name]">
            {{ inputData[item.name].name }}
            <strong>{{ "0x" + inputData[item.name].size.toString(16) }}</strong>
          </span>
        </div>
        <div v-else-if="item.type === 'uploadFile'">
          <el-button @click="uploadFIle(item.name)" type="primary"
            >Choose File</el-button
          >
          <span style="font-size: 12px" v-if="inputData[item.name]">
            <strong>{{ inputData[item.name].name }}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
      </el-form-item>
    </el-form>
    <el-row>
      <el-col :span="6">
        <el-tag type="info">Service Feature Description:</el-tag>
      </el-col>
      <el-col :span="18">
        <el-input
          v-model="desc"
          type="textarea"
          :autosize="{ minRows: 1, maxRows: 3 }"
        ></el-input>
      </el-col>
    </el-row>
    <span style="color: red; margin-right: 5px">{{ error }}</span>
    <el-collapse v-model="activeNames" @change="colChange">
      <el-collapse-item name="1">
        <template slot="title"> User function (preLoad) </template>
        <div>
          <el-button
            type="text"
            icon="el-icon-full-screen"
            class="btn2"
            @click="fullScreen(0)"
          ></el-button>
           <el-button
            type="text"
            @click="openApi"
            class="btn3"
            icon="el-icon-question"
          ></el-button>
          <div class="fn">function preLoad(writeData) {</div>
          <codemirror
            v-model="jsPreFn"
            ref="cmEditorPre"
            v-if="showCode"
            :options="cmOptions"
          />
          <div class="fn">}</div>
        </div>
      </el-collapse-item>
      <el-collapse-item name="2">
        <template slot="title"> User function (afterLoad) </template>
        <div>
          <el-button
            type="text"
            icon="el-icon-full-screen"
            class="btn2"
            @click="fullScreen(1)"
          ></el-button>
          <el-button
            type="text"
            @click="openApi"
            class="btn3"
            icon="el-icon-question"
          ></el-button>
          <div class="fn">function afterLoad(writeData,readData) {</div>
          <codemirror
            v-model="jsFn"
            ref="cmEditorAfter"
            v-if="showCode"
            :options="cmOptions"
          />
          <div class="fn">}</div>
        </div>
      </el-collapse-item>
    </el-collapse>

    <div style="text-align: right; margin-top: 10px" v-if="!group">
      <el-button
        type="primary"
        @click="addService('additem')"
        v-if="!change"
        size="small"
        >Add Service</el-button
      >
      <el-button
        type="warning"
        @click="addService('edititem')"
        size="small"
        v-else
        >Change Service</el-button
      >
    </div>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
import { JSHINT } from "jshint-esnext";
window.JSHINT = JSHINT;
import "codemirror/lib/codemirror.css";
import "codemirror/mode/javascript/javascript.js";
import "codemirror/theme/eclipse.css";
import "./lint.css";
import "codemirror/addon/lint/lint.js";
import "codemirror/addon/lint/javascript-lint.js";
// import "codemirror/addon/hint/show-hint.js";
// import "codemirror/addon/hint/show-hint.css";
//import "codemirror/mode/javascript/javascript.js";
export default {
  data() {
    return {
      codeMirror: [],
      jsFn: "return true;",
      jsPreFn: "",
      showCode: true,
      desc: "",
      activeNames: ["2"],
      cmOptions: {
        theme: "eclipse",
        mode: "javascript",
        lineNumbers: true,
        gutters: ["CodeMirror-lint-markers"],
        lint: {
          esnext: true,
          experimental: ["asyncawait"],
        },
        styleSelectedText: true,
        extraKeys: {
          F11: function (cm) {
            cm.setOption("fullScreen", !cm.getOption("fullScreen"));
          },
          Esc: function (cm) {
            if (cm.getOption("fullScreen")) cm.setOption("fullScreen", false);
          },
        },
      },
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
        fileSizeCompressed: "",
      },
      rules: {},
      error: "",
    };
  },
  mounted() {
    this.updateCodeMirror();
    if (!this.group) {
      for (let i = 0; i < this.codemirror.length; i++) {
        this.codemirror[i].setSize("100%", 200);
      }

      // this.codemirror.setOption('fullScreen',true)
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
      this.jsPreFn = val.preFunc == undefined ? "" : val.preFunc;
      this.desc = val.desc == undefined ? "" : val.desc;
      if (this.group) {
        // this.$nextTick(() => {
        //   this.activeNames = [];
        // });
        this.activeNames = [];
      }
    }
    if (typeof this.config != "undefined") {
      for (var i in this.config.input) {
        if (this.config.input[i].rule) {
          this.rules[this.config.input[i].name] = this.config.input[i].rule;
          for (let j in this.config.input[i].rule) {
            if (this.config.input[i].rule[j].pattern) {
              let raw = new RegExp(this.config.input[i].rule[j].pattern);
              let other = /^(.*?)\((.*?)\)$/;
              //a[this.config.input[i].name][j].pattern='('+rule+')|(^\\${.*?}$)|(^.*?\\(\\${.*?}\\)$)'
              this.rules[this.config.input[i].name][j].pattern = new RegExp(
                raw.source + "|" + other.source
              );

              break;
            }
          }
        }
      }
    }
  },
  // computed: {
  //   codemirror() {
  //     let a = [];
  //     a.push(this.$refs.cmEditorPre.codemirror);
  //     a.push(this.$refs.cmEditorAfter.codemirror);
  //     return a;
  //   },
  // },
  props: {
    group: {
      type: Boolean,
      default: function () {
        return false;
      },
    },
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
    updateCodeMirror() {
      this.codemirror = [];
      this.codemirror.push(this.$refs.cmEditorPre.codemirror);
      this.codemirror.push(this.$refs.cmEditorAfter.codemirror);
    },
    openApi() {
      ipcRenderer.send("startApiHelper");
    },
    fullScreen(index) {
      if (!this.codemirror[index].getOption("fullScreen")) {
        this.codemirror[index].setOption("fullScreen", true);
      }
    },
    colChange(val) {
      if (val == "1" || val == "2") {
        this.showCode = false;
        this.$nextTick(() => {
          this.showCode = true;
          this.$nextTick(() => {
            this.updateCodeMirror();
            for (let i = 0; i < this.codemirror.length; i++) {
              this.codemirror[i].setSize("100%", 200);
            }
          });
        });
      } else {
        this.showCode = false;
      }
    },
    uploadFIle(name) {
      var val = {
        name: ipcRenderer.sendSync("saveFilePath",),
        size: 0,
      };
      this.inputData[name] = val;
    },
    downloadFIle(name) {
      var val = ipcRenderer.sendSync("downloadFilePath");
      this.inputData[name] = {
        name: val.path,
        size: val.size,
      };
    },
    generateData() {
      var data = {};
      data.type = this.type;
      data.func = this.jsFn;
      data.desc = this.desc;
      data.preFunc = this.jsPreFn;
      data.service = {
        name: this.config.name,
        value: this.config.value,
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
          item[this.config.input[i].name] =
            this.inputData[this.config.input[i].name];
        } else if (this.config.input[i].type === "subfunction") {
          item.subFunction = parseInt(this.inputData.subFunction);
          if (this.inputData.suppress) {
            item.suppress = true;
          } else {
            item.suppress = false;
          }
        } else {
          item[this.config.input[i].name] =
            this.inputData[this.config.input[i].name];
        }
        data.payload.push(item);
      }
      return data;
    },
    addService(event) {
      this.$refs.ruleForm.validate((valid) => {
        if (valid) {
          var val = this.generateData();
          if (this.error == "") {
            this.$emit(event, val);
          }
        }
      });
    },
  },
};
</script>

<style>
.btn1 {
  position: absolute;
  z-index: 2;
  right: 50px;
}
.btn2 {
  position: absolute;
  z-index: 2;
  right: 30px;
}
.btn3 {
  position: absolute;
  z-index: 2;
  right: 50px;
}
.CodeMirror-fullscreen {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  height: auto;
  z-index: 9;
}

.hint {
  z-index: 2000;
  background-color: red;
}

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
