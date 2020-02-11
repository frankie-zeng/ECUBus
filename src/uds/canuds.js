/* eslint-disable no-undef */
/* eslint-disable no-empty */
const path = require('path')


const fs = require('fs')
const { ipcMain } = require('electron')
const ErrorText = {
  0: 'OK!',
  1: 'Timeout occured between 2 frames transmission',
  2: 'Sender side timeout while waiting for flow control frame.',
  3: 'Receiver side timeout while waiting for consecutive frame.',
  4: 'Unexpected sequence number',
  5: 'Invalid or unknown FlowStatus',
  6: 'Unexpected protocol data unit'
}


const  PCANTP = require('./../../build/Release/PCANTP.node')
// const  PCANTP = require(path.join(__static, 'PCANTP.node'))
const isDevelopment = process.env.NODE_ENV !== 'production'
const dllPath = isDevelopment?path.join(__static, 'peak'):path.join(process.resourcesPath, 'peak')
class CANUDS {
  constructor (win) {
    this.win = win
    this.cantp = new PCANTP.CANTP(dllPath)
    this.canfd = false
    this.channel = PCANTP.PCANTP_USBBUS1
    this.suppress = false
    this.writeOk = false
    this.udsTimer = setTimeout(() => {}, 0)
    this.writeFile = (writeData, readData) => {
      this.fStream.write(new Uint8Array(readData))
      return true
    }
    clearTimeout(this.udsTimer)
    ipcMain.on('can-connect', (event, arg) => {
      var err = this.cantp.Initialize(arg[0], arg[1])
      this.canfd = false
      this.channel = arg[0]
      event.returnValue = { err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('can-connectFd', (event, arg) => {
      var err = this.cantp.InitializeFd(arg[0], arg[1])
      this.canfd = true
      this.channel = arg[0]
      event.returnValue = { err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('can-disconnect', (event, arg) => {
      var err = this.cantp.Uninitialize(arg)
      event.returnValue = { err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('can-addMap', (event, arg) => {
      var ret = {}
      ret.err = this.cantp.AddMapping(this.channel, arg.txId, arg.rxId, arg.IDTYPE, arg.FORMAT, arg.MSGTYPE, arg.SA, arg.TA, arg.TA_TYPE, arg.RA)
      if (ret.err === 0) {
        ret.err = this.cantp.AddMapping(this.channel, arg.rxId, arg.txId, arg.IDTYPE, arg.FORMAT, arg.MSGTYPE, arg.TA, arg.SA, arg.TA_TYPE, arg.RA)
        ret.msg = this.cantp.GetErrorText(ret.err)
      } else {
        ret.msg = this.cantp.GetErrorText(ret.err)
      }
      event.returnValue = ret
    })
    ipcMain.on('can-deleteMap', (event, arg) => {
      var ret = {}
      ret.err = this.cantp.RemoveMapping(this.channel, arg[0])
      if (ret.err === 0) {
        ret.err = this.cantp.RemoveMapping(this.channel, arg[1])
        ret.msg = this.cantp.GetErrorText(ret.err)
      } else {
        ret.msg = this.cantp.GetErrorText(ret.err)
      }
      event.returnValue = ret
    })
    ipcMain.on('canudsExcute', (event, arg) => {
      this.udsTable = arg.udsTable
      this.timeout = arg.timeout
      this.sDelay = arg.sDelay
      this.index = 0
      this.startTime = new Date().getTime()
      this.step(this.udsTable[this.index])
    })
  }
  log (msg) {
    this.win.webContents.send('udsEvent', {
      err: 0,
      msg: '[校验函数]:(' + this.index + ')' + msg + '\r\n'
    })
  }
  delay (timeout) {
    var t = typeof timeout !== 'undefined' ? timeout : this.timeout
    var ret = {}
    this.udsTimer = setTimeout(() => {
      ret.msg = '[' + (new Date().getTime() - this.startTime) / 1000.0 + ']:'
      ret.msg += '(' + this.index + ') Write no response!'
      ret.err = -2
      this.win.webContents.send('udsEvent', ret)
    }, t)
  }

  eventHandle () {
    var err = {}
    var msg = {}
    var ret = {}

    ret.err = 0
    ret.msg = '[' + (new Date().getTime() - this.startTime) / 1000.0 + ']:'

    err.err = this.cantp.TpRead(this.channel, msg)
    err.msg = this.cantp.GetErrorText(err.err)
    if (err.err === 0) {
      switch (msg.MSGTYPE) {
        case PCANTP.PCANTP_MESSAGE_REQUEST_CONFIRMATION:
          ret.msg += '(' + this.index + ') Write from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
                        ' - result:' + ErrorText[msg.RESULT] + '\r\n'
          if (msg.RESULT === 0) {
            this.writeOk = true
            this.suppress = this.checkSuppress(this.udsTable[this.index])
            if (this.suppress) {
              /* suppress, don't wait response */
              this.index += 1
              if (this.udsTable[this.index]) {
                this.sDelayTimer = setTimeout(() => {
                  this.step(this.udsTable[this.index])
                }, this.sDelay)
              } else {
                ret.err = 1
                ret.msg += 'Write OK!\r\n'
              }
            } else {
              this.udsTimer = setTimeout(() => {
                ret.msg = '[' + (new Date().getTime() - this.startTime) / 1000.0 + ']:'
                ret.msg += '(' + this.index + ') Write no response!'
                ret.err = -2
                this.win.webContents.send('udsEvent', ret)
              }, this.timeout)
            }
          } else {
            ret.err = -3
          }
          break

        case PCANTP.PCANTP_MESSAGE_INDICATION_TX:
          ret.msg += '(' + this.index + ') Tx Message pending from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
                        ' LEN:' + msg.LEN + ' - result:' + ErrorText[msg.RESULT] + '\r\n'
          break
        case PCANTP.PCANTP_MESSAGE_INDICATION:
          ret.msg += '(' + this.index + ') Rx Message pending from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
                        ' LEN:' + msg.LEN + ' - result:' + ErrorText[msg.RESULT] + '\r\n'
          break
        case PCANTP.PCANTP_MESSAGE_DIAGNOSTIC:
        case PCANTP.PCANTP_MESSAGE_REMOTE_DIAGNOSTIC:
          ret.msg += '(' + this.index + ') Receive message from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
                        ' LEN:' + msg.LEN + ' - result:' + ErrorText[msg.RESULT] + '- data:' + msg.DATA.toString(16) + '\r\n'
          if (this.writeOk === true) {
            /* clear timeout */
            clearTimeout(this.udsTimer)
            /* handle response data */
            var checkFunc = {}
            if (typeof this.checkFuncJs === 'string') {
              try {
                // eslint-disable-next-line no-eval
                checkFunc = eval('(writeData,readData)=>{' + this.checkFuncJs + '}')
              } catch (error) {
                // eslint-disable-next-line no-eval
                checkFunc = eval('(writeData,readData)=>{return true}')
              }
            } else {
              checkFunc = this.checkFuncJs
            }

            if (checkFunc(this.writeData, msg.DATA)) {
              if (this.udsTimer.hasRef()) {
                /* a new timeout */
                ret.msg += 'Insert a new timeout wait response!\r\n'
              } else {
                /* check supress,todo */
                if (this.suppress) {
                  clearTimeout(this.sDelayTimer)
                  ret.err = -7
                  ret.msg += '[error]: Suppress service got a response!\r\n'
                } else {
                  /* auto34 insert? */
                  if (this.writeData[0] === 0x34) {
                    this.auto34(msg.DATA)
                  }
                  /* auto34 insert? */
                  if (this.writeData[0] === 0x35) {
                    this.auto35(msg.DATA)
                  }
                  /* call next step */
                  this.index += 1
                  if (this.udsTable[this.index]) {
                    this.step(this.udsTable[this.index])
                  } else {
                    ret.err = 1
                    ret.msg += 'Write OK!\r\n'
                  }
                }
              }
            } else {
              ret.err = -5
              ret.msg += '校验返回数据失败!\r\n'
            }
          }
          break
      }
    } else {
      ret.msg = err.msg + '\r\n'
    }
    this.win.webContents.send('udsEvent', ret)
  }
  registerCallback (fn) {
    this.cantp.RegCb(fn)
  }
  checkSuppress (item) {
    if (item.subFunc) {
      return item.subFunc.suppress
    } else {
      return false
    }
  }
  auto35 (readData) {
    if (readData.length > 2 && readData[0] === 0x74) {
      var len = (readData[1] & 0xf0) >> 4
      var i
      if (len + 2 <= readData.length) {
        var blockLen = 0
        for (i = 0; i < len; i++) {
          blockLen += readData[2 + i] * Math.pow(256, (len - 1 - i))
        }
        if (blockLen > 2) {
          blockLen -= 2
          /* response is ok */
          /* insert */
          var usedTable = this.udsTable.slice(0, this.index + 1)
          var unusedTable = this.udsTable.slice(this.index + 1)
          var newTable = [].concat(usedTable)
          for (i = 0; i < parseInt(this.fileSize / blockLen); i++) {
            newTable.push({
              service: { value: 0x36 },
              addr: this.addr35,
              param: [],
              func: this.writeFile
            })
          }
          if (this.fileSize % blockLen) {
            newTable.push({
              service: { value: 0x36 },
              addr: this.addr35,
              param: [],
              func: this.writeFile
            })
          }
          /* check next service */
          if (unusedTable.length > 0 && unusedTable[0].service.value === 0x37) {

          } else {
            newTable.push({
              service: { value: 0x37 },
              addr: this.addr35,
              param: [],
              func: 'return true;'
            })
          }

          this.udsTable = newTable.concat(unusedTable)
          this.blockNum = 1
        }
      }
    }
  }
  auto34 (readData) {
    if (readData.length > 2 && readData[0] === 0x74) {
      var len = (readData[1] & 0xf0) >> 4
      var i
      if (len + 2 <= readData.length) {
        var blockLen = 0
        for (i = 0; i < len; i++) {
          blockLen += readData[2 + i] * Math.pow(256, (len - 1 - i))
        }
        if (blockLen > 2) {
          blockLen -= 2
          /* response is ok */
          /* insert */
          var usedTable = this.udsTable.slice(0, this.index + 1)
          var unusedTable = this.udsTable.slice(this.index + 1)
          var newTable = [].concat(usedTable)
          for (i = 0; i < parseInt(this.fileSize / blockLen); i++) {
            newTable.push({
              service: { value: 0x36 },
              addr: this.addr34,
              len: blockLen,
              param: [],
              func: 'return true;'
            })
          }
          if (this.fileSize % blockLen) {
            newTable.push({
              service: { value: 0x36 },
              addr: this.addr34,
              len: this.fileSize % blockLen,
              param: [],
              func: 'return true;'
            })
          }
          /* check next service */
          if (unusedTable.length > 0 && unusedTable[0].service.value === 0x37) {

          } else {
            newTable.push({
              service: { value: 0x37 },
              addr: this.addr34,
              param: [],
              func: 'return true;'
            })
          }

          this.udsTable = newTable.concat(unusedTable)
          this.blockNum = 1
        }
      }
    }
  }
  step (item) {
    var i, j, val

    var msg = item.addr
    var data = []
    var ret = {}
    this.writeOk = false
    data.push(item.service.value)
    if (item.service.value === 0x34) {
      this.addr34 = item.addr
      this.fileName = item.other.path
      this.fileSize = item.other.size
      this.fStream = fs.createReadStream(this.fileName)
      this.mode = 'read'
    }
    if (item.service.value === 0x35) {
      this.addr35 = item.addr
      this.fileName = item.other.path
      this.fileSize = item.other.size
      this.fStream = fs.createWriteStream(this.fileName)
      this.mode = 'write'
    }

    if (item.service.value === 0x36) {
      data.push(this.blockNum)
      this.blockNum++
      if (this.blockNum === 256) {
        this.blockNum = 0
      }
      if (this.mode === 'read') {
        var content = this.fStream.read(item.len)
        data = data.concat(Array.prototype.slice.call(content, 0))
      }
    }
    if (item.service.value === 0x37) {
      this.fStream.destroy()
    }

    if (item.subFunc) {
      data.push(item.subFunc.suppress ? item.subFunc.value | 0x80 : item.subFunc.value)
    }
    if (item.param.length > 0) {
      for (i in item.param) {
        for (j in item.param[i].value) {
          val = parseInt(item.param[i].value[j], 16)
          data.push(isNaN(val) ? 0 : val)
        }
      }
    }
    if (this.canfd) {
      msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_FD
      msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_BRS
    }

    this.writeData = data
    this.checkFuncJs = item.func
    msg.DATA = data
    msg.LEN = data.length
    ret.err = this.cantp.TpWrite(this.channel, msg)
    ret.msg = this.cantp.GetErrorText(ret.err)
    if (ret.err !== 0) {
      ret.err = -3
      this.win.webContents.send('udsEvent', ret)
    }
  }
}

module.exports = CANUDS;
