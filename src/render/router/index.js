import Vue from 'vue'
import VueRouter from 'vue-router'
import PCAN from './../views/Pcan.vue'
import HOME from './../views/Home.vue'
import SHE from './../views/SHE.vue'
import DOIP from './../views/DOIP.vue'
import UDS_EMULATE from './../views/udsEmulate.vue'

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
  }
]

const router = new VueRouter({
  routes
})

export default router
