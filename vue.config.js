const path = require('path')

function resolve (dir) {
  return path.join(__dirname, dir)
}

module.exports = {
  pluginOptions: {
    configureWebpack: {
      externals: {
        hse_cfg_js: 'require("hse_cfg_js")'
      }
    },
    electronBuilder: {
      nodeIntegration: true,
      builderOptions: {
        "appId": "ECUBus",
        "copyright":"Copyright © 2020 ${author}",
        "productName":"ECUBus",
        "nodeGypRebuild":false,
        "extraFiles": [
          {
            "from": "public/peak",
            "to": "resources/peak",
            "filter": [
              "*.dll"
            ]
          },
          {
            
            "from": "public/api_helper",
            "to": "resources/api_helper",
            "filter": [
              "**/*"
            ]
          }
        ],
        "win": {  
          "icon": './public/logo.ico',
          "target": [
            {
              "target": "nsis" ,
              'arch': [
                'x64'
              ]
            }
          ],
          "publish": [
            "github"
          ]
        },
        'nsis': {
          'oneClick': true,
          'allowElevation': true,
          'perMachine': true,
          'allowToChangeInstallationDirectory': false,
          'installerIcon': './public/logo.ico',
          'uninstallerIcon': './public/logo.ico',
          'installerHeaderIcon': './public/logo.ico',
          'createDesktopShortcut': true,
          'createStartMenuShortcut': true,
          'license': 'LICENSE.txt',
          'artifactName': 'ECUBus.exe',
          'guid':'c00f7959-9630-4566-b0cb-9c6a154ec9b2',
          'include':'uninstall.nsh'
        },
        // options placed here will be merged with default configuration and passed to electron-builder
      },
      chainWebpackMainProcess: config => {
        if (process.env.NODE_ENV === 'production') {
          config.module
            .rule('node')
            .test(/\.node$/)
            .use('native-ext-loader')
            .loader('native-ext-loader')
            .options({
              rewritePath: process.resourcesPath
            })
            .end()
        }else{
          config.module
            .rule('node')
            .test(/\.node$/)
            .use('node-loader')
            .loader('node-loader')
            .end()
        }
      },
      // chainWebpackRendererProcess: config => {
      //   if (process.env.NODE_ENV === 'production') {
      //     config.module
      //       .rule('node')
      //       .test(/\.node$/)
      //       .use('native-ext-loader')
      //       .loader('native-ext-loader')
      //       .options({
      //         rewritePath: process.resourcesPath
      //       })
      //       .end()
      //   }else{
      //     config.module
      //       .rule('node')
      //       .test(/\.node$/)
      //       .use('native-ext-loader')
      //       .loader('native-ext-loader')
      //       .options({
      //         rewritePath: path.resolve(__dirname,"dist_electron")
      //       })
      //       .end()
      //   }
      // },
      mainProcessWatch: ['src/uds/canuds.js','src/crypto/mp.js','src/uds/ipuds.js','src/uds/decode.js','src/uds/lpuds.js','src/uds/uds.js','src/uds/linuds.js']
    }
  },
  publicPath: './',
  //   devServer: {
  //     // can be overwritten by process.env.HOST
  //     host: '0.0.0.0',
  //     port: 8080
  //   },
  chainWebpack: config => {
    config.resolve.alias
      .set('@', resolve('src'))
      .set('src', resolve('src'))
      .set('common', resolve('src/common'))
      .set('components', resolve('src/components'))
      .set('public', resolve('public'))
  }
}
