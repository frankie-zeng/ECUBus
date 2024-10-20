
<h1>EcuBus Upgrade to EcuBus-Pro</h1>
<h2>https://github.com/frankie-zeng/EcuBus-Pro-Doc</h2>
---

![Release CI](https://github.com/frankie-zeng/ECUBus/workflows/Release%20CI/badge.svg)
![Build CI](https://github.com/frankie-zeng/ECUBus/workflows/Build%20CI/badge.svg)
# **ECU-BUS** 
<img width="250" alt="logo" src="https://user-images.githubusercontent.com/43364599/227956243-93baef8a-e343-4123-914d-f8591a37d750.png">

![2](https://user-images.githubusercontent.com/43364599/227956355-130a51fa-c9dd-440d-82a5-e557f7a3adfa.png)

# Real use case with NXP S32K1/S32K3 UDS bootloader
https://community.nxp.com/t5/S32K-Knowledge-Base/Unified-bootloader-Demo/ta-p/1423099

# Video 
ECU-BUS(NXP UDS bootloader) - 曾福的视频 - 知乎
https://www.zhihu.com/zvideo/1284963912635142144

# Roadmap Update
## 工具重构
会对整个ECU-Bus工具进行一个重构和大的升级，增加更多的功能，和性能提升
## PEAK硬件支持升级
会优化PEAK的硬件支持：
* 适配ISO 15765-2:2016的版本
* 增加更多CAN和CAN-FD波特率和采样点的配置
* 发送接收CAN标准帧的能力
* 增加跟多CAN-TP时间参数配置如：
  - BlockSize
  - ST
  - TIMEOUT_AS
  - TIMEOUT_AR
  - TIMEOUT_BS
  - TIMEOUT_CR
  - ...
## 更多硬件的支持：
目前首先会计划支持Kvaser can的这款设备：
![image](https://user-images.githubusercontent.com/43364599/227955816-126adf55-43a8-41eb-a2b9-420b73a7f8b6.png)


## 欢迎大家进入QQ群(807883635)进行交流:
![Ecu-Bus群聊二维码](https://user-images.githubusercontent.com/43364599/227955335-a0148424-441e-4919-a9f3-47ca0504c3a7.png)


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

