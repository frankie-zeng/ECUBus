<template>
  <div>
    <div>
      <el-dialog title="Address" :visible.sync="showAddr" center width="30%">
        <el-select v-model="addrIndex" placeholder="Address" style="width:100%" size="mini">
          <el-option v-for="(item,key) in addrTable" :key="key" :label="item.name" :value="key">
            <span style="float: left">{{ item.name }}</span>
            <span
              style="float: right; color: #8492a6; font-size: 13px"
            >SA:{{ item.SA}},TA:{{item.TA}}</span>
          </el-option>
        </el-select>
        <span slot="footer">
          <el-button icon="el-icon-caret-right" @click="readRun" size="mini" type="primary">Start</el-button>
        </span>
      </el-dialog>
    </div>
    <el-row style="margin-top:10px;">
      <el-col :span="22" :offset="1" style="text-align:right">
        Suppress Delay:
        <el-input
          v-model="sDelay"
          size="small"
          placeholder="Delay"
          style="width:80px"
          maxlength="6"
        ></el-input>ms,
        Timeout:
        <el-input
          v-model="udsTimeout"
          size="small"
          placeholder="Timeout"
          style="width:80px"
          maxlength="6"
          :disabled="mode==='lp'"
        ></el-input>ms
        <!-- <el-button @click="run" size="small" type="success" :disabled="!connected||running">开始</el-button> -->
        <el-button @click="run" size="small" type="success" :disabled="!connected||running">Start</el-button>
      </el-col>
    </el-row>
    <el-input readonly type="textarea" :rows="5" placeholder="LOG" v-model="logText"></el-input>
  </div>
</template>
<script>
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      udsTimeout: "2000",
      sDelay: "100",
      logText: "",
      showAddr: false,
      addrIndex: ""
    };
  },
  mounted() {
    ipcRenderer.on("udsEnd", (event, val) => {
      this.success(val);
    });
    ipcRenderer.on("udsData", (event, val) => {
      this.logText += val;
    });
    ipcRenderer.on("udsError", (event, val) => {
      this.failed(val.msg);
      this.$store.commit("setTableError",val.index)
    });
  },
  destroyed() {
    ipcRenderer.removeAllListeners("udsEnd");
    ipcRenderer.removeAllListeners("udsData");
    ipcRenderer.removeAllListeners("udsError");
  },
  props: {
    mode: {
      type: String,
      default: function() {
        return "can";
      }
    }
  },
  computed: {
    connected: function() {
      if (this.mode === "can") {
        return this.$store.state.canConnect;
      } else if (this.mode === "doip") {
        return this.$store.state.doipConnect;
      } else if (this.mode === "lp") {
        return true;
      } else {
        return false;
      }
    },
    udsTable: function() {
      if (this.mode === "can") {
        return this.$store.state.canTable;
      } else if (this.mode === "doip") {
        return this.$store.state.doipTable;
      } else if (this.mode === "lp") {
        return this.$store.state.lpTable;
      } else {
        return [];
      }
    },
    running: function() {
      return this.$store.state.running;
    },
    addrTable: function() {
      if (this.mode === "doip") {
        return this.$store.state.doipAddrTable;
      } else if (this.mode === "can") {
        return this.$store.state.canAddrTable;
      } else if (this.mode === "lp") {
        return this.$store.state.lpAddrTable;
      } else {
        return [];
      }
    }
  },
  methods: {
    failed(msg) {
      this.logText += msg;
      this.$store.commit("runChange", false);
      this.$notify.error({
        title: "Error",
        message: msg
      });
    },
    success(msg) {
      this.logText += msg;
      this.$store.commit("runChange", false);
      this.$notify({
        title: "Success",
        message: msg,
        type: "success"
      });
    },
    readRun() {
      var table = JSON.parse(JSON.stringify(this.udsTable));
      if (this.addrTable[this.addrIndex]) {
        this.showAddr = false;
      } else {
        this.$notify.error({
          title: "Error",
          message: "Please choose correct address."
        });
        return;
      }
      this.$store.commit("runChange", true);
      this.$store.commit("setTableError",-1);
      this.logText = "";
      
      ipcRenderer.send(this.mode + "udsExcute", {
        addr: this.addrTable[this.addrIndex],
        udsTable: table,
        timeout: parseInt(this.udsTimeout, 10),
        sDelay: parseInt(this.sDelay, 10)
      });
    },
    run() {
      this.showAddr = true;
      // console.log(this.udsTable)
    }
  }
};
</script>
