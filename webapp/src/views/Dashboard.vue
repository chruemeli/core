<template>
  <div>
    <div class="container">
      <div class="card-group">
        <div v-if="sensors.length === 0" class="loading"></div>
        <div v-for="sensor in sensors" :key="sensor.name" class="card-sensor">
          <h4 class="name">{{ sensor.name }}</h4>
          <p class="value">{{ sensor.value }}</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script language="ts">
import { defineComponent, onMounted, onUnmounted, computed } from 'vue'
import { useStore } from 'vuex'

export default defineComponent({
  setup () {
    let sensorUpdateInterval = null
    const store = useStore()

    const sensors = computed(() => {
      const sensorList = store.getters['sensors/getAll']

      const ret = []
      for (const name in sensorList) {
        // check if the property/key is defined in the object itself, not in parent
        if (Object.prototype.hasOwnProperty.call(sensorList, name)) {
          const sensor = sensorList[name]

          let value = null
          switch (sensor.type) {
            case 'temperature':
              value = `${sensor.value} °C`
              break
            case 'humidity':
              value = `${sensor.value} %`
              break
            case 'integer':
            case 'float':
              value = `${sensor.value}`
              break
          }

          if (value !== null) {
            ret.push({
              name: sensor.name,
              type: sensor.type,
              value: value
            })
          }
        }
      }

      return ret
    })

    onMounted(() => {
      store.commit('setTitle', 'Dashboard')

      sensorUpdateInterval = setInterval(() => {
        store.dispatch('sensors/updateAll')
      }, 5000)
    })

    onUnmounted(() => {
      clearInterval(sensorUpdateInterval)
    })

    return {
      sensors
    }
  }
})
</script>
