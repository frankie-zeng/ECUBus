import Vue from 'vue'
import VueRouter from 'vue-router'
import PCAN from './../views/Pcan.vue'
import LIN from './../views/Lin.vue'
import HOME from './../views/Home.vue'
import SHE from './../views/SHE.vue'
import DOIP from './../views/DOIP.vue'
import UDS_EMULATE from './../views/udsEmulate.vue'
import Certificate from './../views/Certificate.vue'
Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'home',
    component: HOME
  },
  {
    path: '/pcan',
    name: 'pcan',
    component: PCAN
  },
  {
    path: '/udsEmulate',
    name: 'udsEmulate',
    component: UDS_EMULATE
  },
  {
    path: '/security',
    name: 'security',
    component: SHE
  },
  {
    path: '/doip',
    name: 'doip',
    component: DOIP
  },
  {
    path: '/certificate',
    name: 'certificate',
    component: Certificate
  },
  {
    path: '/lin',
    name: 'lin',
    component: LIN
  },
]

const router = new VueRouter({
  routes
})

export default router
