const MP = require('./mp.js')
const crypto = require('crypto')
const aesCmac = require('node-aes-cmac').aesCmac;

const KEY_UPDATE_ENC_C='010153484500800000000000000000B0'
const KEY_UPDATE_MAC_C='010253484500800000000000000000B0'


function KDF(k,constant){
    var c=Buffer.concat([k,constant],k.length+constant.length)
    return MP(c)
}


function genM1M2M3(keyAuthId,keyAuthValue,keyId,keyValue,flag,cid,uid){
    var bM1=Buffer.alloc(16,0)
    var bM2=Buffer.alloc(32,0)
    var k1=KDF(keyAuthValue,Buffer.from(KEY_UPDATE_ENC_C,'hex'))
    var k2=KDF(keyAuthValue,Buffer.from(KEY_UPDATE_MAC_C,'hex'))
    console.log(k1,k2)
    uid.copy(bM1)
    bM1[15]=(((keyId&0x0f)<<4)|(keyAuthId&0x0f))
    
    bM2.writeUInt32BE(((cid<<4)&0xfffffff0),0)
    var t=bM2.readUInt8(3)
    if(flag.WRITE_PROT){
        t|=0x8
    }
    if(flag.BOOT_PROT){
        t|=0x4
    }
    if(flag.DEBUG_PROT){
        t|=0x2
    }
    if(flag.KEY_USAGE){
        t|=0x1
    }
    bM2.writeUInt8(t,3)
    t=0
    if(flag.WILD_CARD){
        t|=0x80
    }
    if(flag.VERIFY_ONLY){
        t|=0x40
    }
    bM2.writeUInt8(t,8)
    keyValue.copy(bM2,16)
    //aes-cbc k1
    var aescbc = crypto.createCipheriv('aes-128-cbc', k1,Buffer.alloc(16,0))
    bM2 = aescbc.update(bM2)
    aescbc.final()
    //cmac k2
    var bM3=Buffer.from(aesCmac(k2,Buffer.concat([bM1,bM2],bM1.length+bM2.length)),'hex')

    


    return {
        M1:bM1,
        M2:bM2,
        M3:bM3
    }
}

module.exports = genM1M2M3