<template>
  <div id="app">
    <el-container>
      <el-main>
        <router-view></router-view>
      </el-main>
    </el-container>
    <el-footer>
      <Progress/>
      <Updater/>
    </el-footer>
  </div>
</template>
<script>
import Progress from './components/progress.vue'
import Updater from './components/updater.vue'
export default {
  data(){
    return{

    }
  },
  components:{
    Progress,
    Updater
  }
}
</script>
<style>
body {
  font-family: "Helvetica Neue", Helvetica, "PingFang SC", "Hiragino Sans GB",
    "Microsoft YaHei", "微软雅黑", Arial, sans-serif;
  margin: 0px;
  padding: 0px;
}
</style>
