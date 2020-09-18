
const k1SHEGen = require('./../crypto/k1she.js')
const k2SHEGen = require('./../crypto/k2tvshe.js')
const k3SHEGen = require('./../crypto/k3she.js')
import { ipcMain } from 'electron'

ipcMain.on('k2SHEGen', (event, arg) => {
  var flag2 = function (flag) {
    var fo = {}
    for (var i in flag) {
      fo[flag[i]] = true
    }
    return fo
  }
  var ret = k2SHEGen(
    arg.authKeyId,//auth key id
    Buffer.from(arg.authKeyValue, 'hex'), //auth key value
    arg.keyId,//update key id
    Buffer.from(arg.keyValue, 'hex'), //update key value
    flag2(arg.flag),//flag
    parseInt(arg.counter),//cid
    Buffer.from(arg.uid, 'hex') //uid
  )

  event.returnValue = ret
})


ipcMain.on('k3SHEGen', (event, arg) => {
  var flag2 = function (flag) {
    var fo = {}
    for (var i in flag) {
      fo[flag[i]] = true
    }
    return fo
  }
  var ret = k3SHEGen(
    arg.authKeyId,//auth key id
    Buffer.from(arg.authKeyValue, 'hex'), //auth key value
    arg.keyId,//update key id
    Buffer.from(arg.keyValue, 'hex'), //update key value
    flag2(arg.flag),//flag
    parseInt(arg.counter),//cid
    Buffer.from(arg.uid, 'hex') //uid
  )

  event.returnValue = ret
})

ipcMain.on('k1SHEGen', (event, arg) => {
  var flag2 = function (flag) {
    var fo = {}
    for (var i in flag) {
      fo[flag[i]] = true
    }
    return fo
  }
  var ret = k1SHEGen(
    parseInt(arg.authKeyId),//auth key id
    Buffer.from(arg.authKeyValue, 'hex'), //auth key value
    parseInt(arg.keyId),//update key id
    Buffer.from(arg.keyValue, 'hex'), //update key value
    flag2(arg.flag),//flag
    parseInt(arg.counter),//cid
    Buffer.from(arg.uid, 'hex') //uid
  )

  event.returnValue = ret
})