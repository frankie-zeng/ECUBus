<template>
  <div>
      <div>
        <el-dialog title="选择一个地址" :visible.sync="showAddr"  center>
        <el-select
            v-model="addrIndex"
            placeholder="请选择"
            style="width:100%"
            size="mini"
        >
            <el-option v-for="(item,key) in addrTable" :key="key" :label="item.name" :value="key">
            <span style="float: left">{{ item.name }}</span>
            <span
                style="float: right; color: #8492a6; font-size: 13px"
            >SA:{{ item.SA}},TA:{{item.TA}}</span>
            </el-option>
        </el-select>
        <span slot="footer">
            <el-button icon="el-icon-folder" @click="realImport"  size="mini"  type="success">导入</el-button>
        </span>
        </el-dialog>
      </div>
        <el-button icon="el-icon-folder" @click="importConfig"  size="mini"  type="info"  >导入配置</el-button>
        <el-button icon="el-icon-share" @click="exportConfig" size="mini"  type="info"  >导出配置</el-button>
  </div>
</template>

<script>
const { ipcRenderer } = require('electron')
export default {
    data(){
        return{
            showAddr:false,
            addrIndex:''
        }
    },
    computed: {
        udsTable: function () {
            if(this.mode==='doip'){
                return this.$store.state.doipTable
            }else if(this.mode==='can'){
                return this.$store.state.canTable
            }else{
                return []
            }
        },
        addrTable: function () {
            if(this.mode==='doip'){
                return this.$store.state.doipAddrTable
            }else if(this.mode==='can'){
                return this.$store.state.canAddrTable
            }else{
                return []
            }
        },
    },
    props:['mode'],
    methods:{
        importConfig(){
            this.showAddr=true
        },
        realImport(){
            if(this.addrTable[this.addrIndex]){
                var file=ipcRenderer.sendSync('readFile')
                var table=JSON.parse(file)
                for(var i in table){
                    table[i].addr=this.addrTable[this.addrIndex]
                }
                if(this.mode==='doip'){
                    this.$store.commit('doipTableLoad', table)
                }else if(this.mode==='can'){
                    this.$store.commit('canTableLoad', table)
                }else{
                    return
                }
                this.showAddr=false
            }
        },
        exportConfig(){
            var val=JSON.parse(JSON.stringify(this.udsTable))
            for(var i in val){
                delete val[i]['addr']
            }
            var file=ipcRenderer.sendSync('saveFile', JSON.stringify(val))
            this.$notify({
                title: '成功',
                message: '保存成功：'+file,
                type: 'success'
            });
        }
    }
}
</script>

<style>
</style>