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
      v-if="refresh"
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
          <el-button @click="downloadFIle(item.name)" type="primary">点击上传</el-button>
          <span style="font-size:12px" v-if="inputData[item.name]">
            {{inputData[item.name].name}}
            <strong>{{'0x'+inputData[item.name].size.toString(16)}}</strong>
          </span>
        </div>
        <div v-else-if="item.type==='uploadFile'">
          <el-button @click="uploadFIle(item.name)" type="primary">点击上传</el-button>
          <span style="font-size:12px" v-if="inputData[item.name]">
            <strong>{{inputData[item.name].name}}</strong>
          </span>
          <!-- <el-input v-model="inputData[item.name]" readonly>
          </el-input>-->
        </div>
      </el-form-item>
      <div class="subheader">User function</div>
      <el-row style="margin-top:10px">
        <div class="fn">function(writeData,readData){</div>
        <codemirror v-model="jsFn"  @blur="jsCheck"  ref="cmEditor"/>
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
      <el-form-item style="text-align:right">
        <span style="color:red;margin-right:5px">{{error}}</span>
        <el-button type="primary" @click="addService">Add Service</el-button>
      </el-form-item>
    </el-form>
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
      inputData: {},
      
      error: "",
      jsFn: "return true;",
      jsError: "",
      refresh:true
    };
  },
  mounted() {
    this.codemirror.setSize('100%',200)
    // you can use this.codemirror to do something...
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
      return this.$refs.cmEditor.codemirror
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
        node: true,
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
      this.inputData[name]={
        name:ipcRenderer.sendSync("saveFilePath"),
        size:0
      }
      this.refresh=false
      this.$nextTick(() => {
        this.refresh = true;
      });
    },
    downloadFIle(name) {
      var val = ipcRenderer.sendSync("downloadFilePath");
      this.inputData[name]={
        name:val.path,
        size:val.size
      }
      this.refresh=false
      this.$nextTick(() => {
        this.refresh = true;
      });
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
          data.payload = [];
          for (var i in this.config.input) {
            var item={}
            item.type=this.config.input[i].type
            item.name=this.config.input[i].name
            if (
              this.config.input[i].type === "downloadFile" ||
              this.config.input[i].type === "uploadFile"
            ) {
              if ((!this.inputData[this.config.input[i].name])&&(this.config.input[i].require)) {
                this.error = "Please chhose a file";
                return;
              }
              if (parseInt(this.inputData.memorySize, 16) <= 0) {
                this.error = "MemorySize should more than 0";
                return;
              }
              if (this.config.input[i].type === "downloadFile") {
                if(this.inputData[this.config.input[i].name]){
                  if (parseInt(this.inputData.memorySize, 16) > this.inputData[this.config.input[i].name].size) {
                    this.error = "MemorySize should less than file size";
                    return;
                  }
                }
              }
              this.error = "";
              item[this.config.input[i].name]=this.inputData[this.config.input[i].name]
            } else if (this.config.input[i].type === "subfunction") {
              item.subFunction = parseInt(this.inputData.subFunction)
              if(this.inputData.suppress){
                item.suppress=true
              }else{
                item.suppress=false
              }
            } else{
              item[this.config.input[i].name]=this.inputData[this.config.input[i].name]
            }
            data.payload.push(item)
          }
          this.$emit("additem", data);
        }
      });
    }
  }
};
</script>

<style>
.fn{
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
