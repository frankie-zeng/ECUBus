const crypto = require('crypto')
const fs = require('fs')
const ch = require('child_process')
const path = require('path');
const HSEnode = require('./../../build/Release/HSE.node')
const she = require('./lib/k3she')
const { aesCmac } = require('./lib/aes-cmac');
const log = require('electron-log');

class HSE extends HSEnode.HSE {
    constructor(fPath, ePath = __static) {
        super()
        this.sheKey = {
            'MASTER_ECU_KEY': [0, 1],
            'BOOT_MAC_KEY': [0, 2],
            'BOOT_MAC': [0, 3],
            'RAM_KEY': [0, 14],
            'KEY1': [0, 4],
            'KEY2': [0, 5],
            'KEY3': [0, 6],
            'KEY4': [0, 7],
            'KEY5': [0, 8],
            'KEY6': [0, 9],
            'KEY7': [0, 10],
            'KEY8': [0, 11],
            'KEY9': [0, 12],
            'KEY10': [0, 13],
            'KEY11': [1, 4],
            'KEY12': [1, 5],
            'KEY13': [1, 6],
            'KEY14': [1, 7],
            'KEY15': [1, 8],
            'KEY16': [1, 9],
            'KEY17': [1, 10],
            'KEY18': [1, 11],
            'KEY19': [1, 12],
            'KEY20': [1, 13],
            'KEY21': [2, 4],
            'KEY22': [2, 5],
            'KEY23': [2, 6],
            'KEY24': [2, 7],
            'KEY25': [2, 8],
            'KEY26': [2, 9],
            'KEY27': [2, 10],
            'KEY28': [2, 11],
            'KEY29': [2, 12],
            'KEY30': [2, 13],
            'KEY31': [3, 4],
            'KEY32': [3, 5],
            'KEY33': [3, 6],
            'KEY34': [3, 7],
            'KEY35': [3, 8],
            'KEY36': [3, 9],
            'KEY37': [3, 10],
            'KEY38': [3, 11],
            'KEY39': [3, 12],
            'KEY40': [3, 13],
            'KEY41': [4, 4],
            'KEY42': [4, 5],
            'KEY43': [4, 6],
            'KEY44': [4, 7],
            'KEY45': [4, 8],
            'KEY46': [4, 9],
            'KEY47': [4, 10],
            'KEY48': [4, 11],
            'KEY49': [4, 12],
            'KEY50': [4, 13],
        }
        this.file = path.join(fPath, 'tmp.pem')
        this.ssl = path.join(ePath, 'openssl.exe')
        String.prototype.replaceAll = function (match, replace) {
            return this.replace(new RegExp(match, 'g'), () => replace);
        }
    }
    getErrorStrLegacy(code) {
        let str
        switch (code) {
            case 0x55A5AA33:
                str = 'HSE service successfully executed with no error.'
                break;
            case 0x55a5aa35:
                str = 'HSE signals that a verification request fails.'
                break;
            case 0x55a5aa55:
                str = 'The address parameters are invalid.'
                break;
            case 0x55A5AA56:
                str = 'The HSE request parameters are invalid.'
                break;
            case 0xAA55A569:
                str = 'The operation or feature not supported.'
                break;
            case 0xAA55A536:
                str = 'The operation is not allowed because of some restrictions (in attributes, life-cycle dependent operations, key-management, etc.)'
                break;
            case 0xAA55A563:
                str = 'There is no enough space to perform service (e.g. format key store)'
                break;
            case 0xaa55a599:
                str = 'The service request failed because read access was denied.'
                break;
            case 0xaa55a5b1:
                str = 'The service request failed because write access was denied.'
                break;
            case 0xAA55A5C1:
                str = 'The service request that uses streaming mode failed (e.g. UPDATES and FINISH steps do not use the same HSE interface ID and channel ID as START step)'
                break;
            case 0xA5AA5571:
                str = 'This error code is returned if a key is locked due to failed boot measurement or an active debugger.'
                break;
            case 0xA5AA5527:
                str = ```
                The key usage flags (provided using the key handle) don't allow to perform the requested crypto operation
                (the key flags don't match the crypto operation; e.g. the key is configured to be used for decryption, and
                the host requested an encryption). In SHE, the key ID provided is either invalid or non-usable due to some
                flag restrictions.
                ```
                break;
            case 0xA5AA5517:
                str = 'Specified key slot is empty.'
                break;
            case 0xA5AA5537:
                str = 'Key slot to be loaded is protected with WRITE PROTECTION restriction flag.'
                break;
            case 0xA5AA5573:
                str = 'Used only in the context of SHE specification: specified key slot cannot be updated due to errors in verification of the parameters.'
                break;
            case 0x55A5AA36:
                str = 'Detect physical errors, flipped bits etc., during memory read or write operations.'
                break;
            case 0x55A5C461:
                str = 'The service can not be canceled.'
                break;
            case 0x55A5C596:
                str = 'The service has been canceled.'
                break;
            case 0x55A5C565:
                str = ```
                This error code is returned if an error not covered by the error codes above is detected inside HSE. For
                HSE-B, it can be returned if flash programming and erase operaton was in progress at the time of giving
                the command.
                ```
                break;
            case 0x55a5c633:
                str = 'The monotonic counter overflows.'
                break;
            case 0x55A5C635:
                str = 'Generic extension failure at runtime.'
                break;
            case 0xA5AAD623:
                str = 'HSE did not perform SHE based secure Boot.'
                break;
            case 0xA5AAD63A:
                str = 'Received SHE_BOOT_OK or SHE_BOOT_FAILURE more then one time.'
                break;
            case 0xDD33DD33:
                str = 'Invalid (malformed) IP packet.'
                break;
            case 0xDD3333DD:
                str = 'Valid packet but replay detected.'
                break;
            case 0xDD3333A5:
                str = 'Valid packet but frame late in sequence.'
                break;
            case 0xDD33A53D:
                str = 'Sequence number overflow.'
                break;
            case 0xDD33A55A:
                str = 'Decap CE DROP (ECN issue) error.'
                break;
            case 0xDD33A3D5:
                str = 'Packet decrypted but TTL exceeded.'
                break;
            case 0xDD33D5D5:
                str = 'Valid Dummy Payload (type 59)'
                break;
            case 0xDD335DD5:
                str = 'Operation successful, but IPsec additions cause overflow of IP header length field.'
                break;
            case 0xDD335D3A:
                str = 'IPsec padding check error found.'
                break;
            default:
                str = ''
        }
        return str
    }
    getErrorStr(code) {
        let str
        switch (code) {
            case 0x55A5AA33:
                str = 'HSE service successfully executed with no error.'
                break;
            case 0x55A5A164:
                str = 'HSE signals that a verification request fails.'
                break;
            case 0x55A5A26A:
                str = 'The address parameters are invalid.'
                break;
            case 0x55A5A399:
                str = 'The HSE request parameters are invalid.'
                break;
            case 0xAA55A11E:
                str = 'The operation or feature not supported.'
                break;
            case 0xAA55A21C:
                str = 'The operation is not allowed because of some restrictions (in attributes, life-cycle dependent operations, key-management, etc.)'
                break;
            case 0xAA55A371:
                str = 'There is no enough space to perform service (e.g. format key store)'
                break;
            case 0xAA55A427:
                str = 'The service request failed because read access was denied.'
                break;
            case 0xAA55A517:
                str = 'The service request failed because write access was denied.'
                break;
            case 0xAA55A6B1:
                str = 'The service request that uses streaming mode failed (e.g. UPDATES and FINISH steps do not use the same HSE interface ID and channel ID as START step)'
                break;
            case 0xA5AA51B2:
                str = 'This error code is returned if a key is locked due to failed boot measurement or an active debugger.'
                break;
            case 0xA5AA52B4:
                str = ```
                The key usage flags (provided using the key handle) don't allow to perform the requested crypto operation
                (the key flags don't match the crypto operation; e.g. the key is configured to be used for decryption, and
                the host requested an encryption). In SHE, the key ID provided is either invalid or non-usable due to some
                flag restrictions.
                ```
                break;
            case 0xA5AA5317:
                str = 'Specified key slot is empty.'
                break;
            case 0xA5AA5436:
                str = 'Key slot to be loaded is protected with WRITE PROTECTION restriction flag.'
                break;
            case 0xA5AA5563:
                str = 'Used only in the context of SHE specification: specified key slot cannot be updated due to errors in verification of the parameters.'
                break;
            case 0x33D6D136:
                str = 'Detect physical errors, flipped bits etc., during memory read or write operations.'
                break;
            case 0x33D6D261:
                str = 'The service can not be canceled.'
                break;
            case 0x33D6D396:
                str = 'The service has been canceled.'
                break;
            case 0x33D6D4F1:
                str = ```
                This error code is returned if an error not covered by the error codes above is detected inside HSE. For
                HSE-B, it can be returned if flash programming and erase operaton was in progress at the time of giving
                the command.
                ```
                break;
            case 0x33D6D533:
                str = 'The monotonic counter overflows.'
                break;
            case 0x33D6D623:
                str = 'HSE did not perform SHE based secure Boot.'
                break;
            case 0x33D7D83A:
                str = 'Received SHE_BOOT_OK or SHE_BOOT_FAILURE more then one time.'
                break;
            default:
                str = ''
        }
        return str
    }
    keyCovert(key) {
        let a = key.split(':')
        let b = []
        for (let i in a) {
            b.push(parseInt(a[i], 16))
        }
        return b
    }
    getSSLVersion() {
        return this.getVersion();
    }
    GET_CATALOG_ID(keyHandle) {
        return ((keyHandle >> 16) & 0xff)
    }
    GET_GROUP_IDX(keyHandle) {
        return ((keyHandle >> 8) & 0xff)
    }
    GET_SLOT_IDX(keyHandle) {
        return (keyHandle & 0xff)
    }
    GET_KEY_HANDLE(catalog, group, slot) {
        return ((catalog & 0xff) << 16) | ((group & 0xff) << 8) | (slot & 0xff)
    }
    gmac(key, value, iv = Buffer.from('f7a093cd1ce82f11bc318b3e', 'hex')) {
        var keyLengthToCipher = { 16: 'aes-128-gcm', 24: 'aes-192-gcm', 32: 'aes-256-gcm' };
        if (!keyLengthToCipher[key.length]) {
            throw new Error('Keys must be 128, 192, or 256 bits in length.');
        }
        var cipher = crypto.createCipheriv(keyLengthToCipher[key.length], key, iv);
        cipher.setAAD(value)
        cipher.final()
        return cipher.getAuthTag()
    }
    cmac(key, value) {
        return aesCmac(key, value, { returnAsBuffer: true })
    }
    importSheKey(offset, keyAuthId, keyAuthValue, keyId, keyValue, flag, cid, uid) {
        var ret = she(keyAuthId, keyAuthValue, keyId, keyValue, flag, cid, uid)
        return this.importSubSheKey(offset, keyId[0], ret.M1, ret.M2, ret.M3)
    }
    genSheMartix(keyAuthId, keyAuthValue, keyId, keyValue, flag, cid, uid) {
        return she(keyAuthId, keyAuthValue, keyId, keyValue, flag, cid, uid)
    }
    setDebugAuthMode(offset, challenge) {
        var buf = Buffer.alloc(this.ATTR_DEBUG_MODE_LEN)
        if (challenge == true)
            buf.writeUInt8(this.HSE_DEBUG_AUTH_MODE_CR)
        else
            buf.writeUInt8(this.HSE_DEBUG_AUTH_MODE_PW)

        return this.setAttr(offset, this.HSE_DEBUG_AUTH_MODE_ATTR_ID, buf)
    }
    importSymKey(offset, keyHandle, keyInfo, key) {
        return this.importKey(offset, keyHandle, keyInfo, Buffer.alloc(0), Buffer.alloc(0), Buffer.from(key, 'hex'))
    }
    curveCovert(cover) {
        if (cover == 'secp384r1') {
            return this.HSE_EC_SEC_SECP384R1;
        } else if (cover == 'ed25519') {
            return this.HSE_EC_25519_ED25519;
        } else if (cover == 'secp521r1') {
            return this.HSE_EC_SEC_SECP521R1;
        } else if (cover == 'brainpoolP320r1') {
            return this.HSE_EC_BRAINPOOL_BRAINPOOLP320R1;
        } else if (cover == 'brainpoolP384r1') {
            return this.HSE_EC_BRAINPOOL_BRAINPOOLP384R1;
        } else if (cover == 'brainpoolP512r1') {
            return this.HSE_EC_BRAINPOOL_BRAINPOOLP512R1;
        } else if (cover == 'prime256v1') {
            return this.HSE_EC_SEC_SECP256R1;
        } else if (cover == 'secp256r1') {
            return this.HSE_EC_SEC_SECP256R1
        } else {
            return 0;
        }
    }
    importPubKey(offset, keyHandle, keyInfo, key) {
        /* store key in file with pem format */
        fs.writeFileSync(this.file, key)
        let ret = ch.spawnSync(this.ssl, ['pkey','-pubin','-in', this.file, '-noout', '-text'])
        if (ret.status == 0) {
            let key = ret.stdout.toString('ascii')
            let keyBitLen
            key = key.replaceAll('\r', '').replaceAll('\n', '').replaceAll(' ', '')
            if (key.indexOf('ED25519') != -1) {
                keyBitLen = 256
            } else {
                keyBitLen = parseInt(/(\d.*)bit/gm.exec(key)[1], 10)
            }
            keyInfo.keyBitLen = keyBitLen
            if (key.indexOf('RSA') != -1) {
                
                let keySize = keyBitLen / 8;
                let n = this.keyCovert(/Modulus:([[0-9a-f:]*)/.exec(key)[1])
                n = n.slice(n.length - keySize, n.length);
                let e = parseInt(/Exponent:(\d*)/.exec(key)[1], 10)
                let be = Buffer.alloc(4)
                be.writeUInt32BE(e)
                be = be.slice(1, 4);
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(n), be, Buffer.alloc(0))
            } else if (key.indexOf('ED25519') != -1) {
                let keySize = parseInt((keyBitLen + 7) / 8);
                let pub = this.keyCovert(/pub:([[0-9a-f:]*)/.exec(key)[1])
                pub = pub.slice(pub.length - keySize, pub.length);
                keyInfo.curveId=this.curveCovert('ed25519');
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(pub),Buffer.alloc(0), Buffer.alloc(0))
            } else {
                let keySize = parseInt((keyBitLen + 7) / 8);
                let curve = /ASN1OID:(.*?)NIST/.exec(key)[1]
                keyInfo.curveId=this.curveCovert(curve);
                let pub = this.keyCovert(/pub:([[0-9a-f:]*)/.exec(key)[1])
                pub = pub.slice(pub.length - keySize * 2, pub.length);
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(pub),Buffer.alloc(0), Buffer.alloc(0))
            }
        } else {
            log.error(ret)
            return {
                err: ret.status,
                data: Buffer.alloc(0),
                msg: ret.stderr.toString('ascii')
            }
        }
    }
    importPrivKey(offset, keyHandle, keyInfo, key) {
        /* store key in file with pem format */
        fs.writeFileSync(this.file, key)
        let ret = ch.spawnSync(this.ssl, ['pkey', '-in', this.file, '-noout', '-text'])
        if (ret.status == 0) {
            let key = ret.stdout.toString('ascii')
            let keyBitLen
            key = key.replaceAll('\r', '').replaceAll('\n', '').replaceAll(' ', '')
            if (key.indexOf('ED25519') != -1) {
                keyBitLen = 256
            } else {
                keyBitLen = parseInt(/(\d.*)bit/gm.exec(key)[1], 10)
            }
            keyInfo.keyBitLen = keyBitLen
            if (key.indexOf('RSA') != -1) {
                let keySize = keyBitLen / 8;
                let n = this.keyCovert(/modulus:([[0-9a-f:]*)/.exec(key)[1])
                n = n.slice(n.length - keySize, n.length);
                let d = this.keyCovert(/privateExponent:([[0-9a-f:]*)/.exec(key)[1])
                d = d.slice(d.length - keySize, d.length);
                let e = parseInt(/publicExponent:(\d*)/.exec(key)[1], 10)
                let be = Buffer.alloc(4)
                be.writeUInt32BE(e)
                be = be.slice(1, 4);
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(n), be, Buffer.from(d))
            } else if (key.indexOf('ED25519') != -1) {
                let keySize = parseInt((keyBitLen + 7) / 8);
                let priv = this.keyCovert(/priv:([[0-9a-f:]*)/.exec(key)[1])
                let pub = this.keyCovert(/pub:([[0-9a-f:]*)/.exec(key)[1])
                priv = priv.slice(priv.length - keySize, priv.length);
                pub = pub.slice(pub.length - keySize, pub.length);
                keyInfo.curveId=this.curveCovert('ed25519');
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(pub),Buffer.alloc(0), Buffer.from(priv))
            } else {
                let keySize = parseInt((keyBitLen + 7) / 8);
                let curve = /ASN1OID:(.*?)NIST/.exec(key)[1]
                keyInfo.curveId=this.curveCovert(curve);
                let priv = this.keyCovert(/priv:([[0-9a-f:]*)/.exec(key)[1])
                let pub = this.keyCovert(/pub:([[0-9a-f:]*)/.exec(key)[1])
                priv = priv.slice(priv.length - keySize, priv.length);
                pub = pub.slice(pub.length - keySize * 2, pub.length);
                return this.importKey(offset, keyHandle, keyInfo, Buffer.from(pub),Buffer.alloc(0), Buffer.from(priv))
            }
        } else {
            return {
                err: ret.status,
                data: Buffer.alloc(0),
                msg: ret.stderr.toString('ascii')
            }
        }
    }
    setDebugKey(offset, key) {
        var buf = Buffer.alloc(this.ATTR_ADKP_LEN)
        key.copy(buf, 0, 0, 16)
        return this.setAttr(offset, this.HSE_APP_DEBUG_KEY_ATTR_ID, buf)
    }
    setLifeCycle(offset, lc) {
        var buf = Buffer.alloc(offset, this.ATTR_LC_LEN)
        buf.writeUInt8(lc, 0)
        return this.setAttr(offset, this.HSE_SECURE_LIFECYCLE_ATTR_ID, buf)
    }
    setBootAuth(offset, enable) {
        var buf = Buffer.alloc(this.ATTR_BOOT_AUTH_LEN)
        if (enable) {
            buf.writeUInt8(this.HSE_IVT_AUTH, 0)
        } else {
            buf.writeUInt8(this.HSE_IVT_NO_AUTH, 0)
        }
        return this.setAttr(offset, this.HSE_ENABLE_BOOT_AUTH_ATTR_ID, buf)
    }
    setCustSecurityPolicy(offset, enADKm, startAsUser) {
        var buf = Buffer.alloc(this.ATTR_EXTEND_CUST_SECURITY_POLICY_LEN, 0)
        if (enADKm)
            buf.writeUInt8(1, 0)
        else
            buf.writeUInt8(0, 0)
        if (startAsUser)
            buf.writeUInt8(1, 1)
        else
            buf.writeUInt8(0, 1)
        return this.setAttr(offset, this.HSE_EXTEND_CUST_SECURITY_POLICY_ATTR_ID, buf)
    }
    setOemSecurityPolicy(offset, startAsUser) {
        var buf = Buffer.alloc(this.ATTR_EXTEND_OEM_SECURITY_POLICY_LEN)
        if (startAsUser)
            buf.writeUInt8(1, 0)
        else
            buf.writeUInt8(0, 0)
        return this.setAttr(offset, this.HSE_EXTEND_OEM_SECURITY_POLICY_ATTR_ID, buf)
    }
    setMu(offset, muConfig = []) {
        var buf = Buffer.alloc(this.ATTR_MU_CONFIG_LEN)
        var len = this.ATTR_MU_CONFIG_LEN / this.ATTR_MU_INST_CONFIG_LEN
        if (muConfig.length != len) {
            return {
                err: -1,
                data: Buffer.alloc(0),
                msg: 'MU config length does not equal HSE MU lenght'
            }
        } else {
            for (let i = 0; i < len; i++) {
                buf.writeUInt8(muConfig[0].muConfig, i * this.ATTR_MU_INST_CONFIG_LEN)
                buf.writeUInt8(muConfig[0].xrdcDomainId, i * this.ATTR_MU_INST_CONFIG_LEN + 1)
                buf.writeInt16LE(muConfig[0].sharedMemChunkSize, i * this.ATTR_MU_INST_CONFIG_LEN + 2)
            }
        }
        return this.setAttr(offset, this.HSE_MU_CONFIG_ATTR_ID, buf)
    }
    // eslint-disable-next-line no-unused-vars
    setTamper(offset, enable) {
        return {
            err: -1,
            data: Buffer.alloc(0),
            msg: 'Unspport service'
        }
    }
    setFirc(offset, divider) {
        var buf = Buffer.alloc(this.ATTR_FIRC_LEN)
        buf.writeUInt8(divider, 0)
        return this.setAttr(offset, this.HSE_FIRC_DIVIDER_CONFIG_ATTR_ID, buf)
    }
    /**
     * 
     * @param {buffer} key RAW ADKP
     * @returns {buffer}
     */
    calADKPExtension(key) {
        const hash = crypto.createHash('sha256')
        hash.update(key)
        return hash.digest()
    }
    calADKPDeviceDep(key, uid) {
        const hash1 = crypto.createHash('sha256')
        const hash2 = crypto.createHash('sha256')
        hash1.update(key)
        var realkey = hash1.digest()
        hash2.update(uid)
        var content = hash2.digest()
        const cipher = crypto.createCipheriv('aes-256-ecb', realkey, null);
        cipher.setAutoPadding(false)
        var ret = cipher.update(content)
        cipher.final()
        return ret
    }

}
module.exports = HSE