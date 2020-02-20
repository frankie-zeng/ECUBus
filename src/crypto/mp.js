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
    // var constPad=Buffer.from('80000000000000000000000000000100','hex')
    // value=Buffer.concat([value,constPad])
    var l=value.length
    //var bitlen=l&8
    var blkSize=16
    var nblk=parseInt((l+blkSize-1)/blkSize)
    var outCur=Buffer.alloc(16,0)
    var xCur
    var outPre
    for(var i=0;i<nblk;i++){
        outPre=outCur
        xCur=value.slice(i*blkSize,(i+1)*blkSize)
        outCur=encrypt(outPre,xCur)
        outCur=bxor(outCur,bxor(xCur,outPre))
    }
    return outCur
}

module.exports = MPCompress