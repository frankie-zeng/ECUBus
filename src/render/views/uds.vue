<template>
  <div>
      <el-page-header
          @back="goBack"
          content="UDS"
          class="header"
          title=""
        ></el-page-header>
     <div class="card">
      <el-row>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoCAN ISO-15765-2</div>
            <div class="tip">UDS Over CAN/CAN-FD</div>
            <el-button @click="goPEAK" type="text">GO</el-button>
          </el-card>
        </el-col>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoIP ISO-13400-2</div>
            <div class="tip">UDS Over Ethernet</div>
            <el-button @click="goDOIP" type="text">GO</el-button>
          </el-card>
        </el-col>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">UDS Loopback</div>
            <div class="tip">UDS Loopback Test</div>
            <el-button @click="goEmulate" type="text">GO</el-button>
          </el-card>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoLIN ISO-17987-2</div>
            <div class="tip">UDS Over LIN</div>
            <el-button @click="goLIN" type="text">GO</el-button>
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {};
  },
  methods: {
    goPEAK() {
      this.$router.push("/uds/pcan");
    },
    goLIN() {
      this.$router.push("/uds/lin");
    },
    goDOIP() {
      this.$router.push("/uds/doip");
    },
    goEmulate() {
      this.$router.push("/uds/udsEmulate");
    },
    goBack() {
      this.$router.push("/");
    },
  },
};
</script>

<style>
</style>