<template>
  <div>
    <el-row>
      <el-col :span="12">
        <div class="proInfo">
          <h3 style="color: #303133">Project Information</h3>
          <h4 style="color: #303133">
            NAME: <span style="color: #606266">{{ project.name }}</span>
          </h4>
          <h4 style="color: #303133">
            PATH: <span style="color: #606266">{{ project.path }}</span>
          </h4>
          <el-button type="danger" size="small">Close Project</el-button>
        </div>
      </el-col>
      <el-col :span="10">
        <el-upload :file-list="fileList" action="" :http-request="fileUpload">
          <el-button size="small" type="primary">Upload Firmware</el-button>
        </el-upload>
      </el-col>
    </el-row>
  </div>
</template>

<script>
export default {
  data() {
    return {
      fileList: [],
    };
  },
  computed: {
    project: function () {
      return this.$store.state.udsProject;
    },
  },
  methods: {
      fileUpload(val){
          console.log(val)
          val.onSuccess()
      }
  },
};
</script>

<style>
.proInfo {
  margin: 50px;
}
</style>