/* eslint-disable no-unused-vars */
<template>
  <div class="subservice">
    <div class="subheader">{{config.name}}</div>
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
            <el-select v-model="inputData[item.name]" style="width:100%" allow-create filterable>
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
          <el-button @click="downloadFIle" type="primary">点击上传</el-button>
          <span v-if="filePath!=''" style="font-size:12px">
            {{filePath}}
            <strong>{{'0x'+fileSize.toString(16)}}</strong>
          </span>
          <span v-else style="font-size:12px;color:red">
            <strong>{{fileError}}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
        <div v-else-if="item.type==='uploadFile'">
          <el-button @click="uploadFIle" type="primary">点击上传</el-button>
          <span v-if="filePath!=''" style="font-size:12px">
            <strong>{{filePath}}</strong>
          </span>
          <span v-else style="font-size:12px;color:red">
            <strong>{{fileError}}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
      </el-form-item>
      <el-row style="margin-top:10px">
        <el-col
          :span="8"
          style="color:gray;font-size:12px"
        >校验函数(JS),输入参数(writeData,readData),用户可以自定义接收处理函数，返回true或者false,使用this.log('string')可以打印出log方便调试,使用this.delay(ms)可以当收到NRC(0x78)的时候进行调用</el-col>
        <el-col :span="15" :offset="1">
          <div class="fn">function(writeData,readData){</div>
          <el-input
            @blur="jsCheck"
            type="textarea"
            :autosize="{ minRows: 2, maxRows: 10}"
            placeholder="请输入内容"
            v-model="jsFn"
            class="fnInput"
          ></el-input>
          <div class="fn">}</div>
        </el-col>
      </el-row>
      <div id="JSLINT_" v-if="jsError!=''">
        <fieldset id="JSLINT_WARNINGS" class="none">
          <legend>Warnings</legend>
          <div id="JSLINT_WARNINGS_LIST">
            <p v-html="jsError"></p>
          </div>
        </fieldset>
      </div>
      <el-form-item style="text-align:right">
        <el-button type="primary" @click="addService">Add Service</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
const { ipcRenderer } = require("electron");
import jslint from "./../../JSLint/jslint.js";
import report from "./../../JSLint/report.js";
export default {
  data() {
    return {
      inputData: {},
      filePath: "",
      fileSize: 0,
      fileError: "Please choose a file",
      jsFn: "return true;",
      jsError: ""
    };
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
    jsCheck() {
      var option = {
        white: true,
        bitwise: true,
        convert: true,
        for: true,
        single: true,
        this: true,
      };
      /* workaroud unused arg */
      var result = jslint(
        "function check(writeData,readData){\r\nif((writeData.length===0)||(readData.length===0)){\r\nreturn true;\r\n}\r\n" +
          this.jsFn +
          "\r\n}",
        option,
        undefined
      );
      this.jsError = report.error(result);
    },
    uploadFIle() {
      this.filePath = ipcRenderer.sendSync("saveFile");
      this.filePath;
    },
    downloadFIle() {
      var val = ipcRenderer.sendSync("downloadFile");
      this.filePath = val.path;
      this.fileSize = val.size;
    },
    addService() {
      var data = {};
      this.$refs.ruleForm.validate(valid => {
        if (valid) {
          data.type = this.type;
          data.func = this.jsFn;
          data.service = {
            name: this.config.name,
            value: this.config.value
          };
          for (var i in this.config.input) {
            if (this.config.input[i].type === "downloadFile") {
              if (this.filePath === "") {
                return;
              }
              data.other = {
                filePath: this.filePath,
                fileSize: this.fileSize
              };
            } else if (this.config.input[i].type === "uploadFile") {
              data.other = {
                filePath: this.filePath,
                fileSize: this.fileSize
              };
            }
            data.payload = this.inputData;
          }
          // console.log(data)
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
.fnInput{
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
#JSLINT_  legend {
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
#JSLINT_WARNINGS>legend {
    background-color: indianred;
}
#JSLINT_WARNINGS>div {
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
