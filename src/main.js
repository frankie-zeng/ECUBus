import Vue from 'vue'
import App from './render/App.vue'
import router from './render/router'
import store from './render/store'
import './render/plugins/element.js'

import VueCodemirror from "vue-codemirror";
import "codemirror/lib/codemirror.css";
import "codemirror/mode/javascript/javascript.js"
import "codemirror/theme/eclipse.css";
import CodeMirror from 'codemirror'
CodeMirror.defineOption("fullScreen", false, function(cm, val, old){
  if (old == CodeMirror.Init) old = false;
  if (!old == !val) return;
  if (val) setFullscreen(cm);
  else setNormal(cm);
});
function setFullscreen(cm) {
  var wrap = cm.getWrapperElement();
  cm.state.fullScreenRestore = {scrollTop: window.pageYOffset, scrollLeft: window.pageXOffset,
                                width: wrap.style.width, height: wrap.style.height};
  wrap.style.width = "";
  wrap.style.height = "auto";
  wrap.style.position='fixed';
  wrap.className += " CodeMirror-fullscreen";
  document.documentElement.style.overflow = "hidden";
  cm.refresh();
}

function setNormal(cm) {
  var wrap = cm.getWrapperElement();
  wrap.className = wrap.className.replace(/\s*CodeMirror-fullscreen\b/, "");
  document.documentElement.style.overflow = "";
  var info = cm.state.fullScreenRestore;
  wrap.style.width = info.width; wrap.style.height = info.height;
  wrap.style.position='relative';
  window.scrollTo(info.scrollLeft, info.scrollTop);
  cm.refresh();
}


Vue.use(VueCodemirror, {
  options: { 
    tabSize: 4,
    indentUnit: 4,
    mode: "javascript",
    lineNumbers: true,
    theme: 'eclipse'
    // more CodeMirror options...
  },
} )
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
