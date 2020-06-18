const app = require('express')()
let port
const server=app.listen(0,'127.0.0.1',() => {
    port=server.address().port
    console.log(port)
});