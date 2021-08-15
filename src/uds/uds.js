/* eslint-disable no-unused-vars */
'use strict';
const fs = require('fs')
const { payload2data, decodeTable } = require('./decode.js')
const elelog = require('electron-log');
const hse = require('./../hse');
const {app} = require('electron');
const path = require('path');
const isDevelopment = process.env.NODE_ENV !== 'production'
/**
 * @class UDS
 */
class UDS {
    constructor(win) {
        this.win = win
        this.map = {}
        this.keeyMap = {}
        this.hse = new hse(app.getPath("appData"),isDevelopment?path.join(__dirname,'..','src','hse'):process.resourcesPath);
    }
    emit(channel, msg) {
        if (channel == 'udsError') {
            this.error(msg)
        } else if (channel == 'udsEnd') {
            this.info(msg)
        }
        this.win.webContents.send(channel, msg)
    }
    progress(show, percent, name = 'main') {
        this.emit('progress', {
            name: name,
            show: show,
            percent: percent
        })
    }
    set(key, value, keep = false) {
        this.map[key] = value
        if (keep) {
            this.keeyMap[key] = value
        }
    }
    get(key) {
        if (key in this.map)
            return this.map[key]
        else if (key in this.keeyMap)
            return this.keeyMap[key]
        else
            return null
    }
    verbose(msg) {
        elelog.verbose(JSON.stringify(msg))
    }
    error(msg) {
        elelog.error(JSON.stringify(msg))
    }
    info(msg) {
        elelog.info(JSON.stringify(msg))
    }
    debug(msg) {
        elelog.debug(JSON.stringify(msg))
    }
    log(msg, type = 'debug') {
        elelog[type](JSON.stringify(msg))
        //this.emit('udsData', JSON.stringify(msg) + '\r\n')
    }
    openFile(filename, flag = 'r') {
        this.fd = fs.openSync(filename, flag)
    }
    readFile(size) {
        var buf = Buffer.alloc(size)
        var len = fs.readSync(this.fd, buf, 0, size)
        return [...buf.slice(0, len)]
    }
    writeFile(data) {
        fs.writeSync(this.fd, Buffer.from(data))
    }
    closeFile() {
        fs.closeSync(this.fd)
    }
    changeNextFrame(name, value = []) {
        if ((this.subTable.length == 0) && (this.udsTable.length > 0)) {
            this.subTable = decodeTable(this.udsTable.shift())
        }
        if (this.subTable.length > 0) {
            for (var i in this.subTable[0].payload) {
                if (this.subTable[0].payload[i].name == name) {
                    this.subTable[0].payload[i][name] = value
                    break
                }
            }
        }
    }
    insertItem(service, payload, func = (writeData, readData) => { return true }, preFunc = (writeData) => { }) {
        this.subTable.unshift({
            func: func,
            preFunc: preFunc,
            payload: payload,
            service: service
        })
    }
    UDSstart(udsTable) {
        this.udsTable = udsTable
        this.allLen = udsTable.length
        this.subTable = []
        this.map = {}
    }
    getNextService() {
        if ((this.udsTable.length == 0) && (this.subTable.length == 0)) {
            return null
        }
        if (this.subTable.length == 0) {
            this.subTable = decodeTable(this.udsTable.shift())
        }
        this.tableIndex = this.allLen - this.udsTable.length
        var item = this.subTable.shift()
        var obj = {}
        if (typeof item.func === 'string') {
            try {
                // eslint-disable-next-line no-eval
                obj.checkFunc = eval('(writeData,readData)=>{' + item.func + '}')
            } catch (error) {
                // eslint-disable-next-line no-eval
                throw 'User function syntax error'
            }
        } else {
            obj.checkFunc = item.func
        }
        if (item.preFunc == undefined) {
            item.preFunc = ""
        }
        if (typeof item.preFunc === 'string') {
            try {
                // eslint-disable-next-line no-eval
                obj.preFunc = eval('(writeData)=>{' + item.preFunc + '}')
            } catch (error) {
                // eslint-disable-next-line no-eval
                throw 'User function syntax error'
            }
        } else {
            obj.preFunc = item.preFunc
        }

        obj.payload = item.payload
        obj.suppress = false
        for (var i in obj.payload) {
            if (obj.payload[i].type == 'subfunction') {
                if (obj.payload[i].suppress) {
                    obj.suppress = true
                }
                break
            }
        }
        obj.preFunc(obj.payload)
        var data = [item.service]
        obj.data = data.concat(payload2data(obj.payload))
        return obj
    }
}



module.exports = UDS;