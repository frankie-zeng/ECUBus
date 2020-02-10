const path = require('path')

function resolve (dir) {
  return path.join(__dirname, dir)
}

module.exports = {
  pluginOptions: {
    electronBuilder: {
      builderOptions: {
        "extraFiles": [
          {
            "from": "build/Release",
            "to": "resources",
            "filter": [
              "*.node"
            ]
          }
        ],
        // options placed here will be merged with default configuration and passed to electron-builder
      },
      chainWebpackMainProcess: config => {
        config.module
          .rule('node')
          .test(/\.node$/)
          .use('node-loader')
          .loader('node-loader')
          .end()
      },
      chainWebpackRendererProcess: config => {
        config.module
          .rule('node')
          .test(/\.node$/)
          .use('node-loader')
          .loader('node-loader')
          .end()
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
