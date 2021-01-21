import axios from 'axios'
import SensorsStoreState from '@/models/store/states/SensorsStoreState'
import { Commit } from 'vuex'

const state: SensorsStoreState = {
  sensors: {}
}

export default {
  namespaced: true,
  state,
  getters: {
    get: (state: SensorsStoreState) : any => (sensorName: string) => {
      return state.sensors[sensorName]
    },
    getAll: (state: SensorsStoreState) : { [key: string]: any } => {
      return state.sensors
    }
  },
  actions: {
    async update ({ commit }: { commit: Commit }, sensorName: string) : Promise<void> {
      try {
        const response = await axios.get('/api/sensor/' + sensorName)

        commit('set', {
          sensorName: sensorName,
          sensorData: response.data
        })
      } catch (error) {
        console.error(error)
      }
    },
    async updateAll ({ commit }: { commit: Commit }) : Promise<void> {
      try {
        const response = await axios.get('/api/sensor')

        for (const key in response.data) {
          if (Object.prototype.hasOwnProperty.call(response.data, key)) {
            const val = response.data[key]

            commit('set', {
              sensorName: key,
              sensorData: val
            })
          }
        }
      } catch (error) {
        console.error(error)
      }
    }
  },
  mutations: {
    set (state: SensorsStoreState, payload: { sensorName: string; sensorData: any }) : void {
      state.sensors[payload.sensorName] = payload.sensorData
    },
    reset (state: SensorsStoreState) : void {
      state.sensors = []
    }
  }
}
