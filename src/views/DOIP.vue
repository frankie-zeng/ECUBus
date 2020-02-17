<template>
    <div>
         <el-page-header @back="goBack" content="DOIP UDS"  class="header" ></el-page-header>
         <div > 
             <DL @change="activeRouter" class="dl"></DL>  
         </div>
         <div v-if="connect" class="active">
           ACTIVE
         </div> 
         <div v-else class="deactive">
           DEACTIVE
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
      active:false
    }
  },
  computed:{
    connect:function(){
      return this.$store.state.ipConnect
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
 .active{
   z-index: auto;
 }
 .deactive{
   background-color: #F56C6C;
   margin: 20px;
   padding: 10px;
   padding-left: 30px;
   border-radius: 8px;
   z-index: auto;
   color: white;
 }
</style>