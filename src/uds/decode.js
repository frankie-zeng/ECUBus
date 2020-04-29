function subItem(item){
    var rawdata=[]
    rawdata.push(item.service.value)
    for (var i in item.payload) {
        if(item.payload[i].data){
            rawdata = rawdata.concat(item.payload[i].data)
        }
    }

    return {
        func:item.func,
        payload:item.payload,
        service:item.service.value
    }
}
function decodeTable(item){
    var data=[]
    
    if (item.type === 'uds') {
        data.push(subItem(item))
    }else if(item.type === 'group') {
        for (var i in item.subtable){
            if(item.subtable[i].type=== 'uds'){
                data.push(subItem(item.subtable[i]))
            }
        }
    }
    return data
}

function payload2data(payload){
    var rawdata=[]
    var buf
    for(var i in payload){
        if(payload[i].type==='subfunction'){
            rawdata.push((parseInt(payload[i].subFunction)|(payload[i].suppress?0x80:0)))
        }else if(payload[i].type === "downloadFile" || payload[i].type === "uploadFile"){
            //do nothing
        }else if(payload[i].type === "input"){
            if (
                payload[i][payload[i].name] &&
                payload[i][payload[i].name] != ""
              ) {
                buf = Buffer.from(
                    payload[i][payload[i].name],
                  "hex"
                );
                rawdata = rawdata.concat([...buf]);
              } 
        }else if(payload[i].type==='text'){
            if (
                payload[i][payload[i].name] &&
                payload[i][payload[i].name] != ""
              ) {
                buf = Buffer.from(
                    payload[i][payload[i].name],
                  "ascii"
                );
                rawdata = rawdata.concat([...buf]);
              } 
        }else if(payload[i].type==='number'){
            if (
                payload[i][payload[i].name] &&
                payload[i][payload[i].name] != ""
              ) {
                
                rawdata.push(payload[i][payload[i].name]);
              } 
        }
    }
    return rawdata

}
module.exports = {decodeTable,payload2data}