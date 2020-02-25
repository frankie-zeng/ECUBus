<template>
    <div>
        <el-row style="margin-top:10px;">
            <el-col :span="22" :offset="1" style="text-align:right">
                Suppress Delay: <el-input v-model="sDelay" size="small" placeholder="请输入内容" style="width:80px"  maxlength="6"></el-input> ms,

                Timeout: <el-input v-model="udsTimeout" size="small" placeholder="请输入内容" style="width:80px"  maxlength="6"></el-input> ms
                 <el-button @click="run" size="small"  type="success" :disabled="!connected||running">开始</el-button>
            </el-col>
        </el-row>
        <el-input
          readonly
          type="textarea"
          :rows="5"
          placeholder="请输入内容"
          v-model="logText">
        </el-input>
    </div>
</template>
<script>
const { ipcRenderer } = require('electron')
export default {
  data () {
    return {
      udsTimeout: '2000',
      sDelay: '100',
      logText: ''
    }
  },
  mounted () {
    ipcRenderer.on('udsEnd', (event, val) => {
      this.success(val)
    })
    ipcRenderer.on('udsData', (event, val) => {
      this.logText+=val
    })
    ipcRenderer.on('udsError', (event, val) => {
      this.failed(val)
    })
  },
  destroyed () {
    ipcRenderer.removeAllListeners('udsEnd')
    ipcRenderer.removeAllListeners('udsData')
    ipcRenderer.removeAllListeners('udsError')
  },
  props: {
    mode: {
      type: String,
      default: function () {
        return 'can'
      }
    }
  },
  computed: {
    connected: function () {
      if(this.mode==='can'){
        return this.$store.state.canConnect
      }else if(this.mode==='doip'){
        return this.$store.state.doipConnect
      }else{
        return false
      }
    },
    udsTable: function () {
      if(this.mode==='can'){
        return this.$store.state.canTable
      }else if(this.mode==='doip'){
        return this.$store.state.doipTable
      }else{
        return []
      }
    },
    running: function () {
      return this.$store.state.running
    }
  },
  methods: {
    failed (msg) {
      this.logText+=msg
      this.$store.commit('runChange', false)
      this.$notify.error({
        title: 'Error',
        message: msg
      })
    },
    success(msg){
      this.logText+=msg
      this.$store.commit('runChange', false)
      this.$notify({
        title: 'Success',
        message: msg,
        type: 'success'
      });
    },
    run () {
      // console.log(this.udsTable)
      this.$store.commit('runChange', true)
      this.logText = ''
      ipcRenderer.send(this.mode + 'udsExcute', {
        udsTable: this.udsTable,
        timeout: parseInt(this.udsTimeout, 10),
        sDelay: parseInt(this.sDelay, 10) 
      })
    }
  }

}
</script>
