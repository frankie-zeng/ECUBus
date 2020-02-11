const path = require('path')

function resolve (dir) {
  return path.join(__dirname, dir)
}

module.exports = {
  pluginOptions: {
    electronBuilder: {
      nodeIntegration: true,
      builderOptions: {
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
              rewritePath: path.resolve(__dirname,"dist_electron")
            })
            .end()
        }
      },
      chainWebpackRendererProcess: config => {
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
              rewritePath: path.resolve(__dirname,"dist_electron")
            })
            .end()
        }
      },
      mainProcessWatch: ['src/uds/canuds.js']
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
      .set('ADDON', resolve('build/Release'))
  }
}
