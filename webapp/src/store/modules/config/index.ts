import axios from 'axios'
import { Commit } from 'vuex'

import Config from '@/models/Config'
import ConfigStoreState from '@/models/store/states/ConfigStoreState'

const state: ConfigStoreState = {
  config: {
    name: ''
  }
}

export default {
  namespaced: true,
  state,
  getters: {
    get (state: ConfigStoreState): Config {
      return state.config
    }
  },
  actions: {
    async update ({ commit }: { commit: Commit }) : Promise<void> {
      try {
        const response = await axios.get('/api/config')

        commit('set', response.data)
      } catch (error) {
        console.error(error)
      }
    }
  },
  mutations: {
    set (state: ConfigStoreState, newstate: Config) : void {
      state.config = newstate
    }
  }
}
