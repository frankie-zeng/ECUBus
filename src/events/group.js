/* eslint-disable no-unused-vars */
import { app, ipcMain} from 'electron'
const fs = require('fs')
const path = require('path')
const group_file=path.join(app.getPath('userData'),'groupservice.js')

if(!fs.existsSync(group_file)){
    fs.writeFileSync(group_file,'[]')
  }
  
  
  ipcMain.on('readGroup',(event,arg)=>{
    event.returnValue =  fs.readFileSync(group_file)
  })
  ipcMain.on('saveGroup',(event,arg)=>{
    var err=0
    var map
    map = new Map(JSON.parse(fs.readFileSync(group_file)));
    map.set(arg[0],arg[1])
    fs.writeFileSync(group_file,JSON.stringify(Array.from(map.entries())))
    event.returnValue = err
  })