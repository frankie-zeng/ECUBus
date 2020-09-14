<template>
  <div class="home">
    <div class="title1">
      <img src="./../assets/logo256.png" />
      <span>ECU-BUS</span>
    </div>

    <el-carousel type="card" height="200px" :autoplay="false" indicatorPosition="none">
      <el-carousel-item>
        <div class="peak">
          <img src="./../assets/PEAK.png" />
          <div class="name">UDS Over CAN/CAN-FD</div>
          <div class="button">
            <i class="el-icon-d-arrow-right" @click="goPEAK"></i>
          </div>
        </div>
      </el-carousel-item>
      <el-carousel-item>
        <div class="doip">
          <img src="./../assets/doip.jpg" />
          <div class="name">UDS Over Ethernet</div>
          <div class="button">
            <i class="el-icon-d-arrow-right" @click="goDOIP"></i>
          </div>
        </div>
      </el-carousel-item>
    </el-carousel>
    <el-divider></el-divider>
    <div class="catalog">UDS ISO-14299</div>
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
            <el-button @click="goPage('/certificate')" type="text">GO</el-button>
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>
<script>
export default {
  data: function() {
    return {
      height: 0,
      width: 0
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
    goPEAK() {
      this.$router.push("/pcan");
    },
    goSECURITY() {
      this.$router.push("/security");
    },
    goPage(url) {
      this.$router.push(url);
    },
    goDOIP() {
      this.$router.push("/doip");
    },
    goEmulate() {
      this.$router.push("/udsEmulate");
    },
    resizeHandle() {
      this.height = window.innerHeight;
    }
  }
};
</script>
<style>
.home {
  font-family: Impact, Haettenschweiler, "Arial Narrow Bold", sans-serif;
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
.doip{
  background-color:white;
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
  margin:5px;
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

.card button {
  float: right;
}
.card .name {
  font-size: 24px;
  color: darkcyan;
}
.card .tip {
  font-family: "Gill Sans", "Gill Sans MT", Calibri, "Trebuchet MS", sans-serif;
  text-align: center;
}
</style>
