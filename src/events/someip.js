const path = require("path")
const { ipcMain } = require('electron')
const isDevelopment = process.env.NODE_ENV !== 'production'
const dllPath = isDevelopment ? path.join(__dirname,"..","public",'someip') : path.join(process.resourcesPath, 'someip')
const configFile = isDevelopment ? path.join(__dirname,"..","public",'someip','vsomeip.json') : path.join(process.resourcesPath,'someip','vsomeip.json')
process.env['VSOMEIP_CONFIGURATION'] = configFile;
const SOMEIP = require("../someip")
console.log(dllPath,configFile)
var app=new Map()
ipcMain.on('someipCreateApp', (event, arg) => {
    var routed= new SOMEIP(dllPath,arg.name)
    routed.CreateApp()
    app.set(arg.name,routed)
    if(arg.route){
        routed.IsRouting()
    }
})
ipcMain.on('someipStartApp',(event,arg)=>{
    app.get(arg).StartApp()
})



