![Release CI](https://github.com/frankie-zeng/ECUBus/workflows/Release%20CI/badge.svg)
![Build CI](https://github.com/frankie-zeng/ECUBus/workflows/Build%20CI/badge.svg)
# **ECU-BUS** 
<img src="./src/render/assets/logo.png" width="50%">

ECU-BUS is a general-purpose ECU toolkit, which contains diagnostic(UDS), security, and other tools.

# Key Features
## **UDS Tester**
### CAN/CAN-FD UDS Teseter（DoCAN 15765-2）

#### Support Hardware Devcie:
- [x] PEAK

### DOIP UDS Tester（DoIP 13400-2）
- [x] Done

### LIN UDS Tester（DoLIN）
- [ ] TODO

## **KEY Generate Tool**
### SHE-KEY M-Martrix Generate
- [x] S32K1-CSEC
- [x] S32K3-HSE-SHE

# Software Framework
ECU-BUS Based on [Electron](https://www.electronjs.org)，Pure JS, Use [node addon](https://nodejs.org/api/n-api.html) to call .dll library,UI use [VUE](https://cn.vuejs.org/) and [Eelement-UI](https://element.eleme.cn/)

[node-gyp](https://github.com/nodejs/node-gyp) is necessary
## Project setup
```
npm install
```

### Compiles and hot-reloads for development
```
npm run electron:serve
```

### Compiles and minifies for production
```
npm run electron:build
```

### Lints and fixes files
```
npm run lint
```
# Software User Guide
## UDS
TODO

