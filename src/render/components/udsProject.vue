<template>
  <div class="udsProject">
    <el-row>
      <el-col :span="8">
        <div class="proInfo">
          <h3 style="color: #303133">Project Information</h3>
          <h5 style="color: #303133">
            NAME: <span style="color: #606266">{{ project.name }}</span>
          </h5>
          <h5 style="color: #303133">
            PATH: <span style="color: #606266">{{ projectPath }}</span>
          </h5>
          <h5 style="color: #303133">
            VERSION: <span style="color: #606266">{{ project.version }}</span>
          </h5>
          <el-button type="danger" size="small" @click="closeProject"
            >Close Project</el-button
          >
          <el-button type="success" size="small" @click="saveProject"
            >Save Project</el-button
          >
        </div>
      </el-col>
      <el-col :span="6">
        <el-upload
          :file-list="fileList"
          action=""
          :http-request="fileUpload"
          :on-remove="fileRemove"
          :on-change="fileChange"
        >
          <el-tooltip
            class="item"
            effect="dark"
            content="Copy files into project folder"
            placement="top-end"
          >
            <el-button size="small" type="primary">Upload Firmware</el-button>
          </el-tooltip>
        </el-upload>
      </el-col>
      <el-col :span="9" :offset="1">
        <el-tabs v-model="activeName">
          <el-tab-pane label="DOCAN" name="can"
            ><el-card>
              <div slot="header" class="clearfix">
                <span>DOCAN Service Table</span>
              </div>
              <IE mode="can" @imported="importCb" />
              <div
                v-for="(item, index) in project.can"
                :key="index"
                style="margin: 5px"
              >
                {{ item.name }}: {{ item.services.length }} services
              </div>
            </el-card></el-tab-pane
          >
          <el-tab-pane label="DOLIN" name="lin">
            <el-card>
              <div slot="header" class="clearfix">
                <span>DOLIN Service Table</span>
              </div>
              <IE mode="lin" @imported="importCb" />
              <div
                v-for="(item, index) in project.lin"
                :key="index"
                style="margin: 5px"
              >
                {{ item.name }}: {{ item.services.length }} services
              </div>
            </el-card></el-tab-pane
          >
          <el-tab-pane label="DOIP" name="doip"
            ><el-card>
              <div slot="header" class="clearfix">
                <span>DOIP Service Table</span>
              </div>
              <IE mode="doip" @imported="importCb" />
              <div
                v-for="(item, index) in project.eth"
                :key="index"
                style="margin: 5px"
              >
                {{ item.name }}: {{ item.services.length }} services
              </div>
            </el-card></el-tab-pane
          >
          <el-tab-pane label="Loopback" name="lp"
            ><el-card>
              <div slot="header" class="clearfix">
                <span>Loopback Service Table</span>
              </div>
              <IE mode="lp" @imported="importCb" />
              <div
                v-for="(item, index) in project.simulate"
                :key="index"
                style="margin: 5px"
              >
                {{ item.name }}: {{ item.services.length }} services
              </div>
            </el-card></el-tab-pane
          >
        </el-tabs>
      </el-col>
    </el-row>
  </div>
</template>

<script>
const { remote } = require("electron");
const fs = remote.require("fs");
const path = remote.require("path");
import IE from "./../components/importexport/importexport.vue";
export default {
  data() {
    return {
      fileList: [],
      activeName: "can",
    };
  },
  components: {
    IE,
  },
  computed: {
    project: function () {
      return this.$store.state.udsProject;
    },
    projectPath: function () {
      return this.$store.state.udsProjectPath;
    },
  },
  mounted() {
    this.fileList = this.project.fileList;
    this.activeName = this.mode;
  },
  props: {
    mode: {
      type: String,
      default: function () {
        return "can";
      },
    },
  },
  methods: {
    fileUpload(val) {
      let newFile = path.join(this.projectPath, val.file.name);
      fs.copyFileSync(val.file.path, newFile);
      val.onSuccess();
    },
    // eslint-disable-next-line no-unused-vars
    fileRemove(file, fileList) {
      try {
        let newFile = path.join(this.projectPath, file.name);
        fs.unlinkSync(newFile);
      } catch {
        this.$notify.error({
          title: "error",
          message: `Remove ${file.name} failed`,
        });
      }
    },
    // eslint-disable-next-line no-unused-vars
    fileChange(file, fileList) {
      this.$store.commit("setUdsProject", {
        key: "fileList",
        val: fileList,
      });
      let configFile = path.join(this.projectPath, "config.json");
      fs.writeFileSync(configFile, JSON.stringify(this.project));
    },
    importCb() {
      let configFile = path.join(this.projectPath, "config.json");
      fs.writeFileSync(configFile, JSON.stringify(this.project));
      this.$emit("imported");
    },
    closeProject() {
      this.$store.commit("setUdsProject", {
        key: "name",
        val: "",
      });
    },
    saveProject() {
      let configFile = path.join(this.projectPath, "config.json");
      fs.writeFileSync(configFile, JSON.stringify(this.project));
      this.$notify({
          title: 'Success',
          message: 'Save Project Successfully',
          type: 'success'
        });
    },
  },
};
</script>

<style>
.proInfo {
  margin: 10px;
}
.udsProject {
  /* background-color: #ebeef5;
  padding: 20px;
  border-radius: 10px; */
}
</style>