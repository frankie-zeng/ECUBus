<template>
  <div>
    <el-row style="margin:0px">
      <el-col :span="12">
        <el-page-header @back="goBack" content="UDS Loopback" class="header" title></el-page-header>
      </el-col>
      <el-col :span="11" class="setup">
        <el-button icon="el-icon-setting" type="success" circle></el-button>
      </el-col>
    </el-row>
    <div style="margin: 10px">
      <el-row style="text-align:right">
        <IE mode="lp" @imported="importCb"/>
      </el-row>
      <el-row>
        <el-col :span="16">
           <serviceTable mode="lp" ref="serviceTable"/>
          <Excute mode="lp"/>
        </el-col>
        <el-col :span="8" class="lpwin">
          <h2 style="margin:10px">Interaction:</h2>
          <el-button icon="el-icon-circle-close" size="mini" class="clearBtn" @click="output=[]"></el-button>
          <div style="margin:10px">
            <div class="lp" id="lp">
              <div v-for="(val,key) in output" :key="key">
                <div v-if="val.dir">
                  <el-avatar size="small" style="vertical-align: middle;">
                    <img src="./../assets/logo28.png" />
                  </el-avatar>
                  <span class="msg1">{{val.msg}}</span>
                </div>
                <div v-else style="text-align:right">
                  <span class="msg2">{{val.msg}}</span>
                  <el-avatar icon="el-icon-user-solid" size="small" style="vertical-align: middle;"></el-avatar>
                </div>
              </div>
            </div>
            <el-input
              placeholder="must hex,(55,3a,7f)"
              v-model="input"
              @keyup.enter.native="doInput"
              :disabled="!running"
            >
              <el-button
                slot="append"
                icon="el-icon-arrow-right"
                @click="doInput"
                :disabled="!running"
              ></el-button>
            </el-input>
          </div>
        </el-col>
      </el-row>
    </div>
  </div>
</template>
<script>
import Excute from "./../components/excute.vue";
import serviceTable from "./../components/service/serviceTable.vue";
import IE from "./../components/importexport/importexport.vue";

const { ipcRenderer } = require("electron");

export default {
  components: {
    Excute,
    serviceTable,
    IE,
  },
  data: function() {
    return {
      cd: false,
      uds: false,
      group: false,
      input: "",
      output: []
    };
  },
  mounted() {
    ipcRenderer.on("lpSend", (event, val) => {
      this.output.push({
        msg: this.array2hex(val),
        dir: true
      });
      this.scrollBottom();
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("lpSend");
  },
  computed: {
    running: function() {
      return this.$store.state.running;
    }
  },
  methods: {
    importCb(){
      this.$refs.serviceTable.update();
    },
    array2hex(val) {
      return Array.from(val, function(byte) {
        return ("0" + (byte & 0xff).toString(16)).slice(-2);
      }).join(",");
    },
    goBack() {
      this.$router.push("/uds");
    },
    scrollBottom() {
      this.$nextTick(() => {
        let div = document.getElementById("lp");
        div.scrollTop = div.scrollHeight;
      });
    },
    doInput() {
      if (this.input != "") {
        let reg = /^([0-9a-fA-F]{1,2})?(,[0-9a-fA-F]{1,2})*$/;
        if (reg.test(this.input)) {
          let val = Array.from(this.input.split(","), function(byte) {
            return parseInt(byte, 16);
          });
          this.output.push({
            msg: this.array2hex(val),
            dir: false
          });
          this.scrollBottom();

          ipcRenderer.send("lpReceive", val);
          this.input = "";
        } else {
          this.$notify.error({
            title: "Error",
            message: "Please Input Right Format,ex: 55,aa,fe"
          });
        }
      }
    }
  }
};
</script>
<style scoped>
.lpwin{
  border-radius: 5px;
    border-style:solid;
    border-width:3px;
    padding:1px;
    border-color:whitesmoke;
}
.clearBtn {
  position: absolute;
  right: 10px;
  top: 10px;
}
.msg2 {
  max-width: 70%;
  word-wrap:break-word; 
  text-align: left;
  background-color: rgb(195, 252, 89);
  line-height: 20px;
  margin: 5px;
  display: inline-block;
  padding: 10px;
  border-radius: 5px;
  vertical-align: middle;
}
.msg1 {
  max-width: 70%;
  word-wrap:break-word; 
  /* word-break:break-all;  */
  background-color: whitesmoke;
  line-height: 20px;
  /* height: 15px; */
  margin: 5px;
  display: inline-block;
  padding: 10px;
  border-radius: 5px;
  vertical-align: middle;
}
.lp {
  max-height: 500px;
  min-height: 500px;
  overflow: auto;
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
