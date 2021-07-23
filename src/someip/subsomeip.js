/* eslint-disable no-undef */

const path = require("path")
// const isDevelopment = process.env.NODE_ENV !== 'production'
// const dllPath = isDevelopment ? path.join(__static,'someip') : path.join(process.resourcesPath, 'someip')
// const configFile = isDevelopment ? path.join(__static,'someip','vsomeip.json') : path.join(process.resourcesPath, 'someip','vsomeip.json')
// console.log(process.argv)
const dllPath = path.join(process.argv[2],'someip') 
const configFile = path.join(process.argv[2],'someip','vsomeip.json')
console.log(`DLL Load Path:${dllPath}`)
console.log(`Configurate File:${configFile}`)

process.env['VSOMEIP_CONFIGURATION'] = configFile;
const SOMEIP = require("./someipnode.js")
var apps=new Map()
process.on('message',(m)=>{
    console.log(m)
    if(m.method=='someipCreateApp'){
        var app= new SOMEIP(dllPath,m.arg.name)
        apps.set(m.arg.name,app)
        app.CreateApp()
        if(m.arg.route){
            app.IsRouting()
        }
    }else if(m.method=='someipStartApp'){
        apps.get(m.arg).StartApp()
    }
})
process.once('SIGTERM', function () {
    let keys=apps.keys();
    for(let i in keys){
        let app=apps.get(i)
        app.StopApp()
    }
});

// // eslint-disable-next-line no-unused-vars
// const routed= new SOMEIP(dllPath,"vsomeipd")
// routed.CreateApp()
// routed.IsRouting()
// routed.StartApp()
// var someip= new SOMEIP(dllPath,"client")
// someip.RegisterStateHandler((val)=>{
//     if(val==0){
//         someip.OfferService(0x1234,0x2345);
//         someip.RequestService(0x1111,0x2222);
//     }
// })
// someip.RegisterAvlHandler(0x1111,0x2222,(service,inst,avl)=>{
//     console.log("avl",service,inst,avl)
//     if(avl){
//         someip.Send({
//             'service':0x1111,
//             'instance':0x2222,
//             'method':0x3333,
//             'payload':Buffer.from("NXP"),
//             'reliable':true
//         })
//     }
// })
// someip.RegisterMsgHandler(0x1111,0x2222,0x3333,(msg)=>{
//     console.log(msg)
// })
// console.log("createApp")
// someip.CreateApp()
// someip.StartApp()
// setTimeout(()=>{
//     someip.StopApp()
//     setTimeout(()=>{
//         console.log("start again")
//         someip = null
//         someip= new SOMEIP(dllPath,"client")
//         console.log("start again1")
//         someip.RegisterStateHandler((val)=>{
//             if(val==0){
//                 someip.OfferService(0x1234,0x2345);
//                 someip.RequestService(0x1111,0x2222);
//             }
//         })
//         someip.RegisterAvlHandler(0x1111,0x2222,(service,inst,avl)=>{
//             console.log("avl",service,inst,avl)
//             if(avl){
//                 someip.Send({
//                     'service':0x1111,
//                     'instance':0x2222,
//                     'method':0x3333,
//                     'payload':Buffer.from("NXP"),
//                     'reliable':true
//                 })
//             }
//         })
//         someip.RegisterMsgHandler(0x1111,0x2222,0x3333,(msg)=>{
//             console.log(msg)
//         })
//         someip.CreateApp()
//         someip.StartApp()
//     },2000)
// },5000)
