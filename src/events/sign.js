/* eslint-disable no-case-declarations */
const crypto = require('crypto')
const fs = require('fs')
const EC = require('elliptic').ec;
const EdDSA = require('elliptic').eddsa;
const hexParser = require("./../hse_helper/intel_hex.js");
const aesCmac = require('node-aes-cmac').aesCmac;
const tdesCmac = require('node-aes-cmac').tdesCmac;
import { ipcMain } from 'electron'

ipcMain.on('signFw', (event, arg) => {
    let ret = {
        err: true,
        msg: 'ok',
        tag: ''
    };

    let fileName = arg.file;
    let smrInfo = arg.info;
    let data = hexParser.parse(fs.readFileSync(fileName)).data;

    let startPos = smrInfo.startAddr - 0x400000;
    let len = smrInfo.length;
    let signData = data.slice(startPos, startPos + len);
    let key;
    let hmac;
    let sign;
    let ec;

    try {
        if (smrInfo.mac) {
            key = Buffer.from(smrInfo.signKey, 'hex')
        } else {
            key = crypto.createPrivateKey(smrInfo.signKey)
        }
        switch (smrInfo.authWay) {
            case 'cmac-aes':
                ret.tag = aesCmac(key, signData, { returnAsBuffer: true })
                break;
            case 'cmac-tdes':
                ret.tag = tdesCmac(key, signData, { returnAsBuffer: true })
                break;
            case 'hmac-sha2-224':
                hmac = crypto.createHmac('sha224', key);
                hmac.update(signData)
                ret.tag = hmac.digest()
                break;
            case 'hmac-sha2-256':
                hmac = crypto.createHmac('sha256', key);
                hmac.update(signData)
                ret.tag = hmac.digest()
                break;
            case 'hmac-sha2-384':

                hmac = crypto.createHmac('sha384', key);
                hmac.update(signData)
                ret.tag = hmac.digest()
                break;
            case 'hmac-sha2-512':

                hmac = crypto.createHmac('sha512', key);
                hmac.update(signData)
                ret.tag = hmac.digest()
                break;
            case 'gmac':

                let gmac;
                if (key.length == 16) {
                    gmac = crypto.createCipheriv('aes-128-gcm', key, Buffer.from(smrInfo.iv, 'hex'))
                } else if (key.length == 24) {
                    gmac = crypto.createCipheriv('aes-192-gcm', key, Buffer.from(smrInfo.iv, 'hex'))
                } else if (key.length == 32) {
                    gmac = crypto.createCipheriv('aes-256-gcm', key, Buffer.from(smrInfo.iv, 'hex'))
                } else {
                    ret.err = false;
                    ret.msg = "Unknow sign algorithm"
                }
                if (gmac != undefined) {
                    gmac.setAAD(signData)
                    gmac.final();
                    ret.tag = gmac.getAuthTag();
                }
                break;
            case 'rsa-pkcs1-sha2-256':
                if (key.asymmetricKeyType != 'rsa') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha256');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'rsa-pkcs1-sha2-224':
                if (key.asymmetricKeyType != 'rsa') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha224');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pkcs1-sha2-384':
                if (key.asymmetricKeyType != 'rsa') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha384');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pkcs1-sha2-512':
                if (key.asymmetricKeyType != 'rsa') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha512');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pss-sha2-256':
                if (key.asymmetricKeyType != 'rsa-pss') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha256');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pss-sha2-224':
                if (key.asymmetricKeyType != 'rsa-pss') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha224');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pss-sha2-384':
                if (key.asymmetricKeyType != 'rsa-pss') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha384');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }

                break;
            case 'rsa-pss-sha2-512':
                if (key.asymmetricKeyType != 'rsa-pss') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha512');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'ecdsa-sha224':
                if (key.asymmetricKeyType != 'ec') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha224');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'ecdsa-sha256':
                if (key.asymmetricKeyType != 'ec') {
                    ret.err = false;
                    ret.msg = "Private key type unmatch"
                } else {
                    sign = crypto.createSign('sha256');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'ecdsa-sha384':
                if(key.asymmetricKeyType!='ec'){
                    ret.err=false;
                    ret.msg="Private key type unmatch"
                }else{
                    sign = crypto.createSign('sha384');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'ecdsa-sha512':
                if(key.asymmetricKeyType!='ec'){
                    ret.err=false;
                    ret.msg="Private key type unmatch"
                }else{
                    sign = crypto.createSign('sha512');
                    sign.update(signData)
                    ret.tag = sign.sign(key)
                }
                break;
            case 'eddsa':
                if(key.asymmetricKeyType!='ed25519'){
                    ret.err=false;
                    ret.msg="Private key type unmatch"
                }else{
                    // sign = crypto.createSign('sha512');
                    // sign.update(signData)
                    ret.tag = crypto.sign(null,signData,key)
                }
                break;
            default:
                ret.err = false;
                ret.msg = "Unknow sign algorithm"
                break;

        }
    } catch (e) {
        ret.err = false;
        ret.msg = e
    }


    event.returnValue = ret
})