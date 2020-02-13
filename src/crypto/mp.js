const crypto = require('crypto')

const h0=Buffer.from([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])

const encrypt = function (key, data) {
    var cipher = crypto.createCipheriv('aes-128-ecb', key,null)
    var crypted = cipher.update(data)
    cipher.final()
    return crypted
};
const bxor=function(a,b){
    var c=Buffer.alloc(a.length)
    for(var i=0;i<a.length;i++){
        c[i]=(a[i]^b[i])
    }
    // console.log(ac)
    return c
}
function MPCompress(value){
    var l=value.length
    var blkSize=16
    var nblk=parseInt((l+blkSize-1)/blkSize)
    var outCur=h0
    var dblk
    var xCur
    var outPre
    for(var i=0;i<nblk;i++){
        outPre=outCur
        dblk=value.slice(i*blkSize,(i+1)*blkSize)
        if(dblk.length<blkSize){
            xCur=dblk.concat(Buffer.alloc(blkSize-dblk.length).fill(0))
        }else{
            xCur=dblk
        }
        outCur=encrypt(outPre,dblk)
        //outCur=bxor(outCur,bxor(xCur,outPre))
        outCur=bxor(outCur,xCur)
        outCur=bxor(outCur,outPre)
    }
    return outCur
}

module.exports = MPCompress