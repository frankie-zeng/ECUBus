/* eslint-disable no-unused-vars */
const { ipcMain, dialog } = require('electron')
const fs = require('fs')
ipcMain.on('readFile', (event, arg) => {
  var file = dialog.showOpenDialogSync({
    filters: [
      { name: 'JSON', extensions: ['json'] },
    ],
  })
  if (Array.isArray(file)) {
    event.returnValue = fs.readFileSync(file[0],'utf8')
  } else {
    event.returnValue = ''
  }
})
ipcMain.on('saveFile', (event, arg) => {
  var file = dialog.showSaveDialogSync({
    filters: [
      { name: 'JSON', extensions: ['json'] },
    ],
  })
  if (typeof file === "string") {
    fs.writeFileSync(file, arg)
  }
  event.returnValue = file
})

ipcMain.on('saveFilePath', (event, arg) => {
  var path = ''
  var file = dialog.showSaveDialogSync()
  if (typeof file === "string") {
    path = file
  }
  event.returnValue = path
})


ipcMain.on('downloadFilePath', (event, arg) => {
  var file = dialog.showOpenDialogSync()
  var size = 0
  var path = ''
  if (Array.isArray(file)) {
    path = file[0]
    size = fs.statSync(file[0]).size
  }
  event.returnValue = {
    path: path,
    size: size
  }
})