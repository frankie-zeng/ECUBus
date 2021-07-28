/* eslint-disable no-unused-vars */
const { ipcMain } = require('electron')
const http = require('http');
const opt = { silent: true };
const cp = require('child_process');
const path = require('path');

let methods = [
    'someipCreateApp',
    'someipStartApp',
    'someipSendRequest',
    'someipSend',
    'someipDeleteRequest',
    'someipOffer',
    'someipStopOffer',
    'someipAddResponse',
    'someipDeleteResponse'
];

function PostRequest(port, method, data, event) {
    const vData=JSON.stringify(data)
    const options = {
        hostname: 'localhost',
        port: port,
        path: `/${method}`,
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Content-Length': vData.length
        }
    }

    const req = http.request(options, res => {
        var body = ''
        res.on('data', data => {
            body += data
        })
        res.on('end', () => {
            event.returnValue = JSON.parse(body)
        })
    })
    req.write(vData)
    req.end()
}

class SOMEIP {
    constructor(win) {
        this.win = win
        this.port = undefined
        //start sub default
        //this.startSub()
        ipcMain.on('someipStart', (event, arg) => {
            this.startSub()
        })
        ipcMain.on('someipStop', (event, arg) => {
            this.killSub()
        })
    }
    startSub() {
        const isDevelopment = process.env.NODE_ENV !== 'production'
        let subpath
        if(isDevelopment){
            subpath='./src/someip/subsomeip.js'
        }else{
            // eslint-disable-next-line no-undef
            subpath=path.join(process.resourcesPath,'someip','subsomeip.js')
        }
        // eslint-disable-next-line no-undef
        this.sub = cp.fork(subpath, [isDevelopment?__static:process.resourcesPath], opt);
        this.sub.on('message', (val) => {
            if(val.method=='port'){
                this.port = val.port
                for (let i in methods) {
                    let method = methods[i]
                    ipcMain.on(method, (event, arg) => {
                        // this.sub.send()
                        PostRequest(this.port, method, arg, event)
                    })
                }
                this.win.webContents.send('someip_start',this.port)
            }else if(val.method=='avl'){
                this.win.webContents.send('someip_avl',{
                    name:val.name,
                    service:val.service,
                    instance:val.instance,
                    avl:val.avl
                })
            }else if(val.method=='msg'){
                this.win.webContents.send('someip_msg',{
                    name:val.name,
                    msg:val.msg,
                })
            }
        })
        this.sub.stdout.on("data", (data) => {
            let level = 'info'
            if (Buffer.from(data).toString('ascii').search('error') > 0) {
                level = 'error'
            }else if (Buffer.from(data).toString('ascii').search('warning') > 0) {
                level = 'warning'
            }
            this.win.webContents.send('someip_log', {
                level: level,
                data: data
            })
        })
        this.sub.stderr.on("data", (data) => {
            console.error(data.toString('ascii'))
            this.win.webContents.send('someip_log', {
                level: 'error',
                data: data
            })
        })
    }
    killSub() {
        for (let i in methods) {
            let method = methods[i]
            ipcMain.removeAllListeners(method)
        }
        this.sub.kill('SIGINT')
    }

}
module.exports = SOMEIP




