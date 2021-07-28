
<template>
  <div>
    <el-row>
      <el-col :span="11">
        <div>
          <el-page-header
            @back="goBack"
            content="SOME/IP"
            class="header"
            title=""
          ></el-page-header>
        </div>
        <div>
          <el-form :inline="true" :model="formApp" size="mini">
            <el-form-item label="APP Name:">
              <el-input v-model="formApp.name" placeholder=""></el-input>
            </el-form-item>
            <el-form-item label="Rounter">
              <el-checkbox v-model="formApp.route"></el-checkbox>
            </el-form-item>
            <el-form-item>
              <el-button type="primary" @click="createApp" :disabled="!started"
                >Create</el-button
              >
            </el-form-item>
          </el-form>
        </div>
        <el-card class="box-card">
          <div>
            <el-select
              v-model="aApp"
              placeholder="Choose App"
              size="mini"
              style="width: 100%"
              @change="appChange"
            >
              <el-option
                v-for="(item, index) in apps"
                :key="item.name"
                :label="item.name + (apps[index].route ? ' (router)' : '')"
                :value="item.name"
              >
              </el-option>
            </el-select>
          </div>
          <div>
            <el-empty v-if="aApp == ''"></el-empty>
            <div v-else>
              <SPAPP
                :name="aApp"
                :route="appIsRoute(this.aApp)"
                :terminal="terminal"
                v-if="appShow"
              />
            </div>
          </div>
        </el-card>
      </el-col>

      <el-col :span="12" :offset="1">
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
        <div style="position: relative">
          <el-button
            icon="el-icon-delete"
            circle
            size="mini"
            class="logClose"
            @click="clearLog"
          ></el-button>
          <div id="terminal" class="logWindow" :style="{ height: logH }"></div>
        </div>
      </el-col>
    </el-row>
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
const util = require("util");
const isDevelopment = process.env.NODE_ENV !== "production";

import SPAPP from "./../components/spapp.vue";
export default {
  data() {
    return {
      configFile: '',
      terminal: "",
      started: false,
      appShow: true,
      clean: true,
      firstCheck: true,
      logH: window.innerHeight / 2 - 110 + "px",
      cm: "",
      configCode: "",
      codeMirrorOptions: {
        theme: "default",
        mode: "application/ld+json",
        lineNumbers: true,
        lint: true,
        readOnly: false,
      },
      formApp: {
        name: "",
        route: false,
      },
      apps: [],
      aApp: "",
    };
  },
  components: {
    SPAPP,
  },
  mounted() {
    this.configFile = path.join(
      // eslint-disable-next-line no-undef
      isDevelopment ? path.join(__static, "someip") : remote.app.getPath("appData"),
      "vsomeip.json"
    );
    if (fs.existsSync(this.configFile)) {
      this.configCode = JSON.stringify(
        JSON.parse(fs.readFileSync(this.configFile).toString("ascii")),
        null,
        4
      );
    } else {
      this.configCode = JSON.stringify({
        unicast: "192.168.1.1",
        netmask: "255.255.255.0",
        logging: {
          level: "debug",
          console: "true",
          file: {
            enable: "false",
            path: "",
          },
          dlt: "false",
        },
        applications: [
          {
            name: "client",
            id: "0x5555",
          },
        ],
        services: [
          {
            service: "0x1111",
            instance: "0x2222",
            reliable: {
              port: "30000",
              "enable-magic-cookies": "false",
            },
            unreliable: "30510",
          },
        ],
        routing: "client",
        "service-discovery": {
          enable: "true",
          multicast: "224.0.0.1",
          port: "30490",
          protocol: "udp",
          initial_delay_min: "10",
          initial_delay_max: "100",
          repetitions_base_delay: "200",
          repetitions_max: "3",
          ttl: "3",
          cyclic_offer_delay: "2000",
          request_response_delay: "1500",
        },
      },null,4);
      this.saveConfig();
    }

    this.cm = this.$refs.editor.codemirror;
    this.cm.setSize("100%", window.innerHeight / 2 - 100);
    // this.cm.doc.markClean()
    this.terminal = new Terminal({
      theme: {
        background: "#ffffff",
        selection: "gray",
      },
      fontWeight: "bold",
      fontSize: 12,
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
      //text = util.format.apply(util, text);

      let usedTime = Date.parse(message.date) - this.startTime;
      if (usedTime < 0) {
        usedTime = 0;
      }
      if (message.level == "info") {
        msg = `\x1B[1;;32m${text}\x1B[0m`;
      } else if (message.level == "error") {
        msg = `\x1B[1;;31m${text}\x1B[0m`;
      } else if (message.level == "warning") {
        msg = `\x1B[1;;33m${text}\x1B[0m`;
      } else {
        msg = `\x1B[1;;30m${text}\x1B[0m`;
      }
      this.terminal.write(msg);
    });
    ipcRenderer.on("someip_start", () => {
      //recreate app
      for (let i in this.apps) {
        let app = this.apps[i];
        ipcRenderer.send("someipCreateApp", app);
      }
      if (this.aApp != "") {
        this.appShow = false;
        this.$nextTick(() => {
          this.appShow = true;
        });
      }
    });
  },
  destroyed() {
    if (this.started) {
      ipcRenderer.send("someipStop");
    }
    window.removeEventListener("resize", this.resizeTerminal);
    ipcRenderer.removeAllListeners("someip_log");
  },

  methods: {
    appChange() {
      this.appShow = false;
      this.$nextTick(() => {
        this.appShow = true;
      });
    },
    appIsRoute(name) {
      for (let i in this.apps) {
        if (name == this.apps[i].name) {
          return this.apps[i];
        }
      }
      return false;
    },
    clearLog() {
      this.terminal.clear();
    },
    createApp() {
      let name = this.formApp.name;
      for (let i in this.apps) {
        if (name == this.apps[i].name) {
          this.$notify({
            title: "Warining",
            message: `APP ${name} has created`,
            type: "warning",
          });
          return;
        }
      }
      let res = ipcRenderer.sendSync("someipCreateApp", this.formApp);
      if (res.error == 0) {
        this.apps.push(JSON.parse(JSON.stringify(this.formApp)));
        this.$notify({
          title: "Success",
          message: `APP ${name} create successfully`,
          type: "success",
        });
        this.formApp = {
          name: "",
          route: false,
        };
      } else {
        this.$notify({
          title: "Error",
          message: `Create APP ${name} Error:${res.error}, See Log`,
          type: "error",
        });
      }
    },
    saveConfig() {
      fs.writeFileSync(this.configFile, this.configCode);
      this.cm.doc.markClean();
      this.clean = true
    },
    codeChange() {
      if (this.firstCheck) {
        this.cm.doc.markClean();
        this.firstCheck = false;
      }
      this.clean = this.cm.doc.isClean();
    },
    resizeTerminal() {
      this.cm.setSize("100%", window.innerHeight / 2 - 100);
      this.logH = window.innerHeight / 2 - 110 + "px";
      this.$nextTick(() => {
        if (window.innerWidth % 1 === 0 && window.innerHeight % 1 === 0) {
          fitAddon.fit();
        }
      });
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
      this.codeMirrorOptions.readOnly = true;
      this.codeMirrorOptions.className = "readOnly";
    },
    stopSP() {
      if (this.started) {
        ipcRenderer.send("someipStop");
        this.started = false;
      }
      this.apps = [];
      this.aApp = "";
      this.codeMirrorOptions.readOnly = false;
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
.logClose {
  position: absolute;
  right: 30px;
  top: 20px;
  z-index: 20;
}
</style>