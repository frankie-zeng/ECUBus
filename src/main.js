import Vue from 'vue'
import App from './render/App.vue'
import router from './render/router'
import store from './render/store'
import './render/plugins/element.js'

import VueCodemirror from "vue-codemirror";
import "codemirror/lib/codemirror.css";
import "codemirror/mode/javascript/javascript.js"
Vue.use(VueCodemirror, {
  options: { 
    tabSize: 4,
    mode: "javascript",
    lineNumbers: true,
    // more CodeMirror options...
  },
} )
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
