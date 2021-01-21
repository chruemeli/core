<template>
  <div>
    <h2>Chrümeli</h2>
    <p>
      Authored by<br>
      <a href="http://rootlogin.ch">Simon Erhardt</a>
    </p>
    <p>
      Licensed under<br>
      <a href="https://www.gnu.org/licenses/lgpl-3.0.html">LGPL 3.0</a>
    </p>
    <p>
      Libraries used<br>
      <a href="https://github.com/esp8266/Arduino">ESP8266 Arduino SDK</a><br>
      <a href="https://arduinojson.org/">Arduino JSON</a><br>
      <a href="https://github.com/bakercp/CRC32">CRC32</a><br>
    </p>
  </div>
</template>

<script language="ts">
import { defineComponent, onMounted } from 'vue'
import { useStore } from 'vuex'

export default defineComponent({
  setup () {
    const store = useStore()

    onMounted(() => {
      store.commit('setTitle', 'About')
    })

    return {}
  }
})
</script>
