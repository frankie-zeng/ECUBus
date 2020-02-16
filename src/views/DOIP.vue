<template>
    <div>
         <el-page-header @back="goBack" content="DOIP UDS"  class="header" ></el-page-header>
         <div > 
             <DL @change="activeRouter" class="dl"></DL>  
         </div>
        
         
         
         
         
    </div>
    
</template>
<script>
import DL from './../components/doip/deviceList.vue'


const { ipcRenderer } = require('electron')

export default {
  components: {
    DL,
  },
  mounted(){
    this.reloadInterface()
  },
  data: function () {
    return {
    }
  },
  methods:{
    reloadInterface(){
        this.netLoading=true
        this.NetInterface=ipcRenderer.sendSync('ip-interface')
        this.netLoading=false
    },
    goBack(){
        this.$router.push('/')
    },
    activeRouter(info){
        console.log(info)
    },
    bind(){

    }
  }
}
</script>
<style>
.header{
    margin: 30px;

 }
 .setting{
     margin: 30px;
 }
 .dl{
     position: absolute;
     top:10px;
     right: 300px;
 }
</style>