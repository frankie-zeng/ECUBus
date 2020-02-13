const crypto = require('crypto')


const encrypt = function (key, data) {
    var cipher = crypto.createCipheriv('AES-128-ECB', key, null)
    var crypted = cipher.update(data)
    cipher.final()
    return crypted
};
const bxor = function (bufferA, bufferB) {
    var length = Math.min(bufferA.length, bufferB.length);
    var output = Buffer.alloc(length);
    for (var index = 0; index < length; index++) {
      output[index] = bufferA[index] ^ bufferB[index];
    }
    return output;
  }
function MPCompress(value){
    var l=value.length
    var blkSize=16
    var nblk=parseInt((l+blkSize-1)/blkSize)
    var outCur=Buffer.alloc(16,0xff)
    var dblk
    var xCur
    var outPre
    for(var i=0;i<nblk;i++){
        outPre=outCur
        dblk=value.slice(i*blkSize,(i+1)*blkSize)
        if(dblk.length<blkSize){
            xCur=Buffer.concat([dblk,Buffer.alloc(blkSize-dblk.length).fill(0)],blkSize)
        }else{
            xCur=dblk
        }
        outCur=encrypt(outPre,dblk)
        outCur=bxor(outCur,bxor(xCur,outPre))
        //outCur=bxor(outCur,xCur)
        //outCur=bxor(outCur,outPre)
    }
    return outCur
}

module.exports = MPCompress