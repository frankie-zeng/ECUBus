![Release CI](https://github.com/frankie-zeng/ECUBus/workflows/Release%20CI/badge.svg)
![Build CI](https://github.com/frankie-zeng/ECUBus/workflows/Build%20CI/badge.svg)
# **ECU-BUS** 
<img src="./src/assets/logo.png" width="30%">

ECU-BUS是一款通用PC端ECU测试工具。


# 核心功能
## **UDS客户端**
### 基于CAN/CAN-FD的UDS客户端（DoCAN 15765-2）


### 硬件列表
- [x] PEAK

### 基于Ethernet的UDS客户端（DoIP 13400-2）
- [ ] TODO

### 基于LIN的UDS客户端（DoLIN）
- [ ] TODO

## **KEY生成工具**
### SHE-KEY生成
- [x] S32K1-CSEC
- [ ] S32K3-HSE-SHE（TODO)

# 软件框架
ECU-BUS 基于[Electron](https://www.electronjs.org)实现，基于JS，对于dll的调用，使用[node addon](https://nodejs.org/api/n-api.html)实现,前端构建采用[VUE](https://cn.vuejs.org/)和[Eelement-UI](https://element.eleme.cn/)
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
