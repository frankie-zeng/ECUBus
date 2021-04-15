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
function get(key){

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
function openFile(filename, flag = 'r'){

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
function changeNextFrame(name, value = []){

}
/**
 * 
 * @param {number} service - UDS service id
 * @param {array} payload - UDS payload value except serivce id 
 * @param {func} func - UDS response handle function
 * @param {func} preFunc - UDS pre-handle function
 * @example
 * this.inserItem(0x36,[0x55,0x33],(writeData,readData)=>{
 *      this.log(writeData)
 * })
 */
function insertItem(service, payload, func = (writeData, readData) => { return true }, preFunc = (writeData) => { }){

}

/**
 * @returns {string} Get internal openssl verison.
 * @example this.getSSLVersion()
 */
function getSSLVersion(){
    
}
/**
 * @description Get catalog value from key handle.
 * @param {number} keyHandle 
 * @returns {number} the key catalog value.
 */
function GET_CATALOG_ID(keyHandle){

}
/**
 * @description Get group value form key handle. 
 * @param {number} keyHandle
 * @returns {number} the key group value.
 */
function GET_GROUP_IDX(keyHandle){
}
/**
 * @description Get slot value form key handle. 
 * @param {number} keyHandle
 * @returns {number} the key slot value.
 */
function GET_SLOT_IDX(keyHandle){
}
/**
 * @description Get keyHandle from catalog,gorup,slot.
 * @param {number} catalog - HSE catalog value 
 * @param {number} group - HSE group value
 * @param {number} slot - HSE slot value
 * @returns {number} 
 * @example
 * this.GET_KEY_HANDLE(this.hse.HSE_KEY_CATALOG_ID_NVM,0,0)
 */
function GET_KEY_HANDLE(catalog,group,slot){
    
}
/**
 * @description Calculate gmac value.
 * @param {buffer} key Gmac key
 * @param {buffer} value 
 * @param {buffer} iv Default IV is f7a093cd1ce82f11bc318b3e for calculate IVT/XRDC/APPBL
 * @returns {buffer} Return gmac result.
 */
function gmac(key,value,iv=Buffer.from('f7a093cd1ce82f11bc318b3e','hex')){
}
/**
 * @description Calculate cmac value.
 * @param {buffer} key 
 * @param {buffer} value 
 * @returns {buffer} Return cmac result.
 */
function cmac(key,value){
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
 * this.hse.importSheKey(0x20000000,
 *          this.hse.sheKey.MASTER_ECU_KEY,Buffer.alloc(16,0),
 *          this.hse.sheKey.MASTER_ECU_KEY,Buffer.from('000102030405060708090a0b0c0d0e0f','hex'),
 *          {
 *              VERIFY_ONLY:true,
 *              KEY_USAGE:true,
 *          },
 *          1,
 *          Buffer.alloc(8,0)
 * )
 */
function importSheKey(offset,keyAuthId,keyAuthValue,keyId, keyValue,flag,cid,uid){
    
}
/**
 * 
 * @param {number} offset 
 * @param {boolean} challenge - True: enable challenge response mode, Flase: password mode.
 * @returns {HSERet}
 */
function setDebugAuthMode(offset,challenge){

}

/**
 * 
 * @param {number} offset 
 * @param {buffer} key - ADKP key, 16 bytes.
 * @returns {HSERet}
 */
function setDebugKey(offset,key){

}

/**
 * 
 * @param {number} offset 
 * @param {number} lc
 * @returns {HSERet}
 */
 function setLifeCycle(offset,lc){

}
/**
 * @description Enable GMAC check
 * @param {number} offset 
 * @param {boolean} enable 
 * @returns {HSERet}
 */
function setBootAuth(offset,enable){
    
}
/**
 * 
 * @param {number} offset 
 * @param {boolean} enADKm - defines or returns ADKP_MASTER 
 * @param {boolean} startAsUser - efines or returns the value of CUST_START_AS_USER
 * @returns {HSERet}
 */
function setCustSecurityPolicy(offset,enADKm,startAsUser){
  
}
/**
 * 
 * @param {number} offset 
 * @param {boolean} startAsUser - defines or returns the value of OEM_START_AS_USER
 * @returns {HSERet}
 */
function setOemSecurityPolicy(offset,startAsUser){

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
function setMu(offset,muConfig=[]){

}
/**
 * 
 * @param {number} offset 
 * @param {boolean} enable 
 * @returns {HSERet}
 */
function setTamper(offset,enable){

}
/**
 * 
 * @param {number} offset 
 * @param {number} divider 
 * @returns {HSERet}
 */
function setFirc(offset,divider)
{
    
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
 */
function importSymKey(offset,keyHandle,keyInfo,key){

}

/**
 * @returns {HSERet}
 * @param {number} offset 
 * @param {number} keyHandle 
 * @param {KeyInfo} keyInfo 
 * @param {Buffer} key 
 */
function importPubKey(offset,keyHandle,keyInfo,key){

}

/**
 * @returns {HSERet}
 * @param {number} offset 
 * @param {number} keyHandle 
 * @param {KeyInfo} keyInfo 
 * @param {Buffer} key 
 */
function importPrivKey(offset,keyHandle,keyInfo,key){

}


/**
 * @typedef {Object} Catalog
 * @property {number} groupOwner 
 * @property {number} keyType 
 * @property {number} maxKeyBitLen 
 * @property {number} muMask
 * @property {number} numOfKeySlots
 * 
 */


/**
 * @param {number} offset
 * @param {Catalog[]} nvm 
 * @param {Catalog[]} ram
 * @returns {HSERet} 
 */
function formatCatalog(offset,nvm,ram){

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
 */
function smrInstallWithData(offset,smrEntry,genInfo,tagInfo){

}

/**
 * 
 * @param {number} offset 
 * @param {SMREntry} smrEntry 
 * @param {SMRGenWithoutData} genInfo 
 * @param {Buffer[]} tagInfo 
 * @returns {HSERet}
 */
 function smrInstallWithoutData(offset,smrEntry,genInfo,tagInfo){

}
/**
 * 
 * @param {number} offset 
 * @param {number} index
 * @param {CREntry} crEntry 
 * @returns {HSERet}
 */
function crInstall(offset,index,crEntry){

}
