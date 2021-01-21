import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router'

import Dashboard from '../views/Dashboard.vue'
import Wifi from '../views/Wifi.vue'
import About from '../views/About.vue'
import Settings from '../views/Settings.vue'
import Logs from '../views/Logs.vue'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    redirect: '/dashboard'
  },
  {
    path: '/index.html',
    redirect: '/dashboard'
  },
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: Dashboard
  },
  {
    path: '/wifi',
    name: 'Wifi',
    component: Wifi
  },
  {
    path: '/settings',
    name: 'Settings',
    component: Settings
  },
  {
    path: '/logs',
    name: 'Logs',
    component: Logs
  },
  {
    path: '/about',
    name: 'About',
    component: About
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
