<template>
  <div style="text-align:center">
      <div >
        <el-transfer v-model="value" :data="group" :titles="['Service', 'Group']"  style="text-align: left; display: inline-block" @change="tChange"></el-transfer>
      </div>
      
      Group Name：<el-input v-model="groupname" placeholder="Group Name" style="width:50%;margin:20px;"></el-input><el-button type="primary" @click="addGroup">Save Group</el-button>
  </div>
</template>

<script>
import config from "./service.js";
// eslint-disable-next-line no-unused-vars
const { ipcRenderer } = require("electron");
export default {
    props: ["mode","table"],
    data() {
        return{
            value:[],
            groupname:'',
            config:config['uds'],
        }
    },
    created(){
        this.value=[]
    },
    computed: {
        group: function() {
            var payload=[]
            for(var i in this.table){
                if(this.table[i].type=='uds'){
                    for(var j in this.table[i].payload){
                        payload.push({
                            key:i+','+j,
                            label:i+'-'+this.table[i].payload[j].name
                        })
                    }
                }
            }
            return payload
        },
    },
    methods:{
        tChange:function(){
            
           
        },
        addGroup:function(){
            var service=[]
            for(var i in this.table){
                if(this.table[i].type=='group'){
                    this.$notify.error({
                        title: "Error",
                        message: 'Remove group firstly.'
                    });
                    return 
                }
                service[i]={
                    name:i+'-'+this.table[i].service.name,
                    value:this.table[i].service.value,
                    input:[]
                }
            }
            for(i in this.value){
                var s=this.value[i].split(',')
                var tableIndex=parseInt(s[0])
                var payloadIndex=parseInt(s[1])
                for(var j in this.config){
                    if(this.config[j].value===this.table[tableIndex].service.value){
                        for(var z in this.config[j].input){
                            if(this.config[j].input[z].name===this.table[tableIndex].payload[payloadIndex].name){
                                service[tableIndex].input.push(this.config[j].input[z])
                                break
                            }
                        }
                        break
                    }
                }
            }
            if( this.groupname===''){
                this.$notify.error({
                    title: "Error",
                    message: 'Please input group name.'
                });
                return 
            }
            if(this.table.length===0){
                this.$notify.error({
                    title: "Error",
                    message: 'Must contain one service.'
                });
                return
            }
            var newgroup={
                name:this.groupname,
                service:service,
                // changeslot:this.value,
                table:JSON.parse(JSON.stringify(this.table))
            }
            ipcRenderer.sendSync("saveGroup", [this.groupname,JSON.stringify(newgroup)])
            this.$emit("added");
            this.$notify({
                title: "Success",
                message: 'Save group ok',
                type: "success"
            });
        }
    }
}
</script>

<style>

</style>