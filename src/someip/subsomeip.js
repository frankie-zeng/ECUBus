/* eslint-disable no-undef */
const http = require('http');
const path = require("path");
const dllPath = process.argv[2]
const configFile = path.join(process.argv[3],'vsomeip.json')
process.stdout.write(`DLL Load Path:${dllPath}\r\nConfigurate File:${configFile}\r\n`)

process.env['VSOMEIP_CONFIGURATION'] = configFile;
const SOMEIP = require("./someipnode.js")
var apps = new Map()

const server = http.createServer((req, res) => {
    if (req.method == 'POST') {
        var body = ''
        req.on('data', (data) => {
            body += data
        })
        req.on('end', () => {
            let val = JSON.parse(body)
            res.writeHead(200);
            if (req.url == '/someipCreateApp') {
                let app = new SOMEIP(dllPath, val.name)
                let err = 0;
                apps.set(val.name, app)
                err = app.CreateApp()
                if ((err == 0) && (val.route)) {
                    app.IsRouting()
                }
                res.end(JSON.stringify({ error: err }));
            } else if (req.url == '/someipStartApp') {
                res.end(JSON.stringify({
                    error: apps.get(val.name).StartApp()
                }));
            } else if (req.url == '/someipSendRequest') {
                apps.get(val.name).RegisterAvlHandler(val.service, val.instance, (service, instance, avl) => {
                    process.send({
                        method: 'avl',
                        name: val.name,
                        service: service,
                        instance: instance,
                        avl: avl
                    })
                })
                res.end(JSON.stringify({
                    error: apps.get(val.name).RequestService(val.service, val.instance)
                }))
            } else if (req.url == '/someipDeleteRequest') {
                apps.get(val.name).UnRegisterAvlHandler(val.service, val.instance)
                apps.get(val.name).ReleaseService(val.service, val.instance)
                res.end(JSON.stringify({
                    error: 0
                }))
            } else if (req.url == '/someipSend') {
                apps.get(val.name).RegisterMsgHandler(val.service, val.instance, val.method, (msg) => {
                    process.send({
                        method: 'msg',
                        name: val.name,
                        msg: msg
                    })
                })
                let req = {
                    service: val.service,
                    instance: val.instance,
                    method: val.method,
                    reliable: val.reliable,
                    payload: Buffer.from(val.payload, 'ascii')
                };
                apps.get(val.name).Send(req)
                res.end(JSON.stringify({
                    error: 0
                }))
            } else if (req.url == '/someipOffer') {
                apps.get(val.name).OfferService(val.service, val.instance)
                res.end(JSON.stringify({
                    error: 0
                }))
            } else if (req.url == '/someipStopOffer') {
                apps.get(val.name).StopOfferService(val.service, val.instance)
                res.end(JSON.stringify({
                    error: 0
                }))
            } else if (req.url == '/someipAddResponse') {
                apps.get(val.name).RegisterMsgHandler(val.service, val.instance, val.method, (msg) => {
                    process.send({
                        method: 'msg',
                        name: val.name,
                        msg: msg
                    })
                    apps.get(val.name).Response(msg,Buffer.from(val.payload,'ascii'))
                })
                res.end(JSON.stringify({
                    error: 0
                }))
            } else if (req.url == '/someipDeleteResponse') {
                apps.get(val.name).UnRegisterMsgHandler(val.service, val.instance, val.method)
                res.end(JSON.stringify({
                    error: 0
                }))
            }
        })
    }
});
server.on('listening', () => {
    process.send({
        method: 'port',
        port: server.address().port
    })
})
process.on("SIGINT", () => {
    let keys = apps.keys();
    for (let i in keys) {
        let app = apps.get(i)
        app.StopApp()
    }
    server.close()
});
server.listen(0)