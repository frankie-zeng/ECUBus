const { ipcMain } = require('electron')
const opt = { silent: true };
const cp = require('child_process');

let methods = [
    'someipCreateApp',
    'someipStartApp'
];
class SOMEIP {
    constructor(win) {
        this.win = win
        //start sub default
        //this.startSub()
        ipcMain.on('someipStart', (event, arg) => {
           this.startSub()
        })
        ipcMain.on('someipStop',(event, arg)=>{
            this.killSub()
        })
    }
    startSub(){
        this.sub = cp.fork('./src/someip/subsomeip.js', [__static], opt);
        for (let i in methods) {
            let method = methods[i]
            ipcMain.on(method, (event, arg) => {
                this.sub.send({
                    method: method,
                    arg: arg
                })
            })
        }
        this.sub.stdout.on("data",(data) => {
            this.win.webContents.send('someip_log',{
                level:'info',
                data:data
            })
        })
        this.sub.stderr.on("data",(data) => {
            this.win.webContents.send('someip_log',{
                level:'error',
                data:data
            })
        })
    }
    killSub(){
        this.sub.kill()
        for (let i in methods) {
            let method = methods[i]
            ipcMain.removeAllListeners(method)
        }
    }   

}
module.exports = SOMEIP


   

