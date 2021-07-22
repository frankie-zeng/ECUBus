const cp = require('child_process');



var opt = { silent: true };
const n = cp.fork('./src/someip/someip.js', [], opt);
n.send({ method: 'CreateApp', name: 'vsomeipd', route: true });
n.send({ method: 'StartApp', name: 'vsomeipd' });
n.stdout.on('data', (data) => {
    console.log(`Received chunk ${data}`);
})
// setTimeout(() => {
//     n.kill()
//     setTimeout(() => {
//         console.log("restart#####")
//         const n = cp.fork('./src/someip/someip.js');
//         n.send({ method: 'CreateApp', name: 'vsomeipd', route: true });
//         n.send({ method: 'StartApp', name: 'vsomeipd' });
//     }, 1000)
// }, 2000)