/* eslint-disable no-undef */
/* eslint-disable no-empty */
const path = require('path')
const sprintf = require('sprintf-js').sprintf
const { ipcMain } = require('electron')
const UDS = require('./uds.js')


const PLIN = require('./../../build/Release/PLIN.node')

const isDevelopment = process.env.NODE_ENV !== 'production'
const dllPath = isDevelopment ? path.join(__static, 'peak') : path.join(process.resourcesPath, 'peak')
class LINUDS extends UDS {
  constructor(win) {
    super(win)
    this.linapi = new PLIN.LINAPI(dllPath)
    this.device = 0
    this.linVer=this.linapi.GetVersion();
    this.debug("LIN dll verion"+this.linVer);
    this.udsTimer = setTimeout(() => { }, 0)
    this.recvTimer = setInterval(() => {

    }, 100000);
    this.receive = false
    clearTimeout(this.udsTimer)
    clearInterval(this.recvTimer)
    this.pidMap = {}
    for (let i = 0; i < 64; i++) {
      this.pidMap[i] = this.linapi.GetPID(i);
    }

    ipcMain.on('linConnect', (event, arg) => {
      let device = arg[0]
      let speed = arg[1]
      let res
      res = this.linapi.RegisterClient();
      if (res == 0) {
        res = this.linapi.ConnectClient(device);
        if (res == 0) {
          res = this.linapi.InitializeHardware(device, PLIN.modMaster, speed)
          if (res == 0) {
            res = this.linapi.SetClientFilter(device, BigInt(0))
          }
        }

      }
      this.device = device
      event.returnValue = {
        err: res,
        msg: this.linapi.GetErrorText(res)
      }
    })
    // eslint-disable-next-line no-unused-vars
    ipcMain.on('linDisconnect', (event, arg) => {
      clearInterval(this.recvTimer)
      this.linapi.SuspendSchedule(this.device)
      let res
      res = this.linapi.RemoveClient();
      event.returnValue = {
        err: res,
        msg: this.linapi.GetErrorText(res)
      }
    })
    ipcMain.on('linDllVersion', (event) => {
      event.returnValue = this.linVer
    })
    ipcMain.on('linudsExcute', (event, arg) => {
      this.UDSstart(arg.udsTable)
      this.timeout = arg.timeout
      this.sDelay = arg.sDelay
      this.stMin = arg.addr.stMin
      this.p2 = arg.addr.p2
      this.p2Opt = arg.addr.p2Opt
      this.sendNad = arg.addr.sendNad
      this.recvNad = arg.addr.recvNad
      let res
      res = this.linapi.SetSchedule(this.device, 0, [
        {
          Type: PLIN.sltMasterRequest,
          Delay: this.stMin,
          FrameId: [0x3c],
          CountResolve: 0,
        },
      ])
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
      }
      res = this.linapi.SetSchedule(this.device, 1, [
        {
          Type: PLIN.sltSlaveResponse,
          Delay: this.p2 / 2,
          FrameId: [0x3d],
          CountResolve: 0,
        }
      ])
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
      }
      res = this.linapi.SetFrameEntry(this.device, {
        FrameId: 0x3c,
        Length: 8,
        Direction: PLIN.dirPublisher,
        ChecksumType: PLIN.cstClassic,
        Flags: PLIN.FRAME_FLAG_RESPONSE_ENABLE,
        InitialData: [0, 0, 0, 0, 0, 0, 0, 0]
      })
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
      }
      res = this.linapi.SetFrameEntry(this.device, {
        FrameId: 0x3d,
        Length: 8,
        Direction: PLIN.dirSubscriber,
        ChecksumType: PLIN.cstClassic,
        Flags: PLIN.FRAME_FLAG_RESPONSE_ENABLE,
        InitialData: [0, 0, 0, 0, 0, 0, 0, 0]
      })
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
      }
      // res=this.linapi.SetClientFilter(this.device,BigInt(0xffffffffffffffffn))
      res = this.linapi.RegisterFrameId(this.device, 0x3c, 0x3d);
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
      }
      this.linapi.SuspendSchedule(this.device)
      if (res == 0) {
        this.step()
      }

    })
  }
  write(data) {
    this.debug(data)
    let msg = {
      FrameId: 0x3c,
      Length: data.length,
      Direction: PLIN.dirPublisher,
      ChecksumType: PLIN.cstClassic,
      Data: data,
      Checksum: 0,
    }
    this.linapi.CalculateChecksum(msg)
    this.writeTime = new Date().getTime()
    return this.linapi.Write(this.device, msg)
  }
  delay(timeout) {
    //restart schedule
    this.linapi.StartSchedule(this.device, 1)
    var t = typeof timeout !== 'undefined' ? timeout : this.timeout
    this.udsTimer = setTimeout(() => {
      this.errorEnd('No response')
    }, t)
  }
  recvHandle() {
    //checktimeout 
    if (this.udsTimer.hasRef()) {
      //clear timeout 
      clearTimeout(this.udsTimer)
      //stop scheduel
      this.linapi.SuspendSchedule(this.device)
      try {
        if ((this.recvData[0] == 0x7F) && (this.recvData[2] == 0x78)) {
          this.debug("receive a wati frame")
          this.delay()
          this.recvData = []
          this.ff = false
          return
        }
        if (this.checkFunc(this.writeData, this.recvData)) {
          if (this.udsTimer.hasRef()) {
            this.info('User insert a new delay')
            this.delay()
            return
          } else {
            this.step()
          }
        } else {
          this.errorEnd('User defined function return false')
        }
      } catch (error) {
        this.errorEnd(sprintf("User defined function syntax error,%s", error.message))
      }
    }



  }
  errorEnd(msg) {
    this.linapi.SuspendSchedule(this.device)
    clearTimeout(this.udsTimer)
    clearInterval(this.recvTimer)
    this.emit('udsError', {
      msg: msg,
      index: this.tableIndex
    })
  }
  step() {
    try {
      var item = this.getNextService()
    } catch (e) {
      this.emit('udsError', {
        msg: "User defined function syntax error," + e.message,
        index: this.tableIndex
      })
      return -1
    }
    if (item === null) {
      clearInterval(this.recvTimer)
      this.emit('udsEnd', "Excute successful")
      return 0
    }
    this.checkFunc = item.checkFunc
    this.writeData = item.payload
    this.suppress = item.suppress
    this.data = item.data
    let res

    let rawdata = [this.sendNad, 0, 0, 0, 0, 0, 0, 0]
    if (this.data.length <= 6) {
      //single frame
      rawdata[1] = this.data.length
      for (let j = 0; j < this.data.length; j++) {
        rawdata[2 + j] = this.data[j]
      }
      res = this.write(rawdata)
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
        return -1
      }
      this.left = 0
    } else {
      //first frame
      rawdata[1] = 0x10 | (this.data.length / 256)
      rawdata[2] = this.data.length & 0xff
      for (let j = 0; j < 5; j++) {
        rawdata[3 + j] = this.data[j]
      }
      res = this.write(rawdata)
      if (res != 0) {
        this.errorEnd(this.linapi.GetErrorText(res))
        return -1
      }
      this.data = this.data.slice(5)
      this.left = Math.ceil(this.data.length / 6)
      this.sn = 1
    }
    if (!this.recvTimer.hasRef()) {
      this.debug("start receve poll feature")
      this.recvTimer = setInterval(() => {
        let val = {}
        while (this.linapi.Read(val) == 0) {
          this.debug(val)
          if ((val.ErrorFlags == 0) && (val.Direction == PLIN.dirPublisher)) {
            this.debug(val)
            if (this.left > 0) {
              let usedTime = new Date().getTime() - this.writeTime
              this.debug(`usedtime:${usedTime}`)
              setTimeout(() => {
                /* ready raw data */
                rawdata[1] = 0x20 | this.sn
                //handle sn overflow, left cf count
                this.sn++
                if (this.sn == 16) {
                  this.sn = 0
                }
                this.left--
                //next data
                let index
                if (this.data.length < 6) {
                  for (index = 0; index < this.data.length; index++) {
                    rawdata[index + 2] = this.data[index]
                  }
                  for (; index < 6; index++) {
                    rawdata[index + 2] = 0
                  }
                } else {
                  for (index = 0; index < 6; index++) {
                    rawdata[index + 2] = this.data[index]
                  }
                  //shift used data 
                  this.data = this.data.slice(6)
                }
                res = this.write(rawdata)
                if (res != 0) {
                  this.errorEnd(this.linapi.GetErrorText(res))
                }
              }, this.stMin - usedTime > 0 ? this.stMin - usedTime : 0);
            } else {
              // this.linapi.SuspendSchedule(this.device)
              this.debug("end one tp write")
              this.recvData = []
              this.ff = false
              //check suppress
              if (this.suppress) {
                setTimeout(() => {
                  this.step()
                }, this.sDelay)
              } else {
                //wait p2 and start receive schedule
                setTimeout(() => {
                  res = this.linapi.StartSchedule(this.device, 1)
                  if (res != 0) {
                    this.errorEnd(this.linapi.GetErrorText(res))
                  } else {
                    //start to wati timeout
                    this.udsTimer = setTimeout(() => {
                      this.errorEnd('No response')
                    }, this.timeout);
                  }
                }, this.p2);
              }
            }
          } else if ((val.ErrorFlags == 0) && (val.Direction == PLIN.dirSubscriber)) {
            if ((val.FrameId == 0x7d) && (val.Data[0] == this.recvNad)) {
              this.debug(val)
              //check frame type
              try {
                if ((val.Data[1] & 0xf0) == 0) {
                  //simple frame
                  this.recvData = val.Data.slice(2, 2 + val.Data[1] & 0xf)
                  this.debug("receive a frame")
                  this.debug(this.recvData)
                  this.recvHandle()
                } else if ((val.Data[1] & 0xf0) == 0x10) {
                  //ff
                  this.ff = true
                  this.sn = 1
                  this.needLen = (val.Data[1] & 0xf) * 256 + val.Data[2]
                  this.debug(`long frame len:${this.needLen}`)
                  this.recvData = this.recvData.concat(val.Data.slice(3, val.Length))
                  this.needLen -= (val.Length - 3)
                } else if (((val.Data[1] & 0xf0) == 0x20) && (this.ff)) {
                  //cf
                  this.udsTimer.refresh()
                  if (this.sn == (val.Data[1] & 0xf)) {
                    this.sn++
                    if (this.sn == 16) {
                      this.sn = 0
                    }
                    this.recvData = this.recvData.concat(val.Data.slice(2, (val.Length - 2) > this.needLen ? (2 + this.needLen) : val.Length))
                    this.needLen -= ((val.Length - 2) > this.needLen ? this.needLen : (val.Length - 2))
                    if (this.needLen == 0) {
                      this.debug("receive a frame")
                      this.debug(this.recvData)
                      this.recvHandle()



                    }
                  } else {
                    this.debug(`error CF sn, hope:${this.sn},got:${val.Data[1] & 0xf}`)
                  }
                } else {
                  this.debug(`error PCI format ${val.Data}`)
                }
              } catch (e) {
                this.errorEnd(sprintf("User defined function syntax error,%s", error.message))
              }
            }
          } else if (val.ErrorFlags != 0) {
            this.errorEnd(sprintf("Lin read error,0x%X", val.ErrorFlags))
          }
        }

      }, 3)
    }








  }
}

module.exports = LINUDS;
