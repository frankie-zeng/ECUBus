/* eslint-disable no-empty */
/* eslint-disable no-undef */
/* eslint-disable no-unused-vars */
const os = require('os')
const { ipcMain } = require('electron')
const sprintf = require('sprintf-js').sprintf
const fs = require('fs')
const { decodeTable, payload2data } = require('./decode.js')

class LPUDS {
    constructor(win) {
        this.win = win
        this.timeout = 2000
        this.sDelay = 100
        this.wait=false
        ipcMain.on('lpudsExcute', (event, arg) => {
            this.sDelay = arg.sDelay
            this.timeout = arg.timeout
            this.udsTable = arg.udsTable
            this.subTable = []
            this.addr = arg.addr
            this.startTime = new Date().getTime()
            this.step()
        })
        ipcMain.on('lpReceive',(event,arg)=>{
            if(this.wait){
                this.wait=false;
                try {
                    if (this.checkFunc(this.writeData, arg)) {
                            this.step()
                    } else {
                        this.emit('udsError', sprintf("[error]:User defined function return false,used time:%d\r\n", new Date().getTime() - this.startTime))
                    }
                } catch (error) {
                    this.emit('udsError', sprintf("[error]:User defined function syntax error,%s,used time:%d\r\n", error.message, new Date().getTime() - this.startTime))
                }
            }
        })
    }
    /*user call*/
    delay(timeout) {
    }
    log(msg) {
        this.emit('udsData', JSON.stringify(msg) + '\r\n')
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
    insertItem(service, payload, func = (writeData,readData)=>{return true}) {
        this.subTable.unshift({
            func: func,
            payload: payload,
            service: service
        })
    }
    /*end user call*/
    emit(channel, msg) {
        this.win.webContents.send(channel, msg)
    }
    step() {
        if ((this.udsTable.length == 0) && (this.subTable.length == 0)) {
            this.emit('udsEnd', sprintf("[done]:Excute successful,used time:%dms\r\n", new Date().getTime() - this.startTime))
            return 0
        }
        if (this.subTable.length == 0) {
            this.subTable = decodeTable(this.udsTable.shift())
        }
        var item = this.subTable.shift()
        if (typeof item.func === 'string') {
            try {
                // eslint-disable-next-line no-eval
                this.checkFunc = eval('(writeData,readData)=>{' + item.func + '}')
            } catch (error) {
                // eslint-disable-next-line no-eval
                this.checkFunc = eval('(writeData,readData)=>{return true}')
            }
        } else {
            this.checkFunc = item.func
        }
        this.writeData = item.payload
        var suppress = false
        var data = [item.service]


        data = data.concat(payload2data(item.payload))
        // console.log(data)
        // return 1
        /* check surpress*/
        for (var i in this.writeData) {
            if (this.writeData[i].type == 'subfunction') {
                if (this.writeData[i].suppress) {
                    suppress = true
                    setTimeout(()=>{
                        this.step()
                    },this.sDelay)
                }
                break
            }
        }
        this.emit('lpSend',data)
        this.wait=true
        return 1
    }
}


module.exports = LPUDS