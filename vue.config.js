const path = require('path')

function resolve (dir) {
  return path.join(__dirname, dir)
}

module.exports = {
  pluginOptions: {
    electronBuilder: {
      nodeIntegration: true,
      builderOptions: {
        "copyright":"Copyright © 2020 ${author}",
        "productName":"ECUBus",
        "nodeGypRebuild":true,
        "extraFiles": [
          {
            "from": "public/peak",
            "to": "resources/peak",
            "filter": [
              "*.dll"
            ]
          },
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
          ]
        },
        'nsis': {
          'oneClick': false,
          'allowElevation': true,
          'allowToChangeInstallationDirectory': true,
          'installerIcon': './public/logo.ico',
          'uninstallerIcon': './public/logo.ico',
          'installerHeaderIcon': './public/logo.ico',
          'createDesktopShortcut': true,
          'createStartMenuShortcut': true,
          'license': 'LICENSE.txt'
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
            .use('native-ext-loader')
            .loader('native-ext-loader')
            .options({
              rewritePath: path.resolve(__dirname,"dist_electron"),
              emit: false
            })
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
      mainProcessWatch: ['src/uds/canuds.js','src/crypto/mp.js','src/uds/ipuds.js']
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
