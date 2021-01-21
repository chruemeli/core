import axios from 'axios'

import Status from '@/models/Status'
import StatusStoreState from '@/models/store/states/StatusStoreState'
import { Commit } from 'vuex'

const state: StatusStoreState = {
  status: {
    isCaptivePortalEnabled: false
  }
}

export default {
  namespaced: true,
  state,
  getters: {
    get (state: StatusStoreState): Status {
      return state.status
    }
  },
  actions: {
    async update ({ commit }: { commit: Commit }) : Promise<void> {
      try {
        const response = await axios.get('/api/status')

        commit('set', response.data)
      } catch (error) {
        console.error(error)
      }
    }
  },
  mutations: {
    set (state: StatusStoreState, newstate: Status) : void {
      state.status = newstate
    }
  }
}
