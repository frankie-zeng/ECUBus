
<template>
  <div>
    <el-row style="margin-top: 10px">
      <el-col :span="22" :offset="1" style="text-align: right">
        Log Level:
        <el-select
          v-model="logLevel"
          placeholder="Log Level"
          size="small"
          style="width: 100px"
          @change="logLevelChange"
        >
          <el-option value="silly" />
          <el-option value="debug" />
          <el-option value="verbose" />
          <el-option value="info" />
          <el-option value="warn" />
          <el-option value="error" />
          <el-option value="false" /> </el-select
        >Suppress Delay:
        <el-input
          v-model="sDelay"
          size="small"
          placeholder="Delay"
          style="width: 80px"
          maxlength="6"
        ></el-input
        >ms, Timeout:
        <el-input
          v-model="udsTimeout"
          size="small"
          placeholder="Timeout"
          style="width: 80px"
          maxlength="6"
          :disabled="mode === 'lp'"
        ></el-input
        >ms, Cycle:
        <el-input-number
          v-model.number="cycle"
          controls-position="right"
          :min="1"
          size="small"
          style="width: 100px"
        ></el-input-number>
        <!-- <el-button @click="run" size="small" type="success" :disabled="!connected||running">开始</el-button> -->
      </el-col>
    </el-row>
    <Progress/>
    <el-row style="margin-top: 10px">
      <el-col :span="22" :offset="1" style="text-align: right">
        <el-button
          @click="run"
          size="small"
          type="success"
          :disabled="!connected || running"
          >Start</el-button
        >
      </el-col>
    </el-row>

    <div id="terminal" class="logWindow"></div>
  </div>
</template>
<script>
/* eslint-disable no-unused-vars */
import Progress from './progress.vue'
const { Terminal } = require("xterm");
const { FitAddon } = require("xterm-addon-fit");
const { ipcRenderer } = require("electron");
const log = require("electron-log");
const util = require("util");
const fitAddon = new FitAddon();
function resizeTerminal() {
  if (window.innerWidth % 1 === 0 && window.innerHeight % 1 === 0) {
    fitAddon.fit();
  }
}
export default {
  data() {
    return {
      cycle: 1,
      interCycle: 1,
      startTime: "",
      udsTimeout: "5000",
      sDelay: "100",
      logLevel: "info",
      terminal: "",
      schIndex: 0,
    };
  },
  components:{
    Progress
  },
  mounted() {
    this.terminal = new Terminal({
      theme: {
        background: "#ffffff",
        selection: "gray",
      },
      fontWeight: "bold",
    });
    this.terminal.loadAddon(fitAddon);
    this.terminal.setOption("disableStdin", true);
    this.terminal.open(document.getElementById("terminal"));
    this.terminal.attachCustomKeyEventHandler((e) => {
      return false;
    });
    fitAddon.fit();
    window.addEventListener("resize", resizeTerminal);
    this.logLevel = this.$store.state.logLevel;
    ipcRenderer.on("udsEnd", (event, val) => {
      if (!this.schRun()) {
        if (this.interCycle > 1) {
          this.interCycle--;
          this.terminal.write(
            `\x1B[1;;33mStart again:${this.interCycle}\x1B[0m\r\n`
          );
          /* restart a new sch*/
          this.schIndex = 0;
          this.schRun();
        } else {
          this.success(val);
        }
      }
    });
    ipcRenderer.on(log.transports.ipc.eventId, (event, message) => {
      const text = util.format.apply(util, message.data);
      let msg;
      let usedTime = Date.parse(message.date) - this.startTime;
      if (usedTime < 0) {
        usedTime = 0;
      }
      if (message.level == "info") {
        msg = `\x1B[1;;32m[${usedTime}ms][${
          this.udsTable[this.schIndex - 1].name
        }]${text}\x1B[0m\r\n`;
      } else if (message.level == "error") {
        msg = `\x1B[1;;31m[${usedTime}ms][${
          this.udsTable[this.schIndex - 1].name
        }]${text}\x1B[0m\r\n`;
      } else if (message.level == "warn") {
        msg = `\x1B[1;;33m[${usedTime}ms][${
          this.udsTable[this.schIndex - 1].name
        }]${text}\x1B[0m\r\n`;
      } else {
        msg = `\x1B[1;;30m[${usedTime}ms][${
          this.udsTable[this.schIndex - 1].name
        }]${text}\x1B[0m\r\n`;
      }
      this.terminal.write(msg);
    });
    ipcRenderer.on("udsError", (event, val) => {
      this.failed(val.msg);
      this.$store.commit("setTableError", [this.schIndex - 1, val.index]);
    });
  },
  destroyed() {
    window.removeEventListener("resize", resizeTerminal);
    ipcRenderer.removeAllListeners("udsEnd");
    ipcRenderer.removeAllListeners(log.transports.ipc.eventId);
    ipcRenderer.removeAllListeners("udsError");
  },
  props: {
    mode: {
      type: String,
      default: function () {
        return "can";
      },
    },
  },
  computed: {
    connected: function () {
      if (this.mode === "lp") {
        return true;
      } else {
        return this.$store.state[this.mode + "Connect"];
      }
    },
    udsTable: function () {
      return this.$store.state[this.mode + "Table"];
    },
    running: function () {
      return this.$store.state.running;
    },
    addrTable: function () {
      return this.$store.state[this.mode + "AddrTable"];
    },
  },
  methods: {
    logLevelChange() {
      ipcRenderer.send("logLevel", this.logLevel);
      this.$store.commit("logLevel", this.logLevel);
    },
    failed(msg) {
      this.$store.commit("runChange", false);
      this.$notify.error({
        title: "Error",
        message: msg,
        duration: 0,
      });
    },
    success(msg) {
      this.$store.commit("runChange", false);
      this.$notify({
        title: "Success",
        message: msg,
        type: "success",
      });
    },
    run() {
      this.schIndex = 0;
      this.terminal.clear();
      this.interCycle = this.cycle;
      //check all address
      for (var i in this.udsTable) {
        if (this.addrTable[this.udsTable[i].addr] == undefined) {
          this.$notify.error({
            title: "Error",
            message: `(${this.udsTable[i].name}):Please choose correct address.`,
          });
          return;
        }
      }

      this.$store.commit("runChange", true);
      this.$store.commit("setTableError", -1);
      this.startTime = new Date().getTime();
      this.schRun();
    },
    schRun() {
      if (this.schIndex < this.udsTable.length) {
        var table = this.udsTable[this.schIndex];
        ipcRenderer.send(this.mode + "udsExcute", {
          addr: this.addrTable[table.addr],
          udsTable: table.services,
          timeout: parseInt(this.udsTimeout, 10),
          sDelay: parseInt(this.sDelay, 10),
        });
        this.schIndex++;
        return true;
      } else {
        /* all sch has excuted*/
        return false;
      }
    },
  },
};
</script>
<style>
@import "../../../node_modules/xterm/css/xterm.css";
.logWindow {
  height: 300px;
  border-radius: 5px;
  border-style: solid;
  border-width: 2px;
  border-color: gray;
}
/* .terminal {
  height: 150px;
  padding: 10px;
  border-radius: 5px;
  border-style: solid;
  border-width: 2px;
  border-color: gray;
  font-size: 15px;
  overflow: auto;
} */
</style>
