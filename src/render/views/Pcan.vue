<template>
  <div>
    <el-dialog title="PEAK TP Setting" :visible.sync="cd" width="80%">
      <div class="connect">
        <PConnect />
      </div>
    </el-dialog>
    <el-dialog title="Add UDS Service" :visible.sync="uds" width="80%">
      <div class="connect">
        <ADDService @additem="uds=false" mode="can" />
      </div>
    </el-dialog>
    <el-dialog title="Combine Multi Service Into Group" :visible.sync="group" width="80%">
      <div class="connect">
        <Group mode="can" @added="group=false"/>
      </div>
    </el-dialog>
    <el-row style="margin:0px">
      <el-col :span="12">
        <el-page-header @back="goBack" content="PCAN UDS" class="header" title=""></el-page-header>
      </el-col>
      <el-col :span="11" class="setup">
        <el-button
          icon="el-icon-setting"
          @click="cd=true"
          class="setup"
          :class="[connected?'el-button--success':'el-button--danger']"
          circle
        ></el-button>
      </el-col>
    </el-row>
    <div style="margin: 10px">
      <el-row style="text-align:right">
        <IE mode="can" />
      </el-row>
      <el-row style="text-align:right">
        <el-button
          icon="el-icon-wallet"
          @click="group=true"
          size="mini"
          type="primary"
        >Save Group</el-button>
        <el-button
          icon="el-icon-plus"
          @click="uds=true"
          size="mini"
          type="primary"
          :disabled="running"
        >Add Service</el-button>
      </el-row>
      <serviceTable mode="can" />
    </div>
    <Excute mode="can" />
  </div>
</template>
<script>
import PConnect from "./../components/connect/PCANConnect.vue";
import ADDService from "./../components/service/addservice.vue";
import Excute from "./../components/excute.vue";
import serviceTable from "./../components/service/serviceTable.vue";
import IE from "./../components/importexport/importexport.vue";
import Group from "./../components/service/groupservice.vue";

export default {
  components: {
    PConnect,
    Excute,
    serviceTable,
    ADDService,
    IE,
    Group
  },
  data: function() {
    return {
      cd: false,
      uds: false,
      group: false
    };
  },
  computed: {
    connected: function() {
      return this.$store.state.canConnect;
    },
    canAddrTable: function() {
      return this.$store.state.canAddrTable;
    },
    canTable: function() {
      return this.$store.state.canTable;
    },
    running: function() {
      return this.$store.state.running;
    }
  },
  methods: {
    goBack() {
      this.$router.push("/");
    }
  }
};
</script>
<style scoped>
.UDSTable {
  font-family: Impact, Haettenschweiler, "Arial Narrow Bold", sans-serif;
  font-size: 20px;
  color: #409eff;
  padding: 10px;
  margin: 10px;
}
.header {
  margin: 30px;
}
.el-aside {
  text-align: center;
}
.connect {
  margin: 30px;
  margin-left: 20px;
  margin-right: 20px;
  margin-top: 5px;
}
.setup button {
  font-size: 20px;
  line-height: 20px;
  text-align: center;
}
.setup {
  line-height: 84px;
  vertical-align: middle;
  text-align: right;
}
.uploadButton span {
  margin: 0px;
}
</style>
