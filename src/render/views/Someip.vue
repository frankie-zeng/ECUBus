
<template>
  <div>
      <div>
          <el-page-header
          @back="goBack"
          content="SOME/IP"
          class="header"
          title=""
        ></el-page-header>
      </div>

      <div>
          <el-tag>Configurate file:{{configFile}} <el-button icon="el-icon-folder-opened" type="text" size="mini"></el-button></el-tag>
      </div>
        <el-divider></el-divider>
      <h2>
          Request Service
      </h2>
      <el-divider></el-divider>
      <h2>
          Offer Service
      </h2>
  </div>
</template>

<script>
const { ipcRenderer } = require('electron')
export default {
    data(){
        return{
            configFile:'',

        }
    },
    mounted(){
        ipcRenderer.send('someipCreateApp', {
            name:'vsomeipd',
            route:true
        })
        ipcRenderer.send('someipStartApp', 'vsomeipd')
    },
    methods:{
        goBack(){
            this.$router.push("/");
        }
    }

}
</script>

<style>

</style>