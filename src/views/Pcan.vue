<template>
    <div>
         <el-dialog title="PEAK TP 配置" :visible.sync="cd" width="80%">
                <div class="connect">
                    <PConnect/>
                </div>
            </el-dialog>
        <el-dialog title="添加一个UDS服务" :visible.sync="uds"  width="80%">
                <div class="connect">
                    <UDSService @addDone="uds=false"/>
                </div>
            </el-dialog>
        <el-row style="margin:0px">
            <el-col :span="12">
                 <el-page-header @back="goBack" content="PCAN UDS"  class="header" ></el-page-header>
            </el-col>
            <el-col :span="11"  class="setup">
                <el-button icon="el-icon-setting" @click="cd=true" class="setup" :class="[connected?'el-button--success':'el-button--danger']" circle ></el-button>
            </el-col>
        </el-row>
        <div style="margin: 10px">
        <el-row style="text-align:right">
                <el-button icon="el-icon-folder" @click="importConfig"  size="mini"  type="primary"  :disabled="running">导入配置<form ref="iform" style="display: none" ><input type="file" ref="import"  @change="previewFiles" accept="application/json"/></form></el-button>
                <el-button icon="el-icon-share" @click="exportConfig" size="mini"  type="primary"  :disabled="running">导出配置<input type="file" ref="export" style="display: none"/></el-button>
        </el-row>
        <el-row style="text-align:right">
                <el-button icon="el-icon-plus" @click="uds=true"   size="mini" type="primary"  :disabled="running">添加服务</el-button>
        </el-row>
        <UDS/>
        </div>
        <Excute mode="can"/>
    </div>
</template>
<script>
import PConnect from './../components/PCANConnect.vue'
import UDSService from './../components/UDSService.vue'
import UDS from './../components/UDS.vue'
import Excute from './../components/excute.vue'

const { ipcRenderer } = require('electron')

export default {
  components: {
    PConnect,
    UDSService,
    UDS,
    Excute
  },
  data: function () {
    return {
      cd: false,
      uds: false
    }
  },
  computed: {
    connected: function () {
      return this.$store.state.canConnect
    },
    canTpMapTable: function () {
      return this.$store.state.canTpMapTable
    },
    udsTable: function () {
      return this.$store.state.udsTable
    },
    running: function () {
      return this.$store.state.running
    }
  },
  methods: {
    goBack () {
      this.$router.push('/')
    },
    exportConfig () {
      // this.$refs.export.click()
      var output = {
        'addr': this.canTpMapTable,
        'uds': this.udsTable
      }

      var element = document.createElement('a')
      element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(JSON.stringify(output, null, 2)))
      element.setAttribute('download', 'config.json')
      element.style.display = 'none'
      document.body.appendChild(element)
      element.click()
      document.body.removeChild(element)
    },
    importConfig () {
      if (!this.connected) {
        this.$message.error('请先连接设备')
        return
      }
      this.$refs.import.click()
    },
    previewFiles () {
      var filePath = event.target.files[0].path
      this.$refs.iform.reset()
      var data = JSON.parse(ipcRenderer.sendSync('openFile', filePath).toString())
      var err
      for (var i in data.addr) {
        if (data.addr[i].maped) {
          err = ipcRenderer.sendSync('can-addMap', data.addr[i])
          if (err.err !== 0) {
            this.$alert(err.msg, '导入失败', {
              confirmButtonText: '确定'
            })
            return
          }
        }
      }
      this.$store.commit('canTpMapLoad', data.addr)
      this.$store.commit('udsTableLoad', data.uds)
    }
  }
}
</script>
<style scoped>
    .header{
        margin: 30px;
    }
    .el-aside {
        text-align: center;
    }
    .connect{
        margin: 30px;
        margin-left: 20px;
        margin-right: 20px;
        margin-top: 5px;
    }
    .setup button{
        font-size: 20px;
        line-height: 20px;
        text-align: center
    }
    .setup {
        line-height: 84px;
        vertical-align: middle;
        text-align: right
    }
    .uploadButton span{
      margin: 0px;
    }

</style>
