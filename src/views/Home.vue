<template>
    <div class="home">
        <div class="title">
          <img src='./../assets/logo256.png'/><span>ECU-BUS</span>
        </div>
        
        <el-carousel type="card" height="200px" :autoplay="true" indicatorPosition="none">
          <el-carousel-item>
            
            <div  class='peak'  @click="goPEAK">
              <img src='./../assets/PEAK.png'>
              <div class="name">UDS Over CAN/CAN-FD</div>
              <div class="button">
                <i class="el-icon-d-arrow-right"  @click="goPEAK"></i>
              </div>
              
            </div>
          </el-carousel-item>
        </el-carousel>
         <el-divider></el-divider>
         <div class="card">
           <el-row>
             <el-col :span="7" :offset="1">
              <el-card shadow="always">
                <div class="name">ISO-15765-2 + ISO-14299</div>
                <div class="tip">UDS Over CAN/CAN-FD</div>
                <el-button @click="goPEAK" type="text">GO</el-button>
              </el-card>
             </el-col>
             <el-col :span="7" :offset="1">
              <el-card shadow="always">
                <div class="name">ISO-13400-2 + ISO-14299</div>
                <div class="tip">UDS Over Ethernet</div>
                <el-button @click="goPEAK" type="text" disabled>GO</el-button>
              </el-card>
             </el-col>
           </el-row>
         </div>
    </div>
</template>
<script>
export default {
  data: function () {
    return {
      height: 0,
      width: 0,
    }
  },
  mounted () {
    this.height = window.innerHeight
    this.width = window.innerWidth
    window.addEventListener('resize', this.resizeHandle)
  },
  destroyed () {
    window.removeEventListener('resize', this.resizeHandle)
  },
  methods: {
    goPEAK () {
      this.$router.push('/pcan')
    },
    resizeHandle () {
      this.height = window.innerHeight - 233
    },
  }
}
</script>
<style>
  .home {
    font-family: Impact, Haettenschweiler, 'Arial Narrow Bold', sans-serif
  }
  .title{
      height: 48px;
       line-height: 48px;
  }
  .title span{
    position: absolute;
    margin-left: 5px;
    font-size: 32px;
  }
  .title img{
    height: 48px;
  }
  .peak{
    background-color: black;
    height: 100%;
    border-radius: 10px;
  }
  .peak img{
    margin-left: 10px;
    margin-top: 5px;
  }
  .peak .name{
    color:white;
    font-size: 36px;
    text-align: center;
    margin: 20px;
  }
  .peak .button{
    position: absolute;
    color: white;
    font-size: 25px;
    margin: 10px;
    right: 0px;
    bottom: 0px;
  }

  .card button{
    float: right;
  }
  .card .name{
    font-size: 24px;
    color: darkcyan;
  }
  .card .tip{
    font-family:'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
    text-align: center;
    
  }
</style>
