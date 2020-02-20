const k1she = require('./k1she.js')
const mp =require('./mp.js')

const chunks = [ '000102030405060708090a0b0c0d0e0f',
           '010153484500800000000000000000b0', ]

var inData=''

for(var i in chunks){
    inData+=chunks[i]
}

console.log(mp(Buffer.from(inData,'hex')))

console.log(k1she(
    1,//auth key id
    Buffer.from('000102030405060708090a0b0c0d0e0f','hex'), //auth key value
    4,//update key id
    Buffer.from('0f0e0d0c0b0a09080706050403020100','hex'), //update key value
    {},//flag
    1,//cid
    Buffer.from('000000000000000000000000000001','hex') //uid
    )
)