<template>
  <div>
    <div class="container">

      <div v-if="page === 0">
        <div class="card-group">
          <div v-if="state.wifis.length === 0" class="loading"></div>
          <div class="card-wifi" v-for="wifi in state.wifis" :key="wifi.ssid">
            <div class="content">
              <div class="ssid">{{ wifi.ssid }}</div>
              <div class="info">Encryption: {{ getEncryptionName(wifi.secure) }} | RSSI: {{ wifi.rssi }} | Channel: {{ wifi.channel }} | BSSID: {{ wifi.bssid }}</div>
            </div>
            <a @click="configureWifi(wifi)">
              <i class="icon-arrow-right"></i>
            </a>
          </div>
          <hr>
          <div class="card-wifi">
            <div class="content">
              <div class="ssid">Connect manually</div>
            </div>
            <a @click="configureWifi(null)">
              <i class="icon-screwdriver"></i>
            </a>
          </div>
        </div>
      </div>
      <div v-if="page === 1">
        <form @submit.prevent="setWifi">
          <div class="input-group">
            <label>Network SSID</label>
            <input type="text" placeholder="Network SSID" v-model="networkSettings.ssid">
          </div>
          <div class="input-group">
            <input type="checkbox" v-model="networkSettings.isEncryptionEnabled">
            <label>Enable encryption</label>
          </div>
          <div v-if="networkSettings.isEncryptionEnabled" class="input-group">
            <label>Network password</label>
            <input type="password" placeholder="Network password" v-model="networkSettings.psk">
          </div>

          <button type="submit" class="success">Connect <i class="icon-arrow-right"></i></button>
        </form>
      </div>
    </div>

    <div v-if="showModal" class="modal">
      <div class="modal-container">
        <h3>Reconnect network</h3>
        <div class="loading"></div>
        <p>
          Your microcontroller tries now<br>
          to connect with your configured Wifi.<br>
          Connect now with your network. You<br>
          may find the new IP in your Router<br>
          configuration.
        </p>
      </div>
    </div>
  </div>
</template>

<script language="ts">
import axios from 'axios'
import { defineComponent, computed, onMounted, onUnmounted, ref, reactive } from 'vue'
import { useStore } from 'vuex'

import EncryptionType from '../models/EncryptionType'

export default defineComponent({
  setup () {
    const store = useStore()
    const state = reactive({ wifis: []})

    const showModal = ref(false)
    const page = ref(0)
    const networkSettings = reactive({
      ssid: '',
      isEncryptionEnabled: false,
      psk: ''
    })

    let wifiUpdateInterval = null

    function getEncryptionName (encType) {
      switch (encType) {
        case EncryptionType.ENC_TYPE_WPA:
          return 'WPA-PSK'
        case EncryptionType.ENC_TYPE_WPA2:
          return 'WPA2-PSK'
        case EncryptionType.ENC_TYPE_WEP:
          return 'WEP'
        case EncryptionType.ENC_TYPE_NONE:
          return 'open'
        case EncryptionType.ENC_TYPE_AUTO:
          return 'WPA/WPA2-PSK'
      }
    }

    function configureWifi (wifi) {
      if (wifi != null) {
        this.networkSettings.ssid = wifi.ssid
        this.networkSettings.isEncryptionEnabled = (wifi.secure !== EncryptionType.ENC_TYPE_NONE)
      } else {
        this.networkSettings.ssid = ''
        this.networkSettings.isEncryptionEnabled = false
      }

      this.networkSettings.psk = ''

      this.page = 1
    }

    async function setWifi () {
      clearInterval(wifiUpdateInterval)
      this.showModal = true

      try {
        const wifiObj = {}
        wifiObj.ssid = this.networkSettings.ssid

        if (this.networkSettings.isEncryptionEnabled) {
          wifiObj.psk = this.networkSettings.psk
        }

        await axios.post('/api/wifi', wifiObj)
      } catch (error) {
        console.error(error)
      }
    }

    async function setWifiScanEnabled (isEnabled) {
      const req = {
        "scan-wifi": isEnabled
      }

      try {
        await axios.post('/api/config', req)
      } catch (error) {
        console.error(error)
      }
    }

    async function updateWifiNetworks () {
      try {
        const response = await axios.get('/api/wifi')

        if (response.data !== null) {
          state.wifis = response.data
          console.log(response.data);
          console.log(wifis);
        }
      } catch (error) {
        console.error(error)
      }
    }

    onMounted(() => {
      store.commit('setTitle', 'WiFi settings')

      setWifiScanEnabled(true)
      wifiUpdateInterval = setInterval(() => {
        updateWifiNetworks();
      }, 3000)
    })

    onUnmounted(() => {
      setWifiScanEnabled(false)
      clearInterval(wifiUpdateInterval)
    })

    return {
      page,
      state,
      getEncryptionName,
      configureWifi,
      networkSettings,
      setWifi,
      showModal
    }
  }
})
</script>
