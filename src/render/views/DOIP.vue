<template>
  <div>
    <el-dialog title="DOIP Setting" :visible.sync="cd" width="80%">
      <div class="connect">
        <DOIPConnect />
      </div>
    </el-dialog>
    <el-row style="margin: 0px">
      <el-col :span="12">
        <el-page-header
          @back="goBack"
          content="DOIP UDS"
          class="header"
          title=""
        ></el-page-header>
      </el-col>
      <el-col :span="11" class="setup">
        <el-button
          icon="el-icon-setting"
          @click="cd = true"
          :class="[connected ? 'el-button--success' : 'el-button--danger']"
          circle
        ></el-button>
      </el-col>
    </el-row>
    <div style="margin: 10px">
      <el-row style="text-align: right">
        <IE mode="doip" @imported="importCb" />
      </el-row>
      <serviceTable mode="doip" ref="serviceTable" />
      <Excute mode="doip" />
    </div>
  </div>
</template>
<script>
import DOIPConnect from "./../components/connect/DOIPConnect.vue";
import Excute from "./../components/excute.vue";
import serviceTable from "./../components/service/serviceTable.vue";
import IE from "./../components/importexport/importexport.vue";

// const { ipcRenderer } = require('electron')

export default {
  components: {
    DOIPConnect,
    Excute,
    serviceTable,
    IE,
  },
  data: function () {
    return {
      cd: false,
      uds: false,
      group: false,
    };
  },
  computed: {
    running: function () {
      return this.$store.state.running;
    },
    connected: function () {
      return this.$store.state.doipConnect;
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
