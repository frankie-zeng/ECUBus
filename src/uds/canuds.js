/* eslint-disable no-undef */
/* eslint-disable no-empty */
const path = require('path')
const sprintf = require('sprintf-js').sprintf

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


const PCANTP = require('./../../build/Release/PCANTP.node')
// const  PCANTP = require(path.join(__static, 'PCANTP.node'))
const isDevelopment = process.env.NODE_ENV !== 'production'
const dllPath = isDevelopment ? path.join(__static, 'peak') : path.join(process.resourcesPath, 'peak')
class CANUDS {
  constructor(win) {
    this.win = win
    this.cantp = new PCANTP.CANTP(dllPath)
    this.canfd = false
    this.channel = PCANTP.PCANTP_USBBUS1
    this.udsTimer = setTimeout(() => { }, 0)
    this.writing = false
    this.writeFile = (writeData, readData) => {
      this.fStream.write(new Uint8Array(readData))
      return true
    }
    clearTimeout(this.udsTimer)
    ipcMain.on('canConnect', (event, arg) => {
      var err = this.cantp.Initialize(arg[0], arg[1])
      this.canfd = false
      this.channel = arg[0]
      event.returnValue = {
        err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('canConnectFd', (event, arg) => {
      var err = this.cantp.InitializeFd(arg[0], arg[1])
      this.canfd = true
      this.channel = arg[0]
      event.returnValue = {
        err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('canDisconnect', (event, arg) => {
      var err = this.cantp.Uninitialize(arg)
      event.returnValue = {
        err: err,
        msg: this.cantp.GetErrorText(err)
      }
    })
    ipcMain.on('canAddMap', (event, arg) => {
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
    ipcMain.on('canDeleteMap', (event, arg) => {
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
      this.step()
    })
  }
  log(msg) {
    this.emit('udsData', msg)
  }
  delay(timeout) {
    var t = typeof timeout !== 'undefined' ? timeout : this.timeout
    this.udsTimer = setTimeout(() => {
      this.emit('udsError', sprintf('[error]:No Response,used time:%dms\r\n', new Date().getTime() - this.startTime))
    }, t)
  }
  emit(channel, msg) {
    this.win.webContents.send(channel, msg)
  }
  Unload() {
    this.cantp.Unload()
  }
  eventHandle() {
    var err
    var msg = {}
    err = this.cantp.TpRead(this.channel, msg)
    if (err === 0) {
      switch (msg.MSGTYPE) {
        case PCANTP.PCANTP_MESSAGE_REQUEST_CONFIRMATION:
          if (msg.RESULT === 0) {
            this.writing = false
            if (this.udsTable[this.index - 1].suppress) {
              /* suppress, don't wait response */
              setTimeout(() => {
                this.step()
              }, this.sDelay)
            } else {
              this.udsTimer = setTimeout(() => {
                this.emit('udsError', sprintf('[error]:No response,used time:%d\r\n', new Date().getTime() - this.startTime))
              }, this.timeout)
            }
          } else {
            this.emit('udsError', sprintf('[error]:Write from 0x%x to 0x%x with RA 0x%x,result:%s,used time:%d\r\n', msg.SA.toString(16), msg.TA.toString(16), msg.RA.toString(16),
              ErrorText[msg.RESULT], new Date().getTime() - this.startTime))
          }
          break

        // case PCANTP.PCANTP_MESSAGE_INDICATION_TX:
        //   // ret.msg += '(' + this.index + ') Tx Message pending from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
        //   //   ' LEN:' + msg.LEN + ' - result:' + ErrorText[msg.RESULT] + '\r\n'
        //   break
        // case PCANTP.PCANTP_MESSAGE_INDICATION:
        //   // ret.msg += '(' + this.index + ') Rx Message pending from 0x' + msg.SA.toString(16) + ' to 0x' + msg.TA.toString(16) + ' with RA 0x' + msg.RA.toString(16) +
        //   //   ' LEN:' + msg.LEN + ' - result:' + ErrorText[msg.RESULT] + '\r\n'
        //   break
        case PCANTP.PCANTP_MESSAGE_DIAGNOSTIC:
        case PCANTP.PCANTP_MESSAGE_REMOTE_DIAGNOSTIC:
          if (this.writing === false) {
            clearTimeout(this.udsTimer)
            this.emit('udsData', sprintf("[data]:msg:%s.\r\n", msg.DATA.join(',')))
            if (this.checkFunc(this.writeData, msg.DATA)) {
              if (this.udsTimer.hasRef()) {
                this.emit('udsData', sprintf("[data]:User insert a new delay\r\n"))
              } else {
                /* auto34 insert? */
                if (this.writeData[0] === 0x35) {
                  this.auto35(msg.DATA)
                  setTimeout(() => {
                    this.step()
                  }, 5);
                }
                else if (this.writeData[0] === 0x34) {
                  this.auto34(msg.DATA)
                  setTimeout(() => {
                    this.step()
                  }, 5);
                } else {
                  this.step()
                }
              }
            } else {
              this.emit('udsError', sprintf("[error]:User defined function return false,used time:%d\r\n", new Date().getTime() - this.startTime))
            }
          }
          break
      }
    } else {
      this.emit('udsError', sprintf("[error]:%s,used time:%d\r\n", this.cantp.GetErrorText(ret.err), new Date().getTime() - this.startTime))
    }
  }
  registerCallback(fn) {
    this.cantp.RegCb(fn)
  }
  auto35(readData) {
    if (readData.length > 2 && readData[0] === 0x75) {
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
          var usedTable = this.udsTable.slice(0, this.index)
          var unusedTable = this.udsTable.slice(this.index)
          var newTable = [].concat(usedTable)
          for (i = 0; i < parseInt(this.fileSize / blockLen); i++) {
            newTable.push({
              type: 'uds',
              service: { value: 0x36 },
              addr: this.addr35,
              payload: {},
              func: this.writeFile
            })
          }
          if (this.fileSize % blockLen) {
            newTable.push({
              type: 'uds',
              service: { value: 0x36 },
              addr: this.addr35,
              payload: {},
              func: this.writeFile
            })
          }
          /* check next service */
          if (unusedTable.length > 0 && unusedTable[0].service.value === 0x37) {

          } else {
            newTable.push({
              type: 'uds',
              service: { value: 0x37 },
              addr: this.addr35,
              payload: {},
              func: 'return true;'
            })
          }

          this.udsTable = newTable.concat(unusedTable)
          this.blockNum = 1
        }
      }
    }
  }
  auto34(readData) {
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
          var usedTable = this.udsTable.slice(0, this.index)
          var unusedTable = this.udsTable.slice(this.index)
          var newTable = [].concat(usedTable)
          for (i = 0; i < parseInt(this.fileSize / blockLen); i++) {
            newTable.push({
              type: 'uds',
              service: { value: 0x36 },
              addr: this.addr34,
              len: blockLen,
              payload: {},
              func: 'return true;'
            })
          }
          if (this.fileSize % blockLen) {
            newTable.push({
              type: 'uds',
              service: { value: 0x36 },
              addr: this.addr34,
              len: this.fileSize % blockLen,
              payload: {},
              func: 'return true;'
            })
          }
          /* check next service */
          if (unusedTable.length > 0 && unusedTable[0].service.value === 0x37) {

          } else {
            newTable.push({
              type: 'uds',
              service: { value: 0x37 },
              addr: this.addr34,
              payload: {},
              func: 'return true;'
            })
          }

          this.udsTable = newTable.concat(unusedTable)
          this.blockNum = 1
        }
      }
    }
  }
  step() {
    if (this.index === this.udsTable.length) {
      this.emit('udsEnd', sprintf("[done]:Excute successful,used time:%dms\r\n", new Date().getTime() - this.startTime))
      return
    }
    var item = this.udsTable[this.index]
    this.index++;
    var rawdata = []
    if (item.type === 'uds') {
      rawdata.push(item.service.value)
      if (item.service.value === 0x34) {
        this.addr34 = item.addr
        this.fileName = item.other.filePath
        this.fileSize = item.other.fileSize
        this.fStream = fs.createReadStream(this.fileName)
        this.mode = 'read'
      }
      if (item.service.value === 0x35) {
        this.addr35 = item.addr
        this.fileName = item.other.filePath
        this.fileSize = item.other.fileSize
        this.fStream = fs.createWriteStream(this.fileName)
        this.mode = 'write'
      }

      if (item.service.value === 0x36) {
        rawdata.push(this.blockNum)
        this.blockNum++
        if (this.blockNum === 256) {
          this.blockNum = 0
        }
        if (this.mode === 'read') {
          var content = this.fStream.read(item.len)
          rawdata = rawdata.concat(Array.prototype.slice.call(content, 0))
        }
      }
      if (item.service.value === 0x37) {
        this.fStream.destroy()
      }
      if (typeof item.func === 'string') {
        try {
          // eslint-disable-next-line no-eval
          this.checkFunc = eval('(writeData,readData)=>{' + item.func + '}')
        } catch (error) {
          // eslint-disable-next-line no-eval
          this.checkFunc = eval('(writeData,readData)=>{return true}')
        }
      } else {
        this.checkFunc = item.func
      }
      for (var i in item.payload) {
        rawdata = rawdata.concat(item.payload[i])
      }
      if (item.suppress) {
        rawdata[1] |= 0x80
      }
      var msg = item.addr
      if (this.canfd) {
        msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_FD
        msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_BRS
      }
      this.writeData = rawdata
      msg.DATA = rawdata
      msg.LEN = rawdata.length
      var err = this.cantp.TpWrite(this.channel, msg)
      if (err !== 0) {
        this.emit('udsError', sprintf('[error]:%s,used time:%d\r\n'), this.cantp.GetErrorText(err), new Date().getTime() - this.startTime)
      } else {
        this.writing = true
      }
    }
  }
}

module.exports = CANUDS;
