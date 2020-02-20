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
    ipcRenderer.on('udsEvent', (event, val) => {
      if (val.err === 1) {
        this.$store.commit('runChange', false)
        this.$notify({
          title: 'Success',
          message: 'Done!',
          type: 'success'
        })
      }
      if (val.err < 0) {
        this.$store.commit('runChange', false)
        this.$notify.error({
          title: 'Error',
          message: 'Excute failed,see log window for error'
        })
      }
      this.logText += val.msg
    })
  },
  destroyed () {
    ipcRenderer.removeAllListeners('udsEvent')
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
      }else{
        return true
      }
    },
    udsTable: function () {
      return this.$store.state.udsTable
    },
    basicTable: function (){
      return this.$store.state.doipTable
    },
    running: function () {
      return this.$store.state.running
    }
  },
  methods: {
    failed () {
      this.$store.commit('runChange', false)
      this.$notify.error({
        title: 'Error',
        message: 'Excute failed'
      })
    },
    run () {
      this.$store.commit('runChange', true)
      this.logText = ''
      if(this.mode==='can'){
        ipcRenderer.send(this.mode + 'udsExcute', {
          udsTable: this.udsTable,
          timeout: parseInt(this.udsTimeout, 10),
          sDelay: parseInt(this.sDelay, 10) })
      }else if(this.mode==='doip'){
        ipcRenderer.send(this.mode + 'udsExcute', {
          udsTable: this.udsTable,
          basicTable: this.basicTable,
          timeout: parseInt(this.udsTimeout, 10),
          sDelay: parseInt(this.sDelay, 10) })
      }
    }
  }

}
</script>
