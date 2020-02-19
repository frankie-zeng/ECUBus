<template>
    <div class="uds">
        <div >
            <el-row style="text-align:left">
                服务名字：
                    <el-select v-model="service" placeholder="Choose Service"  filterable style="width:500px">
                        <el-option
                        v-for="(item, key) in DOIPConfig"
                        :key="item.value"
                        :label="item.name"
                        :value="item.value"
                        :disabled="item.disabled">
                          <span style="float: left">{{ item.name }}</span>
                          <span style="float: right; color: #8492a6; font-size: 13px">0x{{ key }}</span>
                        </el-option>
                    </el-select>
            </el-row>
              <el-row v-if="hasParam">
                <div v-for="(item,index) in itemInfo.param"
                  :key="index">
                  <el-tag style="margin-bottom:10px;margin-top:10px">参数{{index+1}} {{item.name}}:</el-tag>
                  <Input :minLen="lenLimit[index][0]" :maxLen="lenLimit[index][1]" :index="index" @change="dataChange" :readonly="item.readonly?true:false"/>
                </div>

              </el-row>
              <div v-for="(item,index) in userParamData"
                  :key="index" style="margin-top:10px">
                  <el-row>
                    <el-col :span="6">
                      <el-input
                        size="mini"
                        placeholder="参数名字"
                        clearable
                        v-model="item.name">:</el-input>
                    </el-col>
                    <el-col :span="1" class="xx">:
                    </el-col>
                    <el-col :span="9">
                      <span v-for="(data,index) in item.data"
                      :key="index" style="margin:1px">
                          <el-input v-model="item.data[index]"  maxlength="2"  size="mini" style="width:45px"/>
                      </span>
                    </el-col>
                    <el-col :span="8">
                      <el-button   type="primary"  size="mini" icon="el-icon-plus" circle @click="addUserParam1(index)"></el-button>
                      <el-button   type="primary"  size="mini" icon="el-icon-minus" @click="deleteUserParam1(index)" circle></el-button>
                      <el-button type="danger"  size="mini" icon="el-icon-close" @click="deleteUserParam(index)" circle></el-button>
                    </el-col>
                  </el-row>
              </div>
              <el-button type="success" size="mini" icon="el-icon-circle-plus-outline"  @click="addUserParam">添加参数</el-button>
              <el-row style="margin-top:10px">
                <el-col :span="5" style="color:gray;font-size:12px">
                校验函数(JS),输入参数(writeData,readData),用户可以自定义接收处理函数，返回true或者false,使用this.log('string')可以打印出log方便调试,使用this.delay(ms)可以当收到NRC(0x78)的时候进行调用
                </el-col>
                <el-col :span="16" :offset="1">
                  <div class="fn">function(writeData,readData){</div>
                  <el-input
                    @blur="jsCheck"
                    type="textarea"
                    :autosize="{ minRows: 2, maxRows: 10}"
                    placeholder="请输入内容"
                    v-model="jsFn"
                    class="fnInput">
                  </el-input>
                   <div class="fn">}</div>
                 </el-col>
              </el-row>
              <div id="JSLINT_" v-if="jsError!=''">
                <fieldset id="JSLINT_WARNINGS" class="none" >
                  <legend>Warnings</legend>
                  <div id="JSLINT_WARNINGS_LIST"><p v-html="jsError"></p></div>
                </fieldset>
              </div>
               <!-- <div v-if="jsError!=''">{{jsError}}</div> -->
              <el-row style="text-align:right">
                 <el-button type="primary" icon="el-icon-plus" @click="addService" >添加服务</el-button>
              </el-row>
            </div>
    </div>
</template>
<script>
import DOIPConfig from './doip.js'
import Input from './input.vue'
import jslint from './../JSLint/jslint.js'
import report from './../JSLint/report.js'
// const { ipcRenderer } = require('electron')
export default {
  data () {
    return {
      DOIPConfig: DOIPConfig,
      addrIndex: '',
      service: 1,
      subFunc: '',
      userParamData: [],
      jsFn: 'return true;',
      jsError: ''
    }
  },
  props:['mode'],
  components: {
    Input
  },
  watch: {
    itemInfo: function () {
      this.userParamData = []
      this.jsFn = 'return true;'
      this.jsError = ''
    }
  },
  computed: {
    connect: function () {
      return this.$store.state.ipConnect
    },
    itemInfo: function () {
      return DOIPConfig[this.service.toString(16)]
    },
    paramLen: function () {
      var a = []
      for (var i in this.lenLimit) {
        a[i] = this.lenLimit[i][0]
      }
      return a
    },
    paramData: function () {
      var a = []
      for (var i in this.lenLimit) {
        a[i] = new Array(this.lenLimit[i][0]).fill('00')
      }
      return a
    },
    lenLimit: function () {
      var a = []
      for (var i in this.itemInfo.param) {
        a[i] = this.itemInfo.param[i].len()
      }
      return a
    },
    hasParam: function () {
      if (this.itemInfo.param) { return true } else { return false }
    }

  },
  methods: {
    jsCheck () {
      var option = {
        white: true,
        bitwise: true,
        convert: true,
        for: true,
        single: true,
        this: true
      }
      /* workaroud unused arg */
      var result = jslint(
        'function check(writeData,readData){\r\nif((writeData.length===0)||(readData.length===0)){\r\nreturn true;\r\n}\r\n' + this.jsFn + '\r\n}',
        option,
        undefined
      )
      this.jsError = report.error(result)
    },
    addUserParam () {
      this.userParamData.push({
        name: '',
        data: ['00']
      })
    },
    addUserParam1 (index) {
      this.userParamData[index].data.push('00')
    },
    deleteUserParam (index) {
      this.userParamData.splice(index, 1)
    },
    deleteUserParam1 (index) {
      this.userParamData[index].data.pop()
    },
    addService () {
      var item = {}
      item.service = {
        value: this.itemInfo.value,
        name: this.itemInfo.name
      }
      item.param = []
      if (this.hasParam) {
        for (var j in this.itemInfo.param) {
          item.param.push({
            name: this.itemInfo.param[j].name,
            value: JSON.parse(JSON.stringify(this.paramData[j]))
          })
        }
      }
      for (var z in this.userParamData) {
        item.param.push({
          name: this.userParamData[z].name,
          value: JSON.parse(JSON.stringify(this.userParamData[z].data))
        })
      }
      item.func = this.jsFn
      this.$store.commit('doipTableAdd', item)
      this.$emit('addDone')
    },
    dataChange (val) {
      this.paramData[val.index] = val.data
    }
  }
}
</script>
<style>
    .xx{
      text-align: center;
      line-height: 28px;
      vertical-align: middle
    }
    .uds{
        /* margin-right: 50px;
        margin-left: 50px; */
        margin-top: 10px
    }
    .detail{
        border: 1px solid black;
        border-radius: 13px;
        margin: 10px;
        padding: 10px;
    }
    .header1{
      line-height: 40px;
      vertical-align: middle;
      font-size: 18px;
    }
    .file{
      position:absolute;
      top:200px;
      right: 10px;
      z-index: 10;
      border-style: solid;
      margin: 10px;
      border-radius: 20px;
      width:400px;
      text-align: center;
    }
    .file div{
      display: block;
      margin: 10px;
    }
    .fn {
      width: 99%;
      background-color: whitesmoke;
      padding: 3px;
      border-radius: 5px;
      color: darkblue;
      padding-top: 5px;
      padding-bottom: 5px;
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
