/* eslint-disable no-unused-vars */
const os = require('os')
const dgram = require('dgram')
const { ipcMain } = require('electron')

const PORT = 13400
const VER =0x02

class IPUDS {
    constructor(win){
        this.win=win
        this.header=Buffer.from([VER,VER^0xff,0,0,0,0,0,0])

        this.udpFd=dgram.createSocket('udp4')
        this.udpFd.bind(PORT)
        this.udpFd.on('message',(msg, rinfo)=>{
            console.log(msg)
            var ret=this.parseData(msg)
            ret.addr=rinfo
            this.emit('ipAnnounce',ret)
        })
        this.typeList=[0,1,2,3,4,5,6,7,8,0x4001,0x4002,0x4003,0x4004,0x8001,0x8002,0x8003]
        this.clientTypeList=[0,4,6,7,0x4002,0x4004,0x8002,0x8003]
        ipcMain.on('ip-interface', (event, arg) => {
            event.returnValue =os.networkInterfaces()
        })
        ipcMain.on('ip-bind',(event,arg)=>{
            this.udpFd.close()
            this.udpFd=dgram.createSocket('udp4')
            this.udpFd.bind(PORT,arg)
            this.udpFd.on('message',(msg, rinfo)=>{
                console.log(msg)
                var ret=this.parseData(msg)
                ret.addr=rinfo
                this.emit('ipAnnounce',ret)
            })
        })
        ipcMain.on('ip-refresh',(event,arg)=>{
            var msg
            if(arg.vin){
                msg=this.writeReqVIN((x)=>{return x},arg.vin)
            }else if(arg.eid){
                msg=this.writeReqEID((x)=>{return x},arg.eid)
            }else{
                msg=this.writeReqMsg((x)=>{return x})
            }
            this.udpFd.send(msg,PORT+1,arg.multiaddress)
        })

    }
    emit(channel,msg){
        this.win.webContents.send(channel, msg)
    }
    udpBind(address){
        this.udpFd.bind(PORT,address)
        this.udpFd.on('listening',()=>{
            this.emit('ipEvnet',{
                err:0,
                msg:'bind ok!\r\n'
            })
        })
        this.udpFd.on('message',(event,arg)=>{
            var msg=this.writeReqMsg((x)=>{return x})
            this.udpFd.send(msg,13400,)
        })
    }
    parseData(msg){
        /*header handler*/
        var data=Buffer.from(msg)
        var ret={}
        ret.err=0
        ret.msg='empty'
        if((data.readUInt8(0)!==VER)||data.readUInt8(1)!==(VER^0xff)){
            ret.err=-1
            ret.msg="Incorrect pattern format"
            return ret
        }
        var type=data.readUInt16BE(2)
        if(this.typeList.indexOf(type)===-1){
            ret.err=-1
            ret.msg="Unknown payload type"
            return ret
        }
        var len=data.readUInt32BE(4)
        if(len!==(data.length-8)){
            ret.err=-1
            ret.msg="Invalid payload length"
            return ret
        }
        if(this.clientTypeList.indexOf(type)===-1){
            ret.err=-2
            ret.msg="Discard message"
            return ret
        }
        /*real parse*/
        var payload=Buffer.from(data.slice(8))
        ret.type=type
        ret.data={}
        if(type===4&&payload.length>=32){
            ret.data.vin=payload.slice(0,17).toString('ascii')
            ret.data.logicalAddr=payload.readUInt16BE(17)
            ret.data.eid=payload.slice(19,25).toString('hex')
            ret.data.gid=payload.slice(25,31).toString('hex')
            ret.data.fAction=payload.readUInt8(31)
            if(payload.length==33){
                ret.data.syncStatus=payload.readUInt8(32)
            }
        }else if(type===6){
            ret.data.testerAddr=payload.readUInt16BE(0)
            ret.data.entityAddr=payload.readUInt16BE(2)
            ret.data.code=payload.readUInt8(4)
            if(payload.length==13){
                ret.data.option=payload.readUInt32BE(9)
            }
        }else if(type===0x8002){
            ret.data.sa=payload.readUInt16BE(0)
            ret.data.ta=payload.readUInt16BE(2)
            ret.data.code=payload.readUInt8(4)
            ret.data.payload=payload.slice(5).toString('hex')
        }else if(type===0x8003){
            // ret.err=-1
            // ret.msg='Negative diagnostic message'
            ret.data.sa=payload.readUInt16BE(0)
            ret.data.ta=payload.readUInt16BE(2)
            ret.data.code=payload.readUInt8(4)
            ret.data.payload=payload.slice(5).toString('hex')
        }
        else if(type===7){
            //todo active request
        }else if(type===0x4004){
            ret.data.powerMode=payload.readUInt8(0)
        }else if(type===0x4002){
            ret.data.nt=payload.readUInt8(0)
            ret.data.mcts=payload.readUInt8(1)
            ret.data.ncts=payload.readUInt8(2)
            if(payload.length===7){
                ret.data.mds=payload.readUInt32BE(3)
            }
        }
        return ret
    }
    
    changeLen(len){
        this.header.writeUInt32BE(len,4)
    }
    changeType(type){
        this.header.writeUInt16BE(type,2)
    }
    writeNACK(writer,code){
        this.changeLen(1)
        this.changeType(0)
        return writer(Buffer.concat([this.header,Buffer.from([code])],this.header.length+1))
    }
    writeReqMsg(writer){
        this.changeLen(0)
        this.changeType(1)
        return writer(this.header)
    }
    writeReqEID(writer,eid){
        this.changeLen(6)
        this.changeType(2)
        return writer(Buffer.concat([this.header,Buffer.from(eid,'binary')],this.header.length+6))
    }
    writeReqVIN(writer,vin){
        this.changeLen(17)
        this.changeType(3)
        return writer(Buffer.concat([this.header,Buffer.from(vin,'ascii')],this.header.length+17))
    }
    writeRouteActive(writer,sa,activeType,option){
        var len=0
        if(typeof option!==undefined){
            len=option.length
        }
        this.changeLen(7+len)
        this.changeType(5)
        var b=Buffer.alloc(7+len,0)
        if(len>0){
            Buffer.from(option).copy(b,7)
        }
        
        b.writeUInt16BE(sa,0)
        b.writeUInt8(activeType,2)
        return writer(Buffer.concat([this.header,b],this.header.length+b.length))
    }
    writeReqAlive(writer){
        this.changeLen(0)
        this.changeType(7)
        return writer(this.header)
    }
    writeReqDiaPowerMode(writer){
        this.changeLen(0)
        this.changeType(0x4003)
        return writer(this.header)
    }
    writeReqDoipStatus(writer){
        this.changeLen(0)
        this.changeType(0x4001)
        return writer(this.header)
    }
    writeDiaMsg(writer,sa,ta,ud){
        var b=Buffer.alloc(4+ud.length,0)
        Buffer.from(ud).copy(b,4)
        b.writeUInt16BE(sa,0)
        b.writeUInt16BE(ta,2)
        this.changeLen(b.length)
        this.changeType(0x8001)
        return writer(Buffer.concat([this.header,b],this.header.length+b.length))
    }



}


module.exports = IPUDS