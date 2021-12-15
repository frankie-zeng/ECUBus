/* eslint-disable no-unused-vars */

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
