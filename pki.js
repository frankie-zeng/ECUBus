const { pki, asn1, util } = require('node-forge')
const fs = require('fs')
const ASN1 = require('@lapo/asn1js');
const Hex = require('@lapo/asn1js/hex');


var pem = fs.readFileSync('cert.pem')
var cert = pki.certificateFromPem(pem);
console.log(cert.publicKey.e)
var asn1Cert = pki.certificateToAsn1(cert)
var der = asn1.toDer(asn1Cert)
var hex=der.toHex()
console.log(hex)
console.log(ASN1.decode(Hex.decode(hex)))

// var a=asn1.fromDer(der)

// function allLen(asn1){

// }

// function keyOffset(obj){
//     //follow rfc2459
//     var res=[]
//     var val;
//     var offset=0
//     offset+=obj.tlv;
//     offset+=obj.value[0].tlv;
//     val=obj.value[0].value;
//     offset+=val[0].tlv+val[0].valLen
//     offset+=val[1].tlv+val[1].valLen
//     offset+=val[2].tlv+val[2].valLen
//     offset+=val[3].tlv+val[3].valLen
//     offset+=val[4].tlv+val[4].valLen
//     offset+=val[5].tlv+val[5].valLen
//     offset+=val[6].tlv
//     val=val[6].value//subjectPublicKeyInfo
//     offset+=val[0].tlv+val[0].valLen//add algorithm of public key
//     offset+=val[1].tlv
//     val=val[1].value
   
//     offset+=val[0].tlv
//     val=val[0].value//public key
//     offset+=val[0].tlv//n
//     res.push(offset+1)
//     offset+=val[0].valLen
//     offset+=val[1].tlv//e
//     res.push(offset+1)
//     return res
// }
// var res=keyOffset(a)
// console.log(res)
// console.log(data.readUInt8(res[1]))
//console.log(ASN1.decode(Hex.decode(der.toHex())))

