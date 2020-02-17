<template>
    <div>
         <el-page-header @back="goBack" content="DOIP UDS"  class="header" ></el-page-header>
         <div > 
             <DL @change="activeRouter" class="dl" ref="deviceList"></DL>  
         </div>
         <div v-if="connect" class="iactive">
           <el-button type="danger" size="mini" icon="el-icon-close" circle plain @click="deactive"></el-button><span><strong> ACTIVE</strong> VIN: {{activeTarget.vin}} IP: {{activeTarget.ip}} SA: {{activeInfo.testerAddr}} TA: {{activeInfo.entityAddr}}</span>
         </div> 
         <div v-else class="deactive">
           DEACTIVE
         </div>
        
         
         
         
         
    </div>
    
</template>
<script>
import DL from './../components/doip/deviceList.vue'
export default {
  components: {
    DL,
  },
  data: function () {
    return {
      active:false,
      activeTarget:{},
      activeInfo:{}
    }
  },
  computed:{
    connect:function(){
      return this.$store.state.ipConnect
    }
  },
  methods:{
    goBack(){
        this.$router.push('/')
    },
    activeRouter(info){
      this.activeTarget=info[0]
      this.activeInfo=info[1]
    },
    deactive(){
      this.$refs.deviceList.deactive()
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
 .iactive{
   background-color: #67C23A;
   margin: 20px;
   padding: 10px;
   padding-left: 30px;
   border-radius: 8px;
   z-index: auto;
   color: white;
   /* line-height: 28px; */
   vertical-align: middle;
 }
 .iactive span{
   line-height: 30px;
   padding-left: 10px;
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