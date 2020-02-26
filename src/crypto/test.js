const k1she = require('./k1she.js')
const k3she = require('./k3she.js')



console.log(k1she(
    1,//auth key id
    Buffer.from('000102030405060708090a0b0c0d0e0f', 'hex'), //auth key value
    4,//update key id
    Buffer.from('0f0e0d0c0b0a09080706050403020100', 'hex'), //update key value
    {},//flag
    1,//cid
    Buffer.from('000000000000000000000000000001', 'hex') //uid
)
)
console.log(k3she(
    [1,1],//auth key id
    Buffer.from('000102030405060708090a0b0c0d0e0f', 'hex'), //auth key value
    [4,4],//update key id
    Buffer.from('0f0e0d0c0b0a09080706050403020100', 'hex'), //update key value
    {},//flag
    1,//cid
    Buffer.from('0000000000000001', 'hex') //uid
)
)