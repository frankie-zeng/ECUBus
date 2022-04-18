<template>
  <div>
    <el-dialog title="LIN TP Setting" :visible.sync="cd" width="80%">
      <div class="connect">
        <PLINConnect />
      </div>
    </el-dialog>
    <el-row style="margin: 0px">
      <el-col :span="12">
        <el-page-header
          @back="goBack"
          content="LIN UDS"
          class="header"
          title=""
        ></el-page-header>
      </el-col>
      <el-col :span="11" class="setup">
        <el-button
          icon="el-icon-setting"
          @click="cd = true"
          class="setup"
          :class="[connected ? 'el-button--success' : 'el-button--danger']"
          circle
        ></el-button>
      </el-col>
    </el-row>
    <div style="margin: 10px">
      <!-- <el-row style="text-align: right">
        <IE mode="lin" @imported="importCb" />
      </el-row> -->
      <serviceTable mode="lin" ref="serviceTable" />
    </div>
    <Excute mode="lin" />
  </div>
</template>
<script>
import PLINConnect from "./../components/connect/PLINConnect.vue";
import Excute from "./../components/excute.vue";
import serviceTable from "./../components/service/serviceTable.vue";
// import IE from "./../components/importexport/importexport.vue";

export default {
  components: {
    PLINConnect,
    Excute,
    serviceTable,
    // IE,
  },
  data: function () {
    return {
      cd: false,
      uds: false,
      group: false,
    };
  },
  computed: {
    connected: function () {
      return this.$store.state.linConnect;
    },
    linAddrTable: function () {
      return this.$store.state.linAddrTable;
    },
    linTable: function () {
      return this.$store.state.linTable;
    },
    running: function () {
      return this.$store.state.running;
    },
  },
  methods: {
    importCb() {
      this.$refs.serviceTable.update();
    },
    goBack() {
      this.$router.push("/uds");
    },
  },
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
