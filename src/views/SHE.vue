<template>
    <div>
        <el-page-header @back="goBack" content="SHE GENERATE"  class="header" ></el-page-header>
        <div>
            <el-collapse v-model="activeName" accordion style="margin:50px">
                <el-collapse-item title="S32K1 CSEC SHE M1|M2|M3 Generate" name="1">
                    <el-form :model="k1she" :rules="rules" ref="k1she" label-width="120px" label-position="right" size="small">
                        <el-form-item label="Key Name:" prop="name">
                            <el-input v-model="k1she.name" style="width:300px"></el-input>
                        </el-form-item>
                        <el-form-item label="Auth Key ID:" prop="authKeyId">
                            <el-input v-model="k1she.authKeyId" style="width:100px"  maxlength="2" show-word-limit></el-input>
                        </el-form-item>
                        <el-form-item label="Auth Key Vaue:" prop="authKeyValue">
                            <el-input v-model="k1she.authKeyValue"   placeholder="hex" maxlength="32" show-word-limit></el-input>
                        </el-form-item>
                        <el-form-item label="New Key ID:" prop="keyId" >
                            <el-input v-model="k1she.keyId" style="width:100px"  maxlength="2" show-word-limit></el-input>
                        </el-form-item>
                        <el-form-item label="New Key Vaue:" prop="keyValue">
                            <el-input v-model="k1she.keyValue"  placeholder="hex" maxlength="32" show-word-limit></el-input>
                        </el-form-item>
                        <el-form-item label="Flags:" prop="flag">
                            <el-checkbox-group v-model="k1she.flag">
                            <el-checkbox label="WRITE_PROT" name="type"></el-checkbox>
                            <el-checkbox label="BOOT_PROT" name="type"></el-checkbox>
                            <el-checkbox label="DEBUG_PROT" name="type"></el-checkbox>
                            <el-checkbox label="KEY_USAGE" name="type"></el-checkbox>
                            <el-checkbox label="WILD_CARD" name="type"></el-checkbox>
                            <el-checkbox label="VERIFY_ONLY" name="type"></el-checkbox>
                            </el-checkbox-group>
                        </el-form-item>
                        <el-form-item label="UID:" prop="uid">
                            <el-input v-model="k1she.uid"  placeholder="hex" maxlength="30" show-word-limit></el-input>
                        </el-form-item>
                        <el-form-item label="Counter:" prop="counter">
                            <el-input-number v-model="k1she.counter" controls-position="right"  :min="1" ></el-input-number>
                        </el-form-item>
                        <el-form-item style="text-align:right">
                            <el-button type="primary" @click="k1Cal('k1she')">Calculate</el-button>
                        </el-form-item>
                       
                    </el-form>
                    
                </el-collapse-item>
                <!-- <el-collapse-item title="S32K3 HSE M1|M2|M3 Generate" name="2">
                    <div>控制反馈：通过界面样式和交互动效让用户可以清晰的感知自己的操作；</div>
                    <div>页面反馈：操作后，通过页面元素的变化清晰地展现当前状态。</div>
                </el-collapse-item> -->
                <div>
                    <el-button type="text" icon="el-icon-document-copy" class="btn"  :data-clipboard-text="mma"></el-button>
                    <el-input
                        type="textarea"
                        autosize
                        placeholder=""
                        v-model="mma">
                    </el-input>
                </div>
            </el-collapse>
            
        </div>
    </div>
</template>
<script>
const { ipcRenderer } = require('electron')
const ClipboardJS = require('clipboard/dist/clipboard.js')

const sprintf = require('sprintf-js').sprintf
export default {
    mounted(){
        this.clipboard = new ClipboardJS('.btn');
        this.clipboard.on('success',()=>{
            this.$notify({
                title: '成功',
                message: '复制成功',
                type: 'success'
            });
        })
    },
    data(){
        return{
            clipboard:'',
            activeName:'1',
            k1she:{
                name:'test',
                authKeyId:'2',
                authKeyValue:'492c29292b031f4571a870207c47056e',
                keyId:'1',
                keyValue:'492c29292b031f4571a870207c47056e',
                flag:['WRITE_PROT'],
                // flag:{
                //     WRITE_PROT:false,
                //     BOOT_PROT:false,
                //     DEBUG_PROT:false,
                //     KEY_USAGE:false,
                //     WILD_CARD:false,
                //     VERIFY_ONLY:false
                // },
                counter:1,
                uid:'492c29292b031f4571a870207c4705'
            },
            mma:'/* S32K1 CSEC M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n',
            rules: {
                name:[
                    { required: true, message: '请输入KEY的名字', trigger: 'blur' },
                ],
                authKeyId: [
                    { required: true,message: '请输入用于验证的KEY-ID，必须是数字', trigger: 'blur' },
                    
                ],
                authKeyValue: [
                    { required: true, pattern:/^[0-9a-fA-F]+$/,message: '请输入用于验证的KEY值，eg:"0001020304"', trigger: 'change'},
                    { min: 32, max: 32, message: 'KEY的长度为16个字节', trigger: 'blur' }
                ],
                keyId: [
                    { required: true,message: '请输入用于验证的KEY-ID，必须是数字', trigger: 'blur' },
                ],
                keyValue: [
                    { required: true, pattern:/^[0-9a-fA-F]+$/,message: '请输入用于验证的KEY值，HEX格式的形式，eg:"0001020304"', trigger: 'change'},
                    { min: 32, max: 32, message: 'KEY的长度为16个字节', trigger: 'blur' }
                ],
                uid: [
                    { required: true, pattern:/^[0-9a-fA-F]+$/,message: '请输入用于验证的KEY值，eg:"0001020304"', trigger: 'change'},
                    { min: 30, max: 30, message: 'UID的长度为15个字节', trigger: 'blur' }
                ],
            }
        }
    },
    methods:{
        goBack(){
            this.$router.push('/')
        },
        k1Cal(formName) {
            this.mma='/* S32K1 CSEC M1M2M3 generated by ECUBus,https://github.com/frankie-zeng/ECUBus*/\r\n'
            this.$refs[formName].validate((valid) => {
                if (valid) {
                    //console.log(this.k1she)
                   var a
                   var i
                   var mstr=''
                   var m=ipcRenderer.sendSync('k1SHEGen', this.k1she)
                   this.mma+=sprintf('/* %s M1,M2,M3 */\r\n',this.k1she.name)
                   /* M1 */
                   a=[]
                   mstr=m.M1.toString('hex')
                   for(i=0;i<mstr.length/2;i++){
                       a.push('0x'+mstr.slice(i*2,(i+1)*2))
                   }
                   this.mma+=sprintf('uint8_t %sM1[16]=[%s];\r\n',this.k1she.name,a.toString('hex'))
                   /* M2 */
                   a=[]
                   mstr=m.M2.toString('hex')
                   for(i=0;i<mstr.length/2;i++){
                       a.push('0x'+mstr.slice(i*2,(i+1)*2))
                   }
                   this.mma+=sprintf('uint8_t %sM2[32]=[%s];\r\n',this.k1she.name,a.toString('hex'))
                   /* M3 */
                   a=[]
                   mstr=m.M3.toString('hex')
                   for(i=0;i<mstr.length/2;i++){
                       a.push('0x'+mstr.slice(i*2,(i+1)*2))
                   }
                   this.mma+=sprintf('uint8_t %sM3[16]=[%s];\r\n',this.k1she.name,a.toString('hex'))
                   this.mma+='/***************/\r\n'
                }
            })
        },
    }
}
</script>
<style>
    .header{
        margin: 30px;
    }
    .btn{
        position: absolute;
        z-index: 2;
        right: 60px;
    }
</style>