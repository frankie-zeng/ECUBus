const crypto = require('crypto')
const HSEnode = require('./../../build/Release/HSE.node')
const she = require('./lib/k3she')
const {aesCmac} = require('./lib/aes-cmac');
class HSE extends HSEnode.HSE{
    constructor(){
        super()
        this.sheKey={
            'MASTER_ECU_KEY':[0,1],
            'BOOT_MAC_KEY':[0,2],
            'BOOT_MAC':[0,3],
            'RAM_KEY':[0,14],
            'KEY1':[0,4],
            'KEY2':[0,5],
            'KEY3':[0,6],
            'KEY4':[0,7],
            'KEY5':[0,8],
            'KEY6':[0,9],
            'KEY7':[0,10],
            'KEY8':[0,11],
            'KEY9':[0,12],
            'KEY10':[0,13],
            'KEY11':[1,4],
            'KEY12':[1,5],
            'KEY13':[1,6],
            'KEY14':[1,7],
            'KEY15':[1,8],
            'KEY16':[1,9],
            'KEY17':[1,10],
            'KEY18':[1,11],
            'KEY19':[1,12],
            'KEY20':[1,13],
            'KEY21':[2,4],
            'KEY22':[2,5],
            'KEY23':[2,6],
            'KEY24':[2,7],
            'KEY25':[2,8],
            'KEY26':[2,9],
            'KEY27':[2,10],
            'KEY28':[2,11],
            'KEY29':[2,12],
            'KEY30':[2,13],
            'KEY31':[3,4],
            'KEY32':[3,5],
            'KEY33':[3,6],
            'KEY34':[3,7],
            'KEY35':[3,8],
            'KEY36':[3,9],
            'KEY37':[3,10],
            'KEY38':[3,11],
            'KEY39':[3,12],
            'KEY40':[3,13],
            'KEY41':[4,4],
            'KEY42':[4,5],
            'KEY43':[4,6],
            'KEY44':[4,7],
            'KEY45':[4,8],
            'KEY46':[4,9],
            'KEY47':[4,10],
            'KEY48':[4,11],
            'KEY49':[4,12],
            'KEY50':[4,13],
        }
    }
    getSSLVersion(){
        return this.getVersion();
    }
    GET_CATALOG_ID(keyHandle){
        return ((keyHandle>>16)&0xff)
    }
    GET_GROUP_IDX(keyHandle){
        return ((keyHandle>>8)&0xff)
    }
    GET_SLOT_IDX(keyHandle){
        return (keyHandle&0xff)
    }
    GET_KEY_HANDLE(catalog,group,slot){
        return ((catalog&0xff)<<16)|((group&0xff)<<8)|(slot&0xff)
    }
    gmac(key,value,iv=Buffer.from('f7a093cd1ce82f11bc318b3e','hex')){
        var keyLengthToCipher = { 16: 'aes-128-gcm', 24: 'aes-192-gcm', 32: 'aes-256-gcm' };
        if (!keyLengthToCipher[key.length]) {
            throw new Error('Keys must be 128, 192, or 256 bits in length.');
        }
        var cipher = crypto.createCipheriv(keyLengthToCipher[key.length], key, iv);
        cipher.setAAD(value)
        cipher.final()
        return cipher.getAuthTag()
    }
    cmac(key,value){
        return aesCmac(key, value, { returnAsBuffer: true })
    }
    importSheKey(offset,keyAuthId,keyAuthValue,keyId, keyValue,flag,cid,uid){
        var ret=she(keyAuthId,keyAuthValue,keyId, keyValue,flag,cid,uid)
        return this.importSubSheKey(offset,keyId[0],ret.M1,ret.M2,ret.M3)
    }
    genSheMartix(keyAuthId,keyAuthValue,keyId, keyValue,flag,cid,uid){
        return she(keyAuthId,keyAuthValue,keyId, keyValue,flag,cid,uid)
    }
    setDebugAuthMode(offset,challenge){
        var buf=Buffer.alloc(this.ATTR_DEBUG_MODE_LEN)
        if(challenge==true)
            buf.writeUInt8(this.HSE_DEBUG_AUTH_MODE_CR)
        else
            buf.writeUInt8(this.HSE_DEBUG_AUTH_MODE_PW)
        
        return this.setAttr(offset,this.HSE_DEBUG_AUTH_MODE_ATTR_ID,buf)
    }
    setDebugKey(offset,key){
        var buf=Buffer.alloc(this.ATTR_ADKP_LEN)
        key.copy(buf,0,0,16)
        return this.setAttr(offset,this.HSE_APP_DEBUG_KEY_ATTR_ID,buf)
    }
    setLifeCycle(offset,lc){
        var buf=Buffer.alloc(offset,this.ATTR_LC_LEN)
        buf.writeUInt8(lc,0)
        return this.setAttr(offset,this.HSE_SECURE_LIFECYCLE_ATTR_ID,buf)
    }
    setBootAuth(offset,enable){
        var buf=Buffer.alloc(this.ATTR_BOOT_AUTH_LEN)
        if(enable){
            buf.writeUInt8(this.HSE_IVT_AUTH,0)
        }else{
            buf.writeUInt8(this.HSE_IVT_NO_AUTH,0)
        }
        return this.setAttr(offset,this.HSE_ENABLE_BOOT_AUTH_ATTR_ID,buf)
    }
    setCustSecurityPolicy(offset,enADKm,startAsUser){
        var buf=Buffer.alloc(this.ATTR_EXTEND_CUST_SECURITY_POLICY_LEN,0)
        if(enADKm)
            buf.writeUInt8(1,0)
        else
            buf.writeUInt8(0,0)
        if(startAsUser)
            buf.writeUInt8(1,1)
        else
            buf.writeUInt8(0,1)
        return this.setAttr(offset,this.HSE_EXTEND_CUST_SECURITY_POLICY_ATTR_ID,buf)
    }
    setOemSecurityPolicy(offset,startAsUser){
        var buf=Buffer.alloc(this.ATTR_EXTEND_OEM_SECURITY_POLICY_LEN)
        if(startAsUser)
            buf.writeUInt8(1,0)
        else
            buf.writeUInt8(0,0)
        return this.setAttr(offset,this.HSE_EXTEND_OEM_SECURITY_POLICY_ATTR_ID,buf)
    }
    setMu(offset,muConfig=[]){
        var buf=Buffer.alloc(this.ATTR_MU_CONFIG_LEN)
        var len=this.ATTR_MU_CONFIG_LEN/this.ATTR_MU_INST_CONFIG_LEN
        if(muConfig.length!=len){
            return{
                err:-1,
                data:Buffer.alloc(0),
                msg:'MU config length does not equal HSE MU lenght'
            }
        }else{
            for(let i=0;i<len;i++){
                buf.writeUInt8(muConfig[0].muConfig,i*this.ATTR_MU_INST_CONFIG_LEN)
                buf.writeUInt8(muConfig[0].xrdcDomainId,i*this.ATTR_MU_INST_CONFIG_LEN+1)
                buf.writeInt16LE(muConfig[0].sharedMemChunkSize,i*this.ATTR_MU_INST_CONFIG_LEN+2)
            }
        }
        return this.setAttr(offset,this.HSE_MU_CONFIG_ATTR_ID,buf)
    }
    // eslint-disable-next-line no-unused-vars
    setTamper(offset,enable){
        return{
            err:-1,
            data:Buffer.alloc(0),
            msg:'Unspport service'
        }
    }
    setFirc(offset,divider)
    {
        var buf=Buffer.alloc(this.ATTR_FIRC_LEN)
        buf.writeUInt8(divider,0)
        return this.setAttr(offset,this.HSE_FIRC_DIVIDER_CONFIG_ATTR_ID,buf)
    }
    /**
     * 
     * @param {buffer} key RAW ADKP
     * @returns {buffer}
     */
    calADKPExtension(key){
        const hash = crypto.createHash('sha256')
        hash.update(key)
        return hash.digest()
    }
    calADKPDeviceDep(key,uid){
        const hash1 = crypto.createHash('sha256')
        const hash2 = crypto.createHash('sha256')
        hash1.update(key)
        var realkey=hash1.digest()
        hash2.update(uid)
        var content=hash2.digest()
        const cipher = crypto.createCipheriv('aes-256-ecb',realkey, null);
        cipher.setAutoPadding(false)
        var ret=cipher.update(content)
        cipher.final()
        return ret
    }

}
module.exports = HSE