<template>
  <div>
    <el-dialog title="Create Project" :visible.sync="create" width="30%">
      <!-- <span>这是一段信息</span>
      <span slot="footer" class="dialog-footer">
        <el-button @click="dialogVisible = false">取 消</el-button>
        <el-button type="primary" @click="dialogVisible = false"
          >确 定</el-button
        >
      </span> -->
      <el-form :model="createProInfo" label-width="80px" ref="createForm">
        <el-form-item label="Name" required prop="name">
          <el-input v-model="createProInfo.name"></el-input>
        </el-form-item>
        <el-form-item label="Path" required prop="path">
          <el-input v-model="createProInfo.path" disabled>
            <el-button
              slot="append"
              icon="el-icon-folder"
              @click="choseFolder('create')"
            ></el-button>
          </el-input>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" @click="realCreatePro">Create</el-button>
        </el-form-item>
      </el-form>
    </el-dialog>
    <el-page-header
      @back="goBack"
      content="UDS"
      class="header"
      title=""
    ></el-page-header>
    <el-divider />
    <div style="text-align: center; margin: 50px" v-if="project.name == ''">
      <el-button type="primary" @click="createProject"
        >Create UDS Project</el-button
      >
      <el-button type="success" @click="openProject"
        >Open UDS Project</el-button
      >
      <div class="tips_create">
        <el-alert
          show-icon
          title="Please create or open a project firstly"
          :closable="false"
          type="info"
        >
        </el-alert>
      </div>
    </div>
    <div v-else>
      <udsProject />
    </div>
    <el-divider />
    <div class="card">
      <el-row>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoCAN ISO-15765-2</div>
            <div class="tip">UDS Over CAN/CAN-FD</div>
            <el-button
              @click="goPEAK"
              type="text"
              :disabled="project.name == ''"
              >GO</el-button
            >
          </el-card>
        </el-col>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoIP ISO-13400-2</div>
            <div class="tip">UDS Over Ethernet</div>
            <el-button
              @click="goDOIP"
              type="text"
              :disabled="project.name == ''"
              >GO</el-button
            >
          </el-card>
        </el-col>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">UDS Loopback</div>
            <div class="tip">UDS Loopback Test</div>
            <el-button
              @click="goEmulate"
              type="text"
              :disabled="project.name == ''"
              >GO</el-button
            >
          </el-card>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span="6" :offset="1">
          <el-card shadow="always">
            <div class="name">DoLIN ISO-17987-2</div>
            <div class="tip">UDS Over LIN</div>
            <el-button @click="goLIN" type="text" :disabled="project.name == ''"
              >GO</el-button
            >
          </el-card>
        </el-col>
      </el-row>
    </div>
  </div>
</template>

<script>
const { remote } = require("electron");
// eslint-disable-next-line no-unused-vars
const { dialog } = remote;
const fs = remote.require("fs");
const path = remote.require("path");
import topPack from "./../../../package.json";
import udsProject from "./../components/udsProject.vue";
const { ipcRenderer } = require("electron");
export default {
  data() {
    return {
      create: false,
      createProInfo: {
        name: "",
        path: "",
      },
    };
  },
  components: {
    udsProject,
  },
  computed: {
    project: function () {
      return this.$store.state.udsProject;
    },
    projectPath: function () {
      return this.$store.state.udsProjectPath;
    },
  },
  methods: {
    createProject() {
      this.create = true;
    },
    openProject() {
      let val = dialog.showOpenDialogSync({
        title: "Open Project",
        properties: ["openDirectory"],
      });
      if (val != undefined) {
        try {
          let config = JSON.parse(
            fs.readFileSync(path.join(val[0], "config.json"))
          );
          this.$store.commit("setUdsProject", {
            key: "name",
            val: config.name,
          });
          this.$store.commit("canTableLoad", config.can);
          this.$store.commit("linTableLoad", config.lin);
          this.$store.commit("doipTableLoad", config.eth);
          this.$store.commit("lpTableLoad", config.simulate);
          this.$store.commit("setUdsProject", {
            key: "fileList",
            val: config.fileList,
          });
          this.$store.commit("setUdsProject", {
            key: "version",
            val: config.version,
          });
          this.$store.commit("setUdsProjectPath", val[0]);
          ipcRenderer.send("udsProjectPath", val[0]);
        } catch {
          this.$notify.error({
            title: "error",
            message: `Open Project Error`,
          });
        }
      }
    },
    realCreatePro() {
      this.$refs.createForm.validate((valid) => {
        if (valid) {
          let proPath = path.join(
            this.createProInfo.path,
            this.createProInfo.name
          );
          if (fs.existsSync(proPath)) {
            this.$notify.error({
              title: "error",
              message: `Project ${this.createProInfo.name} has exist`,
            });
            return;
          } else {
            fs.mkdirSync(proPath);
            let configFile = path.join(proPath, "config.json");
            ipcRenderer.send("udsProjectPath", proPath);
            this.$store.commit("setUdsProject", {
              key: "name",
              val: this.createProInfo.name,
            });
            this.$store.commit("setUdsProjectPath", proPath);
            this.$store.commit("setUdsProject", {
              key: "version",
              val: topPack.version,
            });
            this.$store.commit("canTableLoad", []);
            this.$store.commit("linTableLoad", []);
            this.$store.commit("doipTableLoad", []);
            this.$store.commit("lpTableLoad", []);
            this.$store.commit("setUdsProject", {
              key: "fileList",
              val: [],
            });
            fs.writeFileSync(configFile, JSON.stringify(this.project));
            this.create = false;
          }
        } else {
          return false;
        }
      });
    },
    choseFolder(c) {
      if (c == "create") {
        let val = dialog.showOpenDialogSync({
          title: "Create Project",
          properties: ["openDirectory"],
        });
        if (val != undefined) {
          this.createProInfo.path = val[0];
        }
      }
    },
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
.tips_create {
  position: absolute;
  top: 120px;
  right: 30px;
}
</style>