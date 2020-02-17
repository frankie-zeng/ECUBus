<template>
    <div>
        <el-dialog
            title="ACTIVE Router"
            :visible.sync="activeStatus"
            width="50%"
            >
            <el-row>
                <el-col :span="24"><el-tag size="small" type="info"  class="info">IP: {{activeItem.ip}}</el-tag></el-col>
                <!-- <el-col :span="11" :offset="1"><el-tag size="small" type="info"  class="info">PORT: {{activeItem.port}}</el-tag></el-col> -->
                <el-col :span="24"><el-tag size="small" type="info"  class="info">EID: 0x{{activeItem.eid}}</el-tag></el-col>
                <el-col :span="24"><el-tag size="small" type="info"  class="info">GID: 0x{{activeItem.gid}}</el-tag></el-col>
                <el-col :span="24"><el-tag size="small" type="info"  class="info">FutureAction: {{activeItem.fAction}}</el-tag></el-col>
                <el-col :span="24"><el-tag size="small" type="info"  class="info" v-if="activeItem.syncStatus">SyncStatus: {{activeItem.sync}}</el-tag></el-col>
            </el-row>
            <el-form ref="active" :model="active" label-width="150px" size="small" :rules="rules1">
                <el-form-item label="Source Address:" prop="sa">
                    <el-row>
                        <el-col :span="24">
                        <el-input v-model.number="active.sa"></el-input>  
                        </el-col>
                    </el-row> 
                </el-form-item>
                <el-form-item label="Active Type:" prop="type">
                    <el-row>
                        <el-col :span="24">
                         <el-select v-model="active.type" style="width:100%" >
                                <el-option label="Default" value="00"/>
                                <el-option label="WWH-OBD" value="01"/>
                                <el-option label="Central Security" value="E0"/>
                            </el-select>
                        </el-col>
                    </el-row> 
                </el-form-item>
                 <el-form-item style="text-align:right">
                    <el-button type="primary" @click="activeConnect">Active</el-button>
                </el-form-item>
            </el-form>
        </el-dialog>
        <el-dialog
            title="DOIP设置"
            :visible.sync="dialogVisible"
            width="80%"
            >
                <div>
                    <el-form ref="setting" :model="setting" label-width="150px" size="small"  :rules="rules">
                        <el-form-item label="网卡：" prop="interface">
                            <el-row>
                                <el-col :span="20">
                                    <el-select v-model="setting.interface" placeholder="请选择网卡" style="width:100%" @visible-change="reloadInterface" :loading="netLoading">
                                        <el-option
                                        v-for="(item, key) in NetInterface"
                                        :key="key"
                                        :label="key"
                                        :value="item[1]"
                                        >
                                        <span style="float: left">{{ key }}</span>
                                        <span style="float: right; color: #8492a6; font-size: 13px">{{item[1].address}}</span>
                                        </el-option>
                                    </el-select>
                                </el-col>
                                <el-col :span="4" style="text-align:center">
                                    <el-button type="danger" @click="bindAddress">Bind</el-button>
                                </el-col>
                            </el-row>
                        </el-form-item>
                        <el-form-item label="组播/单播 地址：" prop="multiaddress">
                            <el-row>
                                <el-col :span="20">
                                <el-input v-model="setting.multiaddress"></el-input>  
                                </el-col>
                            </el-row> 
                        </el-form-item>
                        <el-form-item label="VIN：" prop="vin">
                            <el-row>
                                <el-col :span="20">
                                <el-input v-model="setting.vin" maxlength="17" show-word-limit></el-input>  
                                </el-col>
                            </el-row> 
                        </el-form-item>
                        <el-form-item label="EID：" prop="eid">
                            <el-row>
                                <el-col :span="20">
                                <el-input v-model="setting.eid" maxlength="12" show-word-limit>
                                     <template slot="prepend">0x</template>
                                     </el-input>  
                                </el-col>
                            </el-row> 
                        </el-form-item>
                        <el-form-item>
                            <el-row>
                                <el-col :span="20">
                                    <i class="el-icon-question">如果<strong>VIN</strong>和<strong>EID</strong>同时存在，会优先调用<strong>VehicleIdentificationRequestWithVIN(0x0003)</strong></i>
                                </el-col>
                            </el-row> 
                        </el-form-item>
                    </el-form>
                </div>
            </el-dialog>
        <VueDragResize :isActive="false" :w="1" :h="1" v-on:resizing="resize" v-on:dragging="resize" :isResizable="false" >
        <div class="divList"  :style="{height:dHigh+'px'}">
            <el-row class="deviceHeader">
                <el-col :span="24">
                    
                </el-col>
                <el-col :span="12" class="list">设备列表({{deviceList.length}})：</el-col>
                <el-col :span="11" style="text-align:right">
                    <el-button type="text" icon="el-icon-setting" style="font-size:20px" @click="dialogVisible=true"></el-button>
                    <el-button type="text" icon="el-icon-sort" style="font-size:20px" @click="changeSize"></el-button>
                    <el-button type="text" icon="el-icon-refresh" style="font-size:20px" @click="refresh"></el-button>
                </el-col>
            </el-row>
            <div>
                <div v-for="(item, key) in deviceList" class="item"
                    :key="key">
                    <el-row class="action">
                        <el-col :span="14"><el-tag size="medium">VIN: {{item.vin}}</el-tag></el-col>
                        <el-col :span="10" style="text-align:right"><el-button type="primary" icon="el-icon-caret-right" size="mini" @click="activeRouter(item)"></el-button></el-col>
                    </el-row>
                    <el-row>
                        <el-col :span="12"><el-tag size="small" type="info"  class="info">IP: {{item.ip}}</el-tag></el-col>
                        <el-col :span="11" :offset="1"><el-tag size="small" type="info"  class="info">PORT: {{item.port}}</el-tag></el-col>
                        <el-col :span="24"><el-tag size="small" type="info"  class="info">Logical Address: {{item.logicalAddr}}</el-tag></el-col>
                        <el-col :span="24"><el-tag size="small" type="info"  class="info">EID: 0x{{item.eid}}</el-tag></el-col>
                        <el-col :span="24"><el-tag size="small" type="info"  class="info">GID: 0x{{item.gid}}</el-tag></el-col>
                        <el-col :span="24"><el-tag size="small" type="info"  class="info">FutureAction: {{item.fAction}}</el-tag></el-col>
                        <el-col :span="24"><el-tag size="small" type="info"  class="info" v-if="item.syncStatus">SyncStatus: {{item.sync}}</el-tag></el-col>
                    </el-row>
                </div>
            </div>
        </div>
        </VueDragResize>


    </div>
</template>
<script>
import VueDragResize from 'vue-drag-resize'
const { ipcRenderer } = require('electron')
function dot2num(dot) 
{
    var d = dot.split('.');
    return ((((((+d[0])*256)+(+d[1]))*256)+(+d[2]))*256)+(+d[3]);
}

function num2dot(int) 
{
    var part1 = int & 255;
    var part2 = ((int >> 8) & 255);
    var part3 = ((int >> 16) & 255);
    var part4 = ((int >> 24) & 255);

    return part4 + "." + part3 + "." + part2 + "." + part1;
}
export default {
    components:{
        VueDragResize
    },
    mounted(){
        this.reloadInterface()
        ipcRenderer.on('ipAnnounce', (event, val) => {
            if(val.err===0){
                var item=val.data
                item.port=val.addr.port
                item.ip=val.addr.address
                this.deviceList.push(item)
            }
        })
    },
    destroyed(){
        ipcRenderer.removeAllListeners('ipAnnounce')
    },
    data(){
        return{
            rules:{
                multiaddress:[
                    {pattern:/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/,message: '请输入正确的IP地址，eg:"192.168.1.1"', trigger: 'change'},
                ],
                eid: [
                    { pattern:/^[0-9a-fA-F]+$/,message: '请输入EID,HEX格式，eg:"0001020304"', trigger: 'change'},
                ],

            },
            rules1:{
                sa:[
                    { required: true, message: '源地址不能为空'},
                    { type: 'number', message: '源地址必须为数字值'}
                ]
            },
            dialogVisible:false,
            activeStatus:false,
            dHigh:300,
            dWidth:250,
            deviceList:[
                {
                    vin:'zengfu',
                }
            ],
            netLoading:false,
            NetInterface:{},
            setting:{
                interface:'',
                multiaddress:'',
                vin:'',
                eid:'',
            },
            activeItem:{},
            active:{
                type:'00',
                sa:''
            }
            
            
        }
    },
    methods:{
        activeConnect(){

        },
        reloadInterface(){
            this.netLoading=true
            this.NetInterface=ipcRenderer.sendSync('ip-interface')
            this.netLoading=false
        },
        bindAddress(){
            console.log(this.setting.interface)
            ipcRenderer.send('ip-bind',this.setting.interface.address)
            var netmask=dot2num(this.setting.interface.netmask)
            netmask^=0xffffffff
            this.setting.multiaddress=num2dot(dot2num(this.setting.interface.address)|netmask)
        },
        activeRouter(val){
            this.activeItem=val
            this.activeStatus=true;
        },
        changeSize(){
            if(this.dHigh===300){
                this.dHigh=40
            }else{
                this.dHigh=300
            }
        },
        resize() {
            
        },
        refresh(){
            if(this.setting.multiaddress===''){
                this.$notify.error({
                    title: 'Error',
                    message: "Must set a multicast address"
                })
                return
            }
            ipcRenderer.send('ip-refresh',this.setting)
        }
    }
}
</script>
<style>
 
 .item{
     margin: 10px;
     padding: 5px;
      border: solid 2px gray;
      border-radius: 5px;
      padding-bottom: 0px;
      
      
 }
.info{
     width: 100%;
     margin-bottom: 4px;
 }
 .divList{
     border: solid 4px whitesmoke;
     border-radius: 10px;
     overflow-y:auto;
     overflow-x:hidden;
     margin-right:-20px;
     width: 260px;
     z-index: 10;
     background-color: white;
 }
 .divList .deviceHeader{
     background-color: whitesmoke;
    border-top-left-radius: 10px;
    border-top-right-radius: 10px;
    margin: -4px;
    
 }
 .divList .list{
     padding: 12px;
     font-weight: bold;
     color: #409EFF
 }

 

</style>