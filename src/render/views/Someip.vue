
<template>
  <div>
    <el-row>
      <el-col :span="12">
        <div>
          <el-page-header
            @back="goBack"
            content="SOME/IP"
            class="header"
            title=""
          ></el-page-header>
        </div>
      </el-col>

      <el-col :span="12">
        <div class="control">
          <el-button
            type="primary"
            icon="el-icon-refresh-left"
            size="mini"
            v-if="started"
            @click="startSP"
            plain
            >Relauch</el-button
          >
          <el-button
            type="success"
            icon="el-icon-video-play"
            size="mini"
            v-else
            @click="startSP"
            plain
            >Start</el-button
          >
          <el-button
            plain
            type="danger"
            icon="el-icon-video-pause"
            size="mini"
            @click="stopSP"
            :disabled="!started"
            >Stop</el-button
          >
          <el-button
            icon="el-icon-delete"
            size="mini"
            type="primary"
            plain
            @click="saveConfig"
            :disabled="started"
            >Save</el-button
          >
          <!-- <el-button icon="el-icon-delete" size="mini" type="primary" plain
            >Clear</el-button
          > -->
        </div>
        <div class="config">
          <el-tag style="width: 100%; margin-bottom: 2px" size="small"
            >{{ configFile }}<span v-if="!clean">*</span></el-tag
          >
          <codemirror
            v-model="configCode"
            ref="editor"
            @changes="codeChange"
            :options="codeMirrorOptions"
          />
        </div>
        <div id="terminal" class="logWindow"></div>
      </el-col>
    </el-row>

    <!-- <div>
      <el-tag
        >Configurate file:{{ configFile }}
        <el-button
          icon="el-icon-folder-opened"
          type="text"
          size="mini"
        ></el-button
      ></el-tag>
    </div>
    <el-divider></el-divider>
    <h2>Request Service</h2>
    <el-divider></el-divider>
    <h2>Offer Service</h2> -->
  </div>
</template>

<script>
/* eslint-disable no-unused-vars */
const { ipcRenderer, remote } = require("electron");
const { Terminal } = require("xterm");
const { FitAddon } = require("xterm-addon-fit");
const fitAddon = new FitAddon();
const path = remote.require("path");
const fs = remote.require("fs");
// eslint-disable-next-line no-undef
const configFile = path.join(__static, "someip", "vsomeip.json");
export default {
  data() {
    return {
      configFile: configFile,
      terminal: "",
      started: false,
      clean: true,
      firstCheck: true,
      cm: "",
      configCode: "",
      codeMirrorOptions: {
        theme: "default",
        mode: "application/ld+json",
        lineNumbers: true,
        lint: true,
      },
    };
  },
  mounted() {
    console.log(this.configFile);
    this.configCode = JSON.stringify(
      JSON.parse(fs.readFileSync(this.configFile).toString("ascii")),
      null,
      4
    );
    this.cm = this.$refs.editor.codemirror;
    this.cm.setSize("100%", window.innerHeight - 500);
    // this.cm.doc.markClean()
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

    window.addEventListener("resize", this.resizeTerminal);
    // ipcRenderer.send('someipStartApp', 'vsomeipd')
    ipcRenderer.on("someip_log", (event, message) => {
      //   const text = util.format.apply(util, message.data);
      let msg;
      let text = Buffer.from(message.data).toString("ascii");
      let usedTime = Date.parse(message.date) - this.startTime;
      if (usedTime < 0) {
        usedTime = 0;
      }
      if (message.level == "info") {
        msg = `\x1B[1;;32m${text}\x1B[0m\r`;
      } else if (message.level == "error") {
        msg = `\x1B[1;;31m${text}\x1B[0m\r`;
      } else {
        msg = `\x1B[1;;30m${text}\x1B[0m\r`;
      }
      this.terminal.write(msg);
    });
    // ipcRenderer.send("someipStart");
    // setTimeout(() => {
    //   ipcRenderer.send("someipCreateApp", {
    //     name: "vsomeipd",
    //     route: true,
    //   });
    //   ipcRenderer.send("someipStartApp", "vsomeipd");
    // }, 1000);
  },
  destroyed() {
    if (this.started) {
      ipcRenderer.send("someipStop");
    }

    window.removeEventListener("resize", this.resizeTerminal);
    ipcRenderer.removeAllListeners("someip_log");
  },

  methods: {
    saveConfig() {
      fs.writeFileSync(this.configFile, this.configCode);
    },
    codeChange() {
      if (this.firstCheck) {
        this.cm.doc.markClean();
        this.firstCheck = false;
      }
      this.clean = this.cm.doc.isClean();
    },
    resizeTerminal() {
      this.cm.setSize("100%", window.innerHeight - 500);
      if (window.innerWidth % 1 === 0 && window.innerHeight % 1 === 0) {
        fitAddon.fit();
      }
    },
    goBack() {
      this.$router.push("/");
    },
    startSP() {
      if (this.started) {
        ipcRenderer.send("someipStop");
        ipcRenderer.send("someipStart");
      } else {
        ipcRenderer.send("someipStart");
        this.started = true;
      }
    },
    stopSP() {
      if (this.started) {
        ipcRenderer.send("someipStop");
        this.started = false;
      }
    },
  },
};
</script>

<style>
.control {
  text-align: center;
  margin: 10px;
}
.config {
  margin-top: 10px;
  margin-bottom: 20px;
}
</style>