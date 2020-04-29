import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import './plugins/element.js'

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
