const k1she = require('./k1she.js')


console.log(k1she(
    1,//auth key id
    Buffer.from([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]), //auth key value
    4,//update key id
    Buffer.from([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]), //update key value
    {WRITE_PROT:true},//flag
    30,//cid
    Buffer.from([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]) //uid
    )
)