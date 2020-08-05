/* eslint-disable no-unused-vars */
import {ipcMain} from 'electron'

const log = require('electron-log');



  
ipcMain.on('logLevel',(event,arg)=>{
    if(arg=='false')
        arg=false
    log.transports.ipc.level=arg
})
  