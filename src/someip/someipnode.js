const SOMEIPNode = require('./../../build/Release/SOMEIP.node')
class SOMEIP extends SOMEIPNode.SOMEIP{
    constructor(path,appName){
        super(path,appName)
    }
}
module.exports = SOMEIP