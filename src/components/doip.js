/* eslint-disable no-unused-vars */
export default {
    '1': {
      value: 0x0001,
      name: 'Vehicle identification request message',
    },
    '2':{
        value:0x0002,
        name:'Payload type vehicle identification request message with EID',
        param:[
            {
                name:'EID',
                len:function(){
                    return [6,6]
                }
            }
        ]
    },
    '3':{
        value:0x0003,
        name:'Payload type vehicle identification request message with VIN',
        param:[
            {
                name:'VIN',
                len:function(){
                    return [17,17]
                }
            }
        ]
    },
    '5':{
        value:0X0005,
        name:'Routing activation request',
        param:[
            {
                name:'SA',
                readonly:true,
                msg:'SA等于地址信息里的SA',
                len:function(){
                    return [2,2]
                }
            },
            {
                name:'Active Type',
                len:function(){
                    return [1,1]
                }
            },
            {
                name:'Reserved',
                readonly:true,
                len:function(){
                    return [4,4]
                }
            },
            {
                name:'OEM Spec',
                len:function(){
                    return [0,4]
                }
            }
        ]
    },
    '4003':{
        value:0X4003,
        name:'Diagnostic power mode information request',
        
    },
    '4001':{
        value:0X4001,
        name:'DoIP entity status information request',
        
    }

  }
  