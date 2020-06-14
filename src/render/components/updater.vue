
<template>
    <div>
        <el-dialog
            title="Upgrade Progress"
            :visible.sync="upgrade"
            width="30%"
            :before-close="closeUpgrade">
            <div style="text-align:center">
                <el-progress type="circle" :percentage="percent" :width="256" :stroke-width="26"></el-progress>
                <div style="text-align:center;width:100%">
                    {{speed}} KB/s
                </div>
            </div>
        </el-dialog>
    </div>
</template>
<script>
/* eslint-disable no-unused-vars */
const { ipcRenderer } = require("electron");
const { log } = require("electron-log");
export default {
    data(){
        return{
            upgrade:false,
            speed:'',
            total:0,
            percent:0
        }
    },
    methods:{
        closeUpgrade(done){
            this.$confirm('Cancel Upgrade?')
          .then(_ => {
            ipcRenderer.send("cancelUpdate");
            done();
          })
          .catch(_ => {});
        }
    },
    mounted() {
        ipcRenderer.on("update-available", (event, val) => {
            this.$confirm('Found Version:'+val.version, 'New Version', {
                confirmButtonText: 'Upgrade',
                cancelButtonText: 'Cancel',
                type: 'warning'
            }).then(() => {
                this.upgrade=true
                ipcRenderer.send("startUpdate");
            }).catch(() => {
            });
        });
        ipcRenderer.on("update-not-available", (event, val) => {
            this.upgrade=false
            this.$notify.info({
                title: 'Upgrade',
                message: "This Is Newest Version!"
            });
        });
        ipcRenderer.on("update-error", (event, val) => {
            this.upgrade=false
            this.$notify.info({
                title: 'Upgrade',
                message: val.EPERM
            });
        });
        ipcRenderer.on("update-download-progress", (event, val) => {
            this.upgrade=true
            this.total=val.total
            this.speed=parseFloat((val.bytesPerSecond/1024).toFixed(2))
            this.percent=parseFloat(val.percent.toFixed(2))
        });
        ipcRenderer.on("update-downloaded", (event, val) => {
            this.upgrade=false
            this.$confirm('Download '+val.version+' OK, Upgrade Now?', {
                confirmButtonText: 'Now',
                cancelButtonText: 'Later',
                type: 'success'
            }).then(() => {
                this.upgrade=true
                ipcRenderer.send("installUpdate");
            }).catch(() => {
            });
        });
    },
    destroyed() {
        ipcRenderer.removeAllListeners("update-available");
        ipcRenderer.removeAllListeners("update-not-available");
        ipcRenderer.removeAllListeners("update-error");
        ipcRenderer.removeAllListeners("update-download-progress");
        ipcRenderer.removeAllListeners("update-downloaded");
    },
}
</script>