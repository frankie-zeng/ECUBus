/* eslint-disable no-undef */
/* eslint-disable no-empty */
const path = require('path')
const sprintf = require('sprintf-js').sprintf
const { decodeTable, payload2data } = require('./decode.js')
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
    this.receive = false
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
      this.subTable = []
      this.timeout = arg.timeout
      this.sDelay = arg.sDelay
      this.addr = arg.addr
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
  openFile(filename, flag = 'r') {
    this.fd = fs.openSync(filename, flag)
  }
  readFile(size) {
    var buf = Buffer.alloc(size)
    var len = fs.readSync(this.fd, buf, 0, size)
    return [...buf.slice(0, len)]
  }
  writeFile(data) {
    fs.writeSync(this.fd, Buffer.from(data))
  }
  closeFile() {
    fs.closeSync(this.fd)
  }
  changeNextFrame(name, value = []) {
    if ((this.subTable.length == 0) && (this.udsTable.length > 0)) {
      this.subTable = decodeTable(this.udsTable.shift())
    }
    if (this.subTable.length > 0) {
      for (var i in this.subTable[0].payload) {
        if (this.subTable[0].payload[i].name == name) {
          this.subTable[0].payload[i][name] = value
          break
        }
      }
    }
  }
  // eslint-disable-next-line no-unused-vars
  insertItem(service, payload, func = (writeData, readData) => { return true }) {
    this.subTable.unshift({
      func: func,
      payload: payload,
      service: service
    })
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

            if (this.suppress) {
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
          if (this.receive) {
            this.receive = false;
            clearTimeout(this.udsTimer)
            // this.emit('udsData', sprintf("[data]:msg:%s.\r\n", msg.DATA.join(',')))
            try {
              if ((msg.DATA[0] == 0x7F) && (msg.DATA[2] == 0X78)) {
                this.receive = true;
                this.delay()
                break
              }
              if (this.checkFunc(this.writeData, msg.DATA)) {
                if (this.udsTimer.hasRef()) {
                  this.receive = true;
                  this.emit('udsData', sprintf("[data]:User insert a new delay\r\n"))
                } else {
                  this.step()
                }
              } else {
                this.emit('udsError', sprintf("[error]:User defined function return false,used time:%d\r\n", new Date().getTime() - this.startTime))
              }
            } catch (error) {
              this.emit('udsError', sprintf("[error]:User defined function syntax error,%s,used time:%d\r\n", error.message, new Date().getTime() - this.startTime))
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
  progress(show, percent) {
    this.emit('progress', {
      show: show,
      percent: percent
    })
  }
  step() {
    if ((this.udsTable.length == 0) && (this.subTable.length == 0)) {
      this.emit('udsEnd', sprintf("[done]:Excute successful,used time:%dms\r\n", new Date().getTime() - this.startTime))
      return 0
    }
    if (this.subTable.length == 0) {
      this.subTable = decodeTable(this.udsTable.shift())
    }
    var item = this.subTable.shift()
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
    this.writeData = item.payload
    var suppress = false
    var data = [item.service]


    data = data.concat(payload2data(item.payload))
    /* check surpress*/
    for (var i in this.writeData) {
      if (this.writeData[i].type == 'subfunction') {
        if (this.writeData[i].suppress) {
          suppress = true
        }
        break
      }
    }
    var msg = this.addr
    this.suppress = suppress
    if (this.canfd) {
      msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_FD
      msg.IDTYPE |= PCANTP.PCANTP_ID_CAN_BRS
    }

    msg.DATA = data
    msg.LEN = data.length
    var err = this.cantp.TpWrite(this.channel, msg)
    if (err !== 0) {
      this.emit('udsError', sprintf('[error]:%s,used time:%d\r\n'), this.cantp.GetErrorText(err), new Date().getTime() - this.startTime)
    } else {
      this.receive = !suppress
    }
  }
}

module.exports = CANUDS;
