const { ipcMain } = require('electron')
const opt = { silent: false};
const cp = require('child_process');

let someip = cp.fork('./src/someip/someip.js', [], opt);
let methods=[
    'someipCreateApp',
    'someipStartApp'
];
for(let i in methods){
    let method=methods[i]
    ipcMain.on(method, (event, arg) => {
        console.log("receive",method)
        someip.send({
            method:method,
            arg:arg
        })
    })
}

