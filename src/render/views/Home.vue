<template>
  <div class="home">
    <div class="title1">
      <img src="./../assets/logo256.png" />
      <span>ECU-BUS</span>
    </div>
    <div class="catalog">UDS ISO-14299</div>
    <div class="card uds">
      <el-card shadow="always">
        <div class="name">Powerful UDS Tester</div>
        <div class="tip">CAN/CANFD</div>
        <div class="tip">Ethernet</div>
        <div class="tip">Lin</div>
        <div class="tip">Simulate</div>
        <el-button @click="goUDS" type="text">GO</el-button>
      </el-card>
    </div>
    <div class="catalog">SECURITY</div>
    <div class="card">
      <el-row>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">SHE KEY GENERATE</div>
            <div class="tip">S32K1-CSEC | S32K3-HSE</div>
            <el-button @click="goSECURITY" type="text">GO</el-button>
          </el-card>
        </el-col>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">Certificate Tool</div>
            <div class="tip">S32K3-HSE</div>
            <el-button @click="goPage('/certificate')" type="text"
              >GO</el-button
            >
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>
<script>
export default {
  data: function () {
    return {
      height: 0,
      width: 0,
    };
  },
  mounted() {
    this.height = window.innerHeight;
    this.width = window.innerWidth;
    window.addEventListener("resize", this.resizeHandle);
  },
  destroyed() {
    window.removeEventListener("resize", this.resizeHandle);
  },
  methods: {
    goSECURITY() {
      this.$router.push("/security");
    },
    goPage(url) {
      this.$router.push(url);
    },
    goUDS() {
      this.$router.push("/uds");
    },
    resizeHandle() {
      this.height = window.innerHeight;
    },
  },
};
</script>
<style>
.home {
  font-family: "Helvetica Neue",Helvetica,"PingFang SC","Hiragino Sans GB","Microsoft YaHei","微软雅黑",Arial,sans-serif;
}
.catalog {
  font-size: 20px;
  margin: 20px;
  color: gray;
}
.title1 {
  height: 48px;
  line-height: 48px;
  margin: 10px;
}
.title1 span {
  position: absolute;
  margin-left: 5px;
  font-size: 32px;
}
.title1 img {
  height: 48px;
}
.doip {
  background-color: white;
  height: 98%;
  border-radius: 10px;
  border-style: solid;
}
.doip .name {
  color: black;
  font-size: 36px;
  text-align: center;
  margin: 10px;
}
.doip .button {
  position: absolute;
  color: black;
  font-size: 25px;
  margin: 10px;
  right: 0px;
  bottom: 0px;
}
.doip img {
  margin: 5px;
  width: 100px;
}
.peak {
  background-color: black;
  height: 100%;
  border-radius: 10px;
}
.peak img {
  margin-left: 10px;
  margin-top: 5px;
}
.peak .name {
  color: white;
  font-size: 36px;
  text-align: center;
  margin: 20px;
}
.peak .button {
  position: absolute;
  color: white;
  font-size: 25px;
  margin: 10px;
  right: 0px;
  bottom: 0px;
}
.lin {
  background-color: rgb(135, 162, 235);
  height: 100%;
  border-radius: 10px;
}
.lin img {
  margin-left: 10px;
  margin-top: 5px;
}
.lin .name {
  color: black;
  font-size: 36px;
  text-align: center;
  margin: 20px;
}
.lin .button {
  position: absolute;
  color: white;
  font-size: 25px;
  margin: 10px;
  right: 0px;
  bottom: 0px;
}
.card button {
  float: right;
}
.card .name {
  font-size: 28px;
  color: #409EFF;
}
.card .tip {
  font-family: "Gill Sans", "Gill Sans MT", Calibri, "Trebuchet MS", sans-serif;
  text-align: center;
}

.uds {
  margin-left: 4%;
  margin-right: 4%;
  
}
</style>
