<template>
  <div>
    <div class="container">
      <form @submit.prevent="saveConfig">
        <h3>Device settings</h3>
        <div class="input-group">
            <label>Device name</label>
            <input type="text" placeholder="Device name" v-model="deviceName">
          </div>
        <button type="submit" class="success">Save</button>
      </form>

      <form @submit.prevent="deleteData">
        <h3>Delete sensor data</h3>
        <p>You can delete all sensor data. This will remove all data until the next presentation.</p>
        <button type="submit" class="warning">Delete</button>
      </form>

      <form @submit.prevent="resetDevice">
        <h3>Firmware reset</h3>
        <p>This will reset your device to factory settings. All data including WiFi settings are removed. You will need to configure this device again.</p>
        <button type="submit" class="danger">Reset</button>
      </form>
    </div>

    <div v-if="showModal" class="modal">
      <div class="modal-container">
        <h3>Reconnect device</h3>
        <div class="loading"></div>
        <p>
          Your microcontroller has been<br>
          reset. You need to connect again.<br>
          Wait a few seconds before<br>
          reconnecting.
        </p>
      </div>
    </div>
    <div v-if="showModalRestart" class="modal">
      <div class="modal-container">
        <h3>Reconnect device</h3>
        <div class="loading"></div>
        <p>
          Your microcontroller has been<br>
          restarted. Please wait a few<br>
          seconds. This window will close<br>
          automatically.
        </p>
      </div>
    </div>
  </div>
</template>

<script language="ts">
import axios from 'axios'

import { ref, defineComponent, onMounted } from 'vue'
import { useStore } from 'vuex'

export default defineComponent({
  setup () {
    const showModal = ref(false)
    const showModalRestart = ref(false)
    const deviceName = ref(false)

    const store = useStore()

    async function saveConfig () {
      try {
        const response = await axios.post('/api/config', {
          name: deviceName.value
        })

        if (response.data.restart !== undefined) {
          if (response.data.restart) {
            showModalRestart.value = true

            setTimeout(() => {
              showModalRestart.value = false
            }, 5000)
          }
        }
      } catch (error) {
        console.error(error)
      }
    }

    async function deleteData () {
      try {
        await axios.delete('/api/sensor')
        store.commit('sensors/reset')
      } catch (error) {
        console.error(error)
      }
    }

    async function resetDevice () {
      if (confirm('Do you really want to reset your device?')) {
        this.showModal = true

        try {
          await axios.post('/api/reset')
        } catch (error) {
          console.error(error)
        }
      }
    }

    onMounted(() => {
      store.commit('setTitle', 'Configuration')

      store.dispatch('config/update').then(() => {
        deviceName.value = store.getters['config/get'].name
      })
    })

    return {
      showModal,
      showModalRestart,
      deviceName,
      saveConfig,
      deleteData,
      resetDevice
    }
  }
})
</script>
