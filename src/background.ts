'use strict'

import { app, protocol, BrowserWindow, session } from 'electron'
import { createProtocol } from 'vue-cli-plugin-electron-builder/lib'
// import installExtension, { VUEJS3_DEVTOOLS } from 'electron-devtools-installer'
const isDevelopment = process.env.NODE_ENV !== 'production'
import { initialize, enable } from '@electron/remote/main'
import Store from 'electron-store';
import path from 'path'
import log from 'electron-log'


declare const __static: string;
initialize()
Store.initRenderer()

// Scheme must be registered before the app is ready
protocol.registerSchemesAsPrivileged([
  { scheme: 'app', privileges: { secure: true, standard: true } }
])



async function createWindow() {
  // Create the browser window.

  const loading = new BrowserWindow({
    show: false,
    frame: false, 
    width: 370,
    height: 125,

    resizable: false,
    transparent: true,
  });

  loading.loadFile(path.join(__static, "loading.html"));
  loading.once("ready-to-show", () => {
    loading.show();
    const win = new BrowserWindow({
      minWidth: 1000,
      minHeight: 600,
      show: false,
      webPreferences: {
        webSecurity: true,
        devTools: isDevelopment && !process.env.IS_TEST, // make sure it is disabled for e2e tests
        // Use pluginOptions.nodeIntegration, leave this alone
        // See nklayman.github.io/vue-cli-plugin-electron-builder/guide/security.html#node-integration for more info
        nodeIntegration: (process.env
          .ELECTRON_NODE_INTEGRATION as unknown) as boolean,
        contextIsolation: !(process.env
          .ELECTRON_NODE_INTEGRATION as unknown) as boolean
      }
    })
    enable(win.webContents)
    if (process.env.WEBPACK_DEV_SERVER_URL) {
      // Load the url of the dev server if in development mode
      win.loadURL(process.env.WEBPACK_DEV_SERVER_URL as string)
      if (!process.env.IS_TEST) win.webContents.openDevTools()
    } else {
      createProtocol('app')
      // Load the index.html when not in development
      win.loadURL('app://./index.html')
      win.setMenu(null)
    }
    win.once("ready-to-show", () => {
      loading.hide();
      loading.close();
      setTimeout(() => {
        win.maximize();
        // win.show()
      }, 500);
    });
  })



}


// Quit when all windows are closed.
app.on('window-all-closed', () => {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) createWindow()
})

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', async () => {
  registerLocalResourceProtocol()
  if (isDevelopment && !process.env.IS_TEST) {
    //Install Vue Devtools
    const devToolsPath = path.join(
      __dirname,'..','devtool'
    )
    try {
      //await installExtension(VUEJS3_DEVTOOLS)
      await session.defaultSession.loadExtension(devToolsPath)
    } catch (e: any) {
      console.error('Vue Devtools failed to install:', e.toString())
    }
  }
  createWindow()
})

function registerLocalResourceProtocol() {
  protocol.registerFileProtocol('local-resource', (request, callback) => {
    const url = request.url.replace(/^local-resource:\/\//, '')
    // Decode URL to prevent errors when loading filenames with UTF-8 chars or chars like "#"
    const decodedUrl = decodeURI(url) // Needed in case URL contains spaces
    try {
      return callback(decodedUrl)
    }
    catch (error) {
      console.error('ERROR: registerLocalResourceProtocol: Could not get file path:', error)
    }
  })
}



// Exit cleanly on request from parent process in development mode.
if (isDevelopment) {
  if (process.platform === 'win32') {
    process.on('message', (data) => {
      if (data === 'graceful-exit') {
        app.quit()
      }
    })
  } else {
    process.on('SIGTERM', () => {
      app.quit()
    })
  }
}
