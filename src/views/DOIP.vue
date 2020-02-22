<template>
    <div>
         <el-dialog title="DOIP 配置" :visible.sync="cd" width="80%">
                <div class="connect">
                    <DOIPConnect/>
                </div>
            </el-dialog>
          <el-dialog title="添加一个UDS服务" :visible.sync="uds"  width="80%">
            <div class="connect">
                <ADDService @additem="uds=false" mode="doip"/>
            </div>
          </el-dialog>
        <el-row style="margin:0px">
            <el-col :span="12">
                 <el-page-header @back="goBack" content="DOIP UDS"  class="header" ></el-page-header>
            </el-col>
            <el-col :span="11"  class="setup">
                <el-button icon="el-icon-setting" @click="cd=true" :class="[connected?'el-button--success':'el-button--danger']"  circle ></el-button>
            </el-col>
        </el-row>
        <div style="margin: 10px">
        <el-row style="text-align:right">
                <IE mode="doip"/>
        </el-row>
        <el-row style="text-align:right">
                <el-button icon="el-icon-plus" @click="uds=true"   size="mini" type="primary"  :disabled="running">添加服务</el-button>
        </el-row>
       
        <serviceTable mode="doip"/>
        <Excute mode="doip"/>
        </div>
        
    </div>
</template>
<script>
import DOIPConnect from './../components/connect/DOIPConnect.vue'
import ADDService from './../components/service/addservice.vue'
import Excute from './../components/excute.vue'
import serviceTable from './../components/service/serviceTable.vue'
import IE from './../components/importexport/importexport.vue'

// const { ipcRenderer } = require('electron')

export default {
  components: {
    DOIPConnect,
    Excute,
    serviceTable,
    ADDService,
    IE
  },
  data: function () {
    return {
      cd: false,
      uds: false,
      doip: false,
    }
  },
  computed: {
    doipAddrTable: function () {
      return this.$store.state.doipAddrTable
    },
    doipTable: function () {
      return this.$store.state.doipTable
    },
    running: function () {
      return this.$store.state.running
    },
    connected: function () {
      return this.$store.state.doipConnect
    }
  },
  methods: {
    goBack () {
      this.$router.push('/')
    },
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
