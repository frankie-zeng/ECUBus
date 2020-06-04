const config=require("./service.js")
var a=config['uds']
var o={
    'suppress':false
}
for(var i in a){
    for(var j in a[i].input){
        o[a[i].input[j].name]=''
    }
}
console.log(o)