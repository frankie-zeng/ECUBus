const fs = require('fs')
function subItem(item) {
    var rawdata = []
    rawdata.push(item.service.value)
    for (var i in item.payload) {
        if (item.payload[i].data) {
            rawdata = rawdata.concat(item.payload[i].data)
        }
    }

    return {
        func: item.func,
        payload: item.payload,
        service: item.service.value
    }
}
function decodeTable(item) {
    var data = []

    if (item.type === 'uds') {
        data.push(subItem(item))
    } else if (item.type === 'group') {
        for (var i in item.subtable) {
            if (item.subtable[i].type === 'uds') {
                data.push(subItem(item.subtable[i]))
            }
        }
    }
    return data
}

// eslint-disable-next-line no-unused-vars
function len(val, size) {
    size = parseInt(size)
    var buf = Buffer.alloc(size)
    var length = parseInt(val)
    for (var i = 0; i < size; i++) {
        buf.writeUInt8(length & 0xff, size - i - 1)
        length = length >> 8
    }
    return buf
}
// eslint-disable-next-line no-unused-vars
function equal(val) {
    return Buffer.from(val, 'hex')
}


function payload2data(payload) {
    var rawdata = []
    var $ = {}
    var i
    for (i in payload) {
        $[payload[i].name] = payload[i][payload[i].name]
    }
    console.log($)
    var buf
    for (i in payload) {
        if (payload[i].type === 'subfunction') {
            rawdata.push((parseInt(payload[i].subFunction) | (payload[i].suppress ? 0x80 : 0)))
        } else if (payload[i].type === "downloadFile" || payload[i].type === "uploadFile") {
            if (payload[i].type === "downloadFile") {
                if ($[payload[i].name].name)
                    $[payload[i].name].size = fs.statSync($[payload[i].name].name).size
            }
        } else if (payload[i].type === "input") {
            if (
                payload[i][payload[i].name] &&
                payload[i][payload[i].name] != ""
            ) {
                let reg = /^(.*?)\((.*?)\)$/
                if ((reg.test(payload[i][payload[i].name]) == true) && (Array.isArray(payload[i][payload[i].name]) == false)) {
                    let final = payload[i][payload[i].name]
                    try {
                        buf = eval(final)
                        payload[i][payload[i].name] = buf.toString('hex')
                        $[payload[i].name] = buf.toString('hex')
                    } catch (error) {
                        console.log(error)
                        buf = Buffer.alloc(0)
                    }
                } else {
                    buf = Buffer.from(
                        payload[i][payload[i].name],
                        "hex"
                    );
                }
                rawdata = rawdata.concat([...buf]);
            }
        } else if (payload[i].type === 'text') {
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
        } else if ((payload[i].type === 'number') || (payload[i].type === 'select')) {
            if (
                typeof (payload[i][payload[i].name]) != 'undefined'
            ) {

                rawdata.push(payload[i][payload[i].name]);
            }
        }
    }
    return rawdata

}
module.exports = { decodeTable, payload2data }