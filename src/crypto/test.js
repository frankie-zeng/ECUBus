const k1she = require('./k1she.js')
const mp =require('./mp.js')

const chunks = [ '49d1f7d9244e04d8b0c127fb1bcbe6eb',
           '492c29292b031f4571a870207c47056e',
           '3cb78a42c91d2670595770bf82e12311',
           'daf675c03f910e7ca3bf321c8db2887a',
           '8c4ed5fff19b96d33a94b89ca526f966',
           '2e325ee1c6b5292d9c11c8d0dd6324fb',
           '186d73ea46a391530bf3eed47d4c3021',
           'f4c24a2df9decea22b64ac21daad7489',
           '385de0968d9847521f8a83d543b45ab3',
           '238b2a50b629791e6052f7942e66b4f1',
           '4b9e4ae11c0417981e5506af31e6aa26',
           '1c3f2f041b98c31fc8a4c074f05f5028',
           '9995b9ce908c75672b26f2d6ec82eed3',
           '0a2513fa9768a84ac6ec367a2dd058c9',
           '0a971ad2519d97942ebc860ba3ff00b2',
           'b01395871e47012903fd99066004878d' ]

var inData=''

for(var i in chunks){
    inData+=chunks[i]
}

console.log(mp(Buffer.from(inData,'hex')))

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