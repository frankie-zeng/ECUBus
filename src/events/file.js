/* eslint-disable no-unused-vars */
const { ipcMain, dialog } = require('electron')
const fs = require('fs')
const nodepath = require('path')
let udsProjectPath

ipcMain.on('readFile', (event, arg) => {
  var file = dialog.showOpenDialogSync({
    filters: [
      { name: 'JSON', extensions: ['json'] },
    ],
  })
  if (Array.isArray(file)) {
    event.returnValue = fs.readFileSync(file[0], 'utf8')
  } else {
    event.returnValue = ''
  }
})

ipcMain.on('udsProjectPath', (event, arg) => {
  udsProjectPath = arg
  global.udsProjectPath = arg
})


ipcMain.on('saveFile', (event, arg) => {
  var file = dialog.showSaveDialogSync({
    filters: [
      { name: 'JSON', extensions: ['json'] },
    ],
    defaultPath: udsProjectPath

  })
  if (typeof file === "string") {
    fs.writeFileSync(file, arg)
  }
  event.returnValue = file
})

ipcMain.on('saveFilePath', (event, arg) => {
  var path = ''
  var file = dialog.showSaveDialogSync({
    defaultPath: udsProjectPath
  })
  if (typeof file === "string") {
    path = nodepath.relative(udsProjectPath, file)
  }
  event.returnValue = path
})


ipcMain.on('downloadFilePath', (event, arg) => {
  var file = dialog.showOpenDialogSync({
    defaultPath: udsProjectPath
  })
  var size = 0
  var path = ''
  if (Array.isArray(file)) {
    path = nodepath.relative(udsProjectPath, file[0])
    size = fs.statSync(file[0]).size
  }
  event.returnValue = {
    path: path,
    size: size
  }
})


ipcMain.on('readCertDer', (event, arg) => {
  var pem = fs.readFileSync(arg, 'utf-8')
  event.returnValue = pem
})