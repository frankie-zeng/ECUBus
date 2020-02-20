<template>
    <div class="uds">
        <div >
          <div class="file" v-if="itemInfo.file=='upload'">
            <div>
              <el-button size="small" type="info" @click="saveFile">选择保存文件</el-button>
              <div class="el-upload__tip">选择文件后,TransferData(0x36)和RequestTransferExit(0x37)服务由系统自动计算和添加,
                  成功执行完0x37服务后，会生成对应的文件。
                </div>
              <div>{{filePath}}</div>
            </div>
          </div>
          <div class="file" v-if="itemInfo.file=='download'">
            <div>
              文件大小：0x{{fileSize.toString(16)}}
              <el-upload
                 :auto-upload="false"
                :limit="2"
                action="https://jsonplaceholder.typicode.com/posts/"
                :file-list="fileList"
                :on-change="exceedFile"
                :on-remove="removeFile">
                <el-button size="small" type="info">点击上传文件</el-button>
                <div slot="tip" class="el-upload__tip">上传文件后，会自动计算文件大小,TransferData(0x36)和RequestTransferExit(0x37)服务由系统自动计算和添加,
                  MemorySize和addressAndLengthFormatIdentifier(7:4)会根据文件实际大小在点击添加服务后进行调整覆盖。
                </div>
              </el-upload>
            </div>
            </div>

            <el-row style="text-align:left">
                服务名字：
                    <el-select v-model="service" placeholder="Choose Service"  filterable style="width:300px">
                        <el-option
                        v-for="(item, key) in UDSConfig"
                        :key="item.value"
                        :label="item.name"
                        :value="item.value"
                        :disabled="item.disabled">
                          <span style="float: left">{{ item.name }}</span>
                          <span style="float: right; color: #8492a6; font-size: 13px">0x{{ key }}</span>
                        </el-option>
                    </el-select>
            </el-row>
              <el-row>
                地址名字：
                <el-select v-model="addrIndex" placeholder="请选择">
                    <el-option
                    v-for="(item, index) in tpTable"
                    :key="index"
                    :value="index"
                    :label="item.name">
                    </el-option>
                </el-select>
              </el-row>
              <div v-if="hasSub">
                <el-row>
                  自定义SubFunction：
                  <el-switch
                    v-model="suboption" @change="switchChange">
                  </el-switch>
                </el-row>
                  {{itemInfo.subFunction.name}}：
                  <el-select v-model="subFunc" placeholder="请选择" v-if="!suboption">
                      <el-option
                      v-for="item in itemInfo.subFunction.options"
                      :key="item.value"
                      :value="item.value"
                      :label="item.name">
                      </el-option>
                  </el-select>
                  <el-input v-model="subFunc" style="width:65px" v-else maxlength="2" show-word-limit></el-input>
                  <span> - </span>
                  <el-checkbox v-model="suppress" label="Suppress" border > </el-checkbox>
              </div>
              <el-row v-if="hasParam">
                <div v-for="(item,index) in itemInfo.param"
                  :key="index">
                  <el-tag style="margin-bottom:10px;margin-top:10px">参数{{index+1}} {{item.name}}:</el-tag>
                  <Input :minLen="lenLimit[index][0]" :maxLen="lenLimit[index][1]" :index="index" @change="dataChange"/>
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
import UDSConfig from './uds.js'
import Input from './input.vue'
import jslint from './../JSLint/jslint.js'
import report from './../JSLint/report.js'
const { ipcRenderer } = require('electron')
export default {
  data () {
    return {
      UDSConfig: UDSConfig,
      addrIndex: '',
      activeNames: ['1'],
      service: 0x10,
      subFunc: '',
      userParamData: [],
      suppress: false,
      suboption: false,
      fileList: [],
      filePath: '',
      fileSize: 0,
      jsFn: 'return true;',
      jsError: ''
      // hasSub: false,
      // hasParam: false
      // paramLen: [],
      // paramData: []
    }
  },
  props:['mode'],
  components: {
    Input
  },
  watch: {
    itemInfo: function () {
      this.subFunc = ''
      this.userParamData = []
      this.suboption = false
      this.jsFn = 'return true;'
      this.jsError = ''
    }
  },
  computed: {
    connect: function () {
      return this.$store.state.canConnect
    },
    tpTable: function () {
      if(this.mode==='can'){
        return this.$store.state.canTpMapTable
      }
      else if(this.mode==='doip'){
        return this.$store.state.doipAddrTable
      }
      else{
        return []
      }
    },
    itemInfo: function () {
      return UDSConfig[this.service.toString(16)]
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
        a[i] = this.itemInfo.param[i].len(this.subFunc)
      }
      return a
    },
    hasSub: function () {
      if (this.itemInfo.subFunction) { return true } else { return false }
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
    saveFile () {
      this.filePath = ipcRenderer.sendSync('saveFile')
    },
    // eslint-disable-next-line no-unused-vars
    removeFile (files, fileList) {
      this.fileList = []
      this.filePath = ''
      this.fileSize = 0
    },
    exceedFile (files, fileList) {
      this.fileList = fileList.slice(-1)
      this.filePath = this.fileList[0].raw.path
      this.fileSize = this.fileList[0].size
    },
    switchChange () {
      this.subFunc = ''
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
      if (this.hasSub) {
        if (this.subFunc === '') {
          this.$message.error('请选择正确的SubFunction')
          return
        }
      }
      if (!this.tpTable[this.addrIndex]) {
        this.$message.error('请选择正确的地址')
        this.addrIndex = ''
        return
      }
      var item = {}
      item.addr = this.tpTable[this.addrIndex]
      item.service = {
        value: this.itemInfo.value,
        name: this.itemInfo.name
      }
      if (this.itemInfo.file === 'download') {
        item.other = {
          path: this.filePath,
          size: this.fileSize
        }
      }
      if (this.itemInfo.file === 'upload') {
        item.other = {
          path: this.filePath,
          size: parseInt(this.paramData[3].join().split(',').join(''), 16)
        }
      }
      if (this.hasSub) {
        var sfn = 'UserDefine'
        for (var i in this.itemInfo.subFunction.options) {
          if (this.itemInfo.subFunction.options[i].value === this.subFunc) {
            sfn = this.itemInfo.subFunction.options[i].name
            break
          }
        }
        item.subFunc = {
          name: sfn,
          value: parseInt(this.subFunc, 16),
          suppress: this.suppress
        }
      }
      item.param = []
      if (this.hasParam) {
        for (var j in this.itemInfo.param) {
          item.param.push({
            name: this.itemInfo.param[j].name,
            value: JSON.parse(JSON.stringify(this.paramData[j]))
          })
        }

        // if ((this.itemInfo.file === 'download') && (this.fileList.length > 0)) {
        //   var size = this.fileSize.toString(16)
        //   var len = parseInt((size.length + 1) / 2)
        //   var mLen = len > item.param[3].value.length ? len : item.param[3].value.length

        //   for (i = 0; i < len; i++) {
        //     if (i === 0) {
        //       item.param[3].value[mLen - 1] = size.slice(-2)
        //     } else {
        //       item.param[3].value[mLen - 1 - i] = size.slice(-2 * (i + 1), -2 * i)
        //     }
        //   }
        //   var rawLen = parseInt(item.param[1].value[0], 16)
        //   if (((rawLen & 0xf0) >> 4) < len) {
        //     item.param[1].value[0] = ((rawLen & 0x0f) + (len * 16)).toString(16)
        //   }
        // }
      }
      for (var z in this.userParamData) {
        item.param.push({
          name: this.userParamData[z].name,
          value: JSON.parse(JSON.stringify(this.userParamData[z].data))
        })
      }
      item.func = this.jsFn
      this.$store.commit('udsTableAdd', item)
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
