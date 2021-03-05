/* eslint-disable no-case-declarations */
const crypto = require('crypto')
const fs = require('fs')
const hexParser = require("./../hse_helper/intel_hex.js");
const aesCmac = require('node-aes-cmac').aesCmac;
import { ipcMain } from 'electron'

ipcMain.on('signFw', (event, arg) => {
    let ret={
        err:true,
        msg:'ok',
        tag:''
    };

    let fileName=arg.file;
    let smrInfo=arg.info;
    let data=hexParser.parse(fs.readFileSync(fileName)).data;
    
    let startPos=smrInfo.startAddr-0x400000;
    let len=smrInfo.length;

    let signData=data.slice(startPos,startPos+len);

    try{
        switch(smrInfo.authWay){
            case 'cmac-aes':
                let key=Buffer.from(smrInfo.signKey,'hex')
                ret.tag = aesCmac(key, signData, { returnAsBuffer: true })
                break;
            default:
                ret.err=false;
                ret.msg="Unknow sign algorithm"
                break;

        }
    }catch(e){
        ret.err=false;
        ret.msg=e
    }
   
    
    event.returnValue = ret
})