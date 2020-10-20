/* eslint-disable no-empty */
/* eslint-disable no-undef */
/* eslint-disable no-unused-vars */
const { ipcMain } = require('electron')
const sprintf = require('sprintf-js').sprintf
const UDS = require('./uds.js')
class LPUDS extends UDS {
    constructor(win) {
        super(win)
        this.timeout = 2000
        this.sDelay = 100
        this.wait = false
        ipcMain.on('lpudsExcute', (event, arg) => {
            this.sDelay = arg.sDelay
            this.timeout = arg.timeout
            this.UDSstart(arg.udsTable)
            this.addr = arg.addr
            this.step()
        })
        ipcMain.on('lpReceive', (event, arg) => {
            if (this.wait) {
                this.wait = false;
                try {
                    if ((arg[0] == 0x7F) && (arg[2] == 0x78)) {
                        this.wait = true;
                    } else {
                        if (this.checkFunc(this.writeData, arg)) {
                            this.step()
                        } else {
                            this.emit('udsError', {
                                msg: "User defined function return false",
                                index: this.tableIndex
                            })

                        }
                    }
                } catch (error) {
                    this.emit('udsError', {
                        msg:  sprintf("User defined function syntax error,%s", error.message),
                        index: this.tableIndex
                    })
                }
            }
        })
    }
    /*user call*/
    step() {
        try {
            var item = this.getNextService()
        } catch (e) {
            console.log(e)
            this.emit('udsError', {
                msg: "User defined function syntax error",
                index: this.tableIndex
            })
            return -1
        }
        if (item === null) {
            this.emit('udsEnd', "Excute successful")
            return 0
        }
        this.checkFunc = item.checkFunc
        this.writeData = item.payload
        var data = item.data
        if (item.suppress) {
            setTimeout(() => {
                this.step()
            }, this.sDelay)
        }
        this.emit('lpSend', data)
        this.wait = true
        return 1
    }
}


module.exports = LPUDS