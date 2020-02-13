import Vue from 'vue'
import VueRouter from 'vue-router'
import PCAN from './../views/Pcan.vue'
import HOME from './../views/Home.vue'
import SECURITY from './../views/Security.vue'

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
    path: '/security',
    name: 'security',
    component: SECURITY
  }
]

const router = new VueRouter({
  routes
})

export default router
