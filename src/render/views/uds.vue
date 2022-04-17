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
      <el-form :model="createProInfo" label-width="120px">
        <el-form-item label="Name" required>
          <el-input v-model="createProInfo.name"></el-input>
        </el-form-item>
        <el-form-item label="Path" required>
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
    <div style="text-align: center; margin: 100px" v-if="project.name == ''">
      <el-button type="primary" @click="createProject"
        >Create UDS Project</el-button
      >
      <el-button type="success">Open UDS Project</el-button>
    </div>
    <div v-else>
      <udsProject/>
    </div>
    <el-divider />
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
const { remote } = require("electron");
// eslint-disable-next-line no-unused-vars
const { dialog } = remote;
const fs = remote.require("fs");
const path = remote.require("path");
import topPack from "./../../../package.json";
import udsProject from "./../components/udsProject.vue"
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
  components:{
      udsProject
  },
  computed: {
    project: function () {
      return this.$store.state.udsProject;
    },
  },
  methods: {
    createProject() {
      this.create = true;
    },
    realCreatePro() {
      let proPath = path.join(this.createProInfo.path, this.createProInfo.name);
      if (fs.existsSync(proPath)) {
        this.$notify.error({
          title: "error",
          message: `Project ${this.createProInfo.name} has exist`,
        });
        return;
      } else {
        fs.mkdirSync(proPath);
        let configFile = path.join(proPath, "config.json");
        fs.writeFileSync(
          configFile,
          JSON.stringify({
            name: this.createProInfo.name,
            version: topPack.version,
          })
        );
        this.$store.commit("setUdsProject", {
          key: "name",
          val: this.createProInfo.name,
        });
        this.$store.commit("setUdsProject", {
          key: "name",
          val: this.createProInfo.name,
        });
        this.$store.commit("setUdsProject", {
          key: "path",
          val: proPath,
        });
        this.create = false;
      }
    },
    choseFolder(c) {
      if (c == "create") {
        this.createProInfo.path = dialog.showOpenDialogSync({
          title: "Create Project",
          properties: ["openDirectory"],
        })[0];
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
.proInfo {
  margin: 50px;
}
</style>