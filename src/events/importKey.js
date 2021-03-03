const crypto = require('crypto')
import { ipcMain } from 'electron'

ipcMain.on('importKey', (event, arg) => {
    let ret = {
        err: true,
        msg: 'ok',
        type: 'sym',
    }
    let key;
    try {
        if (arg.way == 'secret') {
            key = crypto.createSecretKey(Buffer.from(arg.value, 'hex'))
        } else if (arg.way == 'private') {
            key = crypto.createPrivateKey(arg.value)
            ret.type=key.asymmetricKeyType
            //asymmetricKeyDetails support in node v15.7.0
            ret.details=key.asymmetricKeyDetails
        } else {
            key = crypto.createPublicKey(arg.value)
            ret.type=key.asymmetricKeyType
            ////asymmetricKeyDetails support in node v15.7.0
            ret.details=key.asymmetricKeyDetails
        }
    }catch(e){
        ret.err = false
        ret.msg=e.code
    }
    
    event.returnValue = ret
})
