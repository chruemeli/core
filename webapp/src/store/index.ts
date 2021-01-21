import { createStore } from 'vuex'

import StatusStoreModule from './modules/status'
import SensorsStoreModule from './modules/sensors'
import ConfigStoreModule from './modules/config'
import AppStoreState from '@/models/store/states/AppStoreState'

const state: AppStoreState = {
  title: 'IOT Framework'
}

export default createStore({
  state,
  getters: {
    getTitle (state: AppStoreState): string {
      return state.title
    }
  },
  actions: {},
  mutations: {
    setTitle (state: AppStoreState, title: string) {
      state.title = title
    }
  },
  modules: {
    status: StatusStoreModule,
    sensors: SensorsStoreModule,
    config: ConfigStoreModule
  }
})
