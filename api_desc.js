/* eslint-disable no-unused-vars */

/**
 * @typedef {Object} HSERet
 * @property {number} err - Error value
 * @property {buffer} data - Array Buffer
 * @property {string} msg - Message
 */
/**
 * @typedef {Object} SHEFlags
 * @property {boolean} WRITE_PROT - Write protect
 * @property {boolean} BOOT_PROT - Boot protect, the key cannot be used if SMR #0 auth failed
 * @property {boolean} DEBUG_PROT - DEBUG protect, disable when debug plugin
 * @property {boolean} KEY_USAGE - The key can be used for CMAC generation and verification; works in combination with VERIFY_ONLY
 * @property {boolean} VERIFY_ONLY - The key can be used for CMAC verification only
 * @property {boolean} WILDCARD - The UID must be provided when the key is provisioned
 */

/**
 * @description Control UI progress bar display, user need close dispaly manual.
 * @param  {boolean} show - Enable progress bar show in US.
 * @param  {number} percent - Progress percent
 * @param  {string} name='main' - Progress bar name
 * @example 
 * this.progress(true,30,'flash driver')
 * this.progress(false,100,'flash driver)
 */
function progress(show, percent, name = 'main') {

}
/**
 * @description Store value in key-value in tmp database.
 * @param {string} key  
 * @param {any} value
 * @param {boolean} keep - If false, the value will be clear when each start, else, the value will keep always.
 */
function set(key, value, keep = false) {

}
/**
 * @description Get value from key-value database
 * @param {string} key   
 * @returns {any}
 */
function get(key) {

}
/**
 * @description Print verbose level information to log window
 * @param {any} msg 
 */
function verbose(msg) {

}

/**
 * @description Print error level information to log window
 * @param {any} msg 
 */
function error(msg) {

}

/**
 * @description Print info level information to log window
 * @param {any} msg 
 */
function info(msg) {

}

/**
 * @description Print debug level information to log window
 * @param {any} msg 
 */
function debug(msg) {

}
/**
 * @description Flexbile log print.
 * @param {any} msg 
 * @param {string} type - 'verbose','error','info','debug' 
 */
function log(msg, type = 'debug') {

}
/**
 * @description Open a file and store fd in internal.
 * @param {string} filename - File name
 * @param {string} flag - File operation flag
 * @example
 * this.openFile('fw.bin','w')
 * this.writeFile('hello world')
 * this.closeFile()
 */
function openFile(filename, flag = 'r') {

}
/**
 * @description Read data from last open file, return length maybe less than hope size.
 * @param {number} size - hope read size
 * @returns {array}
 */
function readFile(size) {

}
/**
 * @description Write data into last open file.
 * @param {buffer} data 
 */
function writeFile(data) {

}
/**
 * @description Change schedule table next frame payload content.
 * @param {string} name - Next frame payload name
 * @param {array} value 
 * @example 
 * this.changeNextFrame("memorySize",[0,0,0,0x55])
 */
function changeNextFrame(name, value = []) {

}
/**
 * 
 * @param {number} service - UDS service id
 * @param {array} payload - UDS payload value except serivce id 
 * @param {func} func - UDS response handle function
 * @param {func} preFunc - UDS pre-handle function
 * @example
 * this.insertItem(0x36,[0x55,0x33],(writeData,readData)=>{
 *      this.log(writeData)
 * })
 */
function insertItem(service, payload, func = (writeData, readData) => { return true }, preFunc = (writeData) => { }) {

}

/**
 * @description HSE Service API
 */
class hse {
    /**
     * @returns {string} Get internal openssl verison.
     * @example this.getSSLVersion()
     */
    getSSLVersion() {

    }
    /**
     * @description Get catalog value from key handle.
     * @param {number} keyHandle 
     * @returns {number} the key catalog value.
     */
    GET_CATALOG_ID(keyHandle) {

    }
    /**
     * @description Get group value form key handle. 
     * @param {number} keyHandle
     * @returns {number} the key group value.
     */
    GET_GROUP_IDX(keyHandle) {
    }
    /**
     * @description Get slot value form key handle. 
     * @param {number} keyHandle
     * @returns {number} the key slot value.
     */
    GET_SLOT_IDX(keyHandle) {
    }
    /**
     * @description Get keyHandle from catalog,gorup,slot.
     * @param {number} catalog - HSE catalog value 
     * @param {number} group - HSE group value
     * @param {number} slot - HSE slot value
     * @returns {number} 
     * @example
     * this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0)
     */
    GET_KEY_HANDLE(catalog, group, slot) {

    }
    /**
     * @description Calculate gmac value.
     * @param {buffer} key Gmac key
     * @param {buffer} value 
     * @param {buffer} iv Default IV is f7a093cd1ce82f11bc318b3e for calculate IVT/XRDC/APPBL
     * @returns {buffer} Return gmac result.
     */
    gmac(key, value, iv = Buffer.from('f7a093cd1ce82f11bc318b3e', 'hex')) {
    }
    /**
     * @description Calculate cmac value.
     * @param {buffer} key 
     * @param {buffer} value 
     * @returns {buffer} Return cmac result.
     */
    cmac(key, value) {
    }
    /**
     * @description Generate import she key HSE service.
     * @param {number} offset - Target ECU ram address
     * @param {array} keyAuthId - SHE AUTH key id array   
     * @param {buffer} keyAuthValue - SHE AUTH key value 
     * @param {array} keyId - SHE target key id array 
     * @param {buffer} keyValue - SHE target key value 
     * @param {SHEFlags} flag - SHE key flag
     * @param {number} cid - Key update counter number 
     * @param {buffer} uid - UID lenght must equal 8 bytes 
     * @returns {HSERet} 
     * @example
     * this.importSheKey(0x20000000,
     *          this.sheKey.MASTER_ECU_KEY,Buffer.alloc(16,0),
     *          this.sheKey.MASTER_ECU_KEY,Buffer.from('000102030405060708090a0b0c0d0e0f','hex'),
     *          {
     *              VERIFY_ONLY:true,
     *              KEY_USAGE:true,
     *          },
     *          1,
     *          Buffer.alloc(8,0)
     * )
     */
    importSheKey(offset, keyAuthId, keyAuthValue, keyId, keyValue, flag, cid, uid) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {boolean} challenge - True: enable challenge response mode, Flase: password mode.
     * @returns {HSERet}
     */
    setDebugAuthMode(offset, challenge) {

    }

    /**
     * 
     * @param {number} offset 
     * @param {buffer} key - ADKP key, 16 bytes.
     * @returns {HSERet}
     */
    setDebugKey(offset, key) {

    }

    /**
     * 
     * @param {number} offset 
     * @param {number} lc
     * @returns {HSERet}
     */
    setLifeCycle(offset, lc) {

    }
    /**
     * @description Enable GMAC check
     * @param {number} offset 
     * @param {boolean} enable 
     * @returns {HSERet}
     */
    setBootAuth(offset, enable) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {boolean} enADKm - defines or returns ADKP_MASTER 
     * @param {boolean} startAsUser - efines or returns the value of CUST_START_AS_USER
     * @returns {HSERet}
     */
    setCustSecurityPolicy(offset, enADKm, startAsUser) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {boolean} startAsUser - defines or returns the value of OEM_START_AS_USER
     * @returns {HSERet}
     */
    setOemSecurityPolicy(offset, startAsUser) {

    }

    /**
     * @typedef {Object} MUConfig
     * @property {number} muConfig - MU instance number
     * @property {number} xrdcDomainId - XRDC domain id, HSE_H only
     * @property {number} sharedMemChunkSize - HSE_H only
     */


    /**
     * 
     * @param {number} offset 
     * @param {MUConfig[]} muConfig
     * @returns {HSERet} 
     */
    setMu(offset, muConfig = []) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {boolean} enable 
     * @returns {HSERet}
     */
    setTamper(offset, enable) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {number} divider 
     * @returns {HSERet}
     */
    setFirc(offset, divider) {

    }

    /**
     * @typedef {Object} KeyInfo
     * @property {number} keyFlags 
     * @property {number} keyCounter 
     * @property {number} smrFlags 
     * @property {number} keyType
     * @property {number=} keyBitLen
     * 
     */

    /**
     * @returns {HSERet}
     * @param {number} offset 
     * @param {number} keyHandle 
     * @param {KeyInfo} keyInfo 
     * @param {Buffer} key 
     * @example 
     * var keyInfo={
     *       keyFlags:this.HSE_KF_USAGE_ENCRYPT,
     *      keyCounter:0,
     *      smrFlags:this.HSE_KF_SMR_0,
     *       keyType:this.HSE_KEY_TYPE_AES
     *   }
     *   var symkey = Buffer.from('000102030405060708090a0b0c0d0e0f','hex')
     *   this.importSymKey(0x20000000,this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0),keyInfo,symkey)
     */
    importSymKey(offset, keyHandle, keyInfo, key) {

    }

    /**
     * @returns {HSERet}
     * @param {number} offset 
     * @param {number} keyHandle 
     * @param {KeyInfo} keyInfo 
     * @param {string} key
     * @example
     * var keyInfo={
     *       keyFlags:this.HSE_KF_USAGE_VERIFY,
     *       keyCounter:0,
     *       smrFlags:this.HSE_KF_SMR_0,
     *       keyType:this.HSE_KEY_TYPE_RSA_PUB
     *   }
     *   var rsaPubkey = 
    `
    -----BEGIN PUBLIC KEY-----
    MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAx0KfhKB3VgKSUdyrdVE6
    J7iSW0EIDAsv81bpKOjtA/Tj9GvSab0eP8+JwFsHTB+ES/Nc0FzAcITsqP4W/XXo
    jgcT6M8ROZe0IuynZjI6zkOQFzolPkFJanSCjPgigPNtJDVJoKMmStEM3sj5xB+Q
    VMuo/3agd1SBatS9XW6M545znl+YxW42qmjNZXK2/g9xvBF79yEJncGdUa2TnA3Y
    tjAx0OPHAsDsFvgpkXe1YAXZ8Og/PHKqeheSkJP5cEKqfF4JIfqJdvQ9jR08ypas
    x0lhmbgbVO9LvJ36RflGXHYYCfYx4j0d/O6voKmkxFS2wFgqcDN8En8YgWZTxoQb
    6QIDAQAB
    -----END PUBLIC KEY-----
    `
     *   this.importPubKey(0x20000000,this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0),keyInfo,symkey) 
     */
    importPubKey(offset, keyHandle, keyInfo, key) {

    }

    /**
     * @returns {HSERet}
     * @param {number} offset 
     * @param {number} keyHandle 
     * @param {KeyInfo} keyInfo 
     * @param {Buffer} key 
     * @example
     * var keyInfo={
     *       keyFlags:this.HSE_KF_USAGE_VERIFY,
     *       keyCounter:0,
     *       smrFlags:this.HSE_KF_SMR_0,
     *       keyType:this.HSE_KEY_TYPE_RSA_PUB
     *   }
     *   var rsaPrivkey = 
    `-----BEGIN RSA PRIVATE KEY-----
    MIIEpQIBAAKCAQEAx0KfhKB3VgKSUdyrdVE6J7iSW0EIDAsv81bpKOjtA/Tj9GvS
    ab0eP8+JwFsHTB+ES/Nc0FzAcITsqP4W/XXojgcT6M8ROZe0IuynZjI6zkOQFzol
    PkFJanSCjPgigPNtJDVJoKMmStEM3sj5xB+QVMuo/3agd1SBatS9XW6M545znl+Y
    xW42qmjNZXK2/g9xvBF79yEJncGdUa2TnA3YtjAx0OPHAsDsFvgpkXe1YAXZ8Og/
    PHKqeheSkJP5cEKqfF4JIfqJdvQ9jR08ypasx0lhmbgbVO9LvJ36RflGXHYYCfYx
    4j0d/O6voKmkxFS2wFgqcDN8En8YgWZTxoQb6QIDAQABAoIBAQC0fqgKtmSC979d
    AfwaQCLiujFxEd5dMoET9wUrdprD+7/FtymZeKL/AFrycEKS9PWc6uyHHY2AHxhV
    GBzxKS4oAUdKms8qkT2uaRpV22gjUVYjhBsaYn9DPBjJ4zc2Acs6gj6YkVDtcqIq
    6Hg+7KyL8xO5nNZMYbL9d4m1KZ/fWzL5rHjPr7HcL8HbP5BtEby3zKz/2cLyzpPJ
    s+QFjTl981l0AS4g7ezhbNWPuBG0bDl6AnSFYVyQd6DNO44Xp62pSBvwmEIfus6K
    yh3UfOnLQy/h6m3Vax/JKzr4jzFfCxsBCGHqn2rGIuvqPylAfv0+YxBMBmR6qYNm
    57Uihp8BAoGBAPQwo9xp45HLrvpDto2bzx+he4OL7z9NNXvGziQDbgyw3nRsvXs8
    GJS2GCUl7hQme20If7JWaYm6BQpg+Xz00w3q5ib077UUTlLqe7Vi9StRu/MdQ0G9
    JP2VbGXQotBYimful2UQrtfOsmXYgCFkvtS8KSjzPvsUXjpx3m5uSKiJAoGBANDl
    syKLrGB3//Jq4xuEWi2frdpRCpi7zho4Dm2lJoAQJe8FHvOaEW/1AQT3IOdT6kM0
    872disBy2MfJ6vK6nvEtA55U6/EZ3YXFrs/9wZrz6WZFUWZLpMzfjIjSAisttYrb
    S+HYaS/Ff+IkviJ25HeWR2Ss0Hw+nnM5zubxikBhAoGBALEVwQ9sPxuRMcDOcEn7
    r1ZJ/k+koIkwjdPIld/qGNndPkYCQdtn9o+KCrKN8jzst1+X5TtIjVrS6yrerq9e
    wNv7+JJR2LBiHHbnE3W0YQXcTUf+dbLr3zKDNUwU5K3hN+YtUd96HMHmZNuVCT6u
    V/Hgzpyi4o6k87zArUU6dzi5AoGAAzNBp0nUfjXYafJRQF6qswA7PZj4PJbD9Yre
    Tbe1+0hEDIFblAfMNJbgSR8wFdsnP5hUE5eSRJh90bG6M7XAMZmq8IYEh1EKH76D
    ixQsHx+Vd8egE50dDTZvhLkS525v1Yy+gNs74Ut5RhGiCA0iOuLGfThYiU68Rq+g
    B9GzeIECgYEA0rS5cVc694gHeKuuUufrBMJJ6HMvWtS2/RNLJ0EhOcVfsPo5KP7a
    ncFcn1DP0B+bxSRr7Viz5IKI6j2ZeJb3zMVOn7AS5h7Mcf2u4pnSin7C56s9ksDJ
    y3Uv31Ek6iHz/mUmz1uC28ich/lcaNwcYYyODiWE/upq6KEFfwL0QCU=
    -----END RSA PRIVATE KEY-----
    `
     *   this.importPrivKey(0x20000000,this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0),keyInfo,rsaPrivkey) 
     */
    importPrivKey(offset, keyHandle, keyInfo, key) {

    }


    /**
     * @typedef {Object} Catalog
     * @property {number} groupOwner 
     * @property {number} keyType 
     * @property {number} maxKeyBitLen 
     * @property {number} muMask
     * @property {number} numOfKeySlots
     */


    /**
     * @param {number} offset
     * @param {Catalog[]} nvm 
     * @param {Catalog[]} ram
     * @returns {HSERet} 
     * @example
     * var nvmCatalog=[
     *  {groupOwner:this.HSE_KEY_OWNER_OEM, keyType:this.HSE_KEY_TYPE_RSA_PUB,maxKeyBitLen:4096,muMask:this.HSE_ALL_MU_MASK,numOfKeySlots:1}
     * ]
     * var ramCatalog=[
     *  {groupOwner:this.HSE_KEY_OWNER_ANY, keyType:this.HSE_KEY_TYPE_AES,maxKeyBitLen:256,muMask:this.HSE_ALL_MU_MASK,numOfKeySlots:2}
     * ]
     * this.formatCatalog(offset,nvmCatalog,ramCatalog)
     */
    formatCatalog(offset, nvm, ram) {

    }


    /**
     * @typedef {Object} AuthScheme
     * @property {string} type - xcbc_mac,cmac-aes,cmac-tdes,gmac,hmac,rsa-pss,rsa-pkcs,ecdsa,eddsa
     * @property {buffer=} iv - gmac need
     * @property {number=} hashAlgo - hmac,rsa-pss,rsa-pkcs,ecdsa need
     * @property {number=} saltLength - rsa-pss need
     * @property {buffer=} context - eddsa need
     * @property {boolean=} bHashEddsa - eddsa need
     */


    /**
     * @typedef {Object} SMREntry
     * @property {number} pSmrSrc 
     * @property {number} smrSize 
     * @property {number} configFlags 
     * @property {number} verifMethod
     * @property {number} checkPeriod
     * @property {number} keyHandle
     * @property {number} pInstAuthTag
     * @property {AuthScheme} authScheme
     */

    /**
     * @typedef {Object} SMRGenWithData
     * @property {number} accessMode 
     * @property {number=} streamId 
     * @property {number} entryIndex 
     * @property {buffer} smrData
     */

    /**
     * @typedef {Object} SMRGenWithoutData
     * @property {number} accessMode 
     * @property {number=} streamId 
     * @property {number} entryIndex 
     * @property {number} pSmrData
     * @property {number} smrDataLength
     */

    /**
     * @typedef {Object} CREntry
     * @property {number} coreId 
     * @property {number} crSanction 
     * @property {number} smrVerifMap 
     * @property {number} pPassReset
     * @property {number} altSmrVerifMap
     * @property {number} pAltReset
     */
    /**
     * 
     * @param {number} offset 
     * @param {SMREntry} smrEntry 
     * @param {SMRGenWithData} genInfo 
     * @param {Buffer[]} tagInfo 
     * @returns {HSERet}
     * @example
     * var smrEntry={
     *  pSmrSrc:0x20000000,
     *  smrSize:0x2000,
     *  configFlags:this.HSE_SMR_CFG_FLAG_INSTALL_AUTH,
     *  verifMethod:this.HSE_SMR_VERIF_PRE_BOOT_MASK,
     *  checkPeriod:0,
        keyHandle:this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0),
        pInstAuthTag:[0,0x20000000],
        authScheme:{
            type:'cmac-aes',
        }
     * }
       var genInfo={
            accessMode:this.HSE_ACCESS_MODE_ONE_PASS,
            entryIndex:0,
            smrData:Buffer.alloc(200,2),
        }
        var tagInfo={
            Buffer.alloc(0),Buffer.alloc(16,2)
        }
        this.smrInstallWithData(0x20000000,smrEntry,genInfo,tagInfo)
     */
    smrInstallWithData(offset, smrEntry, genInfo, tagInfo) {

    }

    /**
     * 
     * @param {number} offset 
     * @param {SMREntry} smrEntry 
     * @param {SMRGenWithoutData} genInfo 
     * @param {Buffer[]} tagInfo 
     * @returns {HSERet}
     * @example
     * var smrEntry={
     *  pSmrSrc:0x20000000,
     *  smrSize:0x2000,
     *  configFlags:this.HSE_SMR_CFG_FLAG_INSTALL_AUTH,
     *  verifMethod:this.HSE_SMR_VERIF_PRE_BOOT_MASK,
     *  checkPeriod:0,
        keyHandle:this.GET_KEY_HANDLE(this.HSE_KEY_CATALOG_ID_NVM,0,0),
        pInstAuthTag:[0,0x20000000],
        authScheme:{
            type:'cmac-aes',
        }
     * }
       var genInfo={
            accessMode:this.HSE_ACCESS_MODE_ONE_PASS,
            entryIndex:0,
            pSmrData:0x20000000,
            smrDataLength:0x2000
        }
        var tagInfo={
            Buffer.alloc(0),Buffer.alloc(16,2)
        }
        this.smrInstallWithoutData(0x20000000,smrEntry,genInfo,tagInfo)
     */
    smrInstallWithoutData(offset, smrEntry, genInfo, tagInfo) {

    }
    /**
     * 
     * @param {number} offset 
     * @param {number} index
     * @param {CREntry} crEntry 
     * @returns {HSERet}
     * @example
     * var crEntry={
     *  coreId:0,
     *  crSanction:this.HSE_CR_SANCTION_RESET_SOC,
     *  smrVerifMap:1,
     *  pPassReset:0x20000000,
     *  altSmrVerifMap:2,
     *  pAltReset:0x20004000
     * }
     * this.crInstall(0x20000000,0,crEntry)
     */
    crInstall(offset, index, crEntry) {

    }

    /**
     * 
     * @param {number} offset 
     * @param {number} index
     * @returns {HSERet}
     * @example
     * this.smrVerify(0x20000000,0)
     */
    smrVerify(offset, index) {

    }

    /**
         * 
         * @param {buffer} key - RAW ADKP 128bit
         * @returns {buffer}
         */
    calADKPExtension(key) {

    }
    /**
     * 
     * @param {buffer} key - RAM ADKP 128bit
     * @param {*} uid - Device ID 64bit
     */
    calADKPDeviceDep(key, uid) {
    }
}