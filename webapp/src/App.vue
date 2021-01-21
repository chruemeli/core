<template>
  <div id="app">
    <header>
      <div class="container">
        <div class="left">
        </div>
        <div class="title">
          {{ title }}
        </div>
        <div class="right">
        </div>
      </div>
    </header>

    <div v-if="status.isCaptivePortalEnabled" class="alert warning">
      <i class="icon-exclamation" /> Captive portal mode enabled! Please <router-link to="/wifi">connect a wifi</router-link>!
    </div>

    <div id="appView">
      <router-view />
    </div>

    <footer id="appFooter">
      <nav>
          <router-link to="/dashboard"><i class="icon-home"></i></router-link>
          <router-link to="/wifi"><i class="icon-wifi"></i></router-link>
          <router-link to="/settings"><i class="icon-gear"></i></router-link>
          <router-link to="/logs"><i class="icon-file-text"></i></router-link>
          <router-link to="/about"><i class="icon-info"></i></router-link>
      </nav>
    </footer>
  </div>
</template>

<script language="ts">
import { defineComponent, computed, onMounted, onUnmounted } from 'vue'
import { useStore } from 'vuex'

export default defineComponent({
  setup () {
    const store = useStore()
    const status = computed(() => store.getters['status/get'])
    const title = computed(() => store.getters.getTitle)

    let statusUpdateInterval = null

    onMounted(() => {
      store.dispatch('status/update')

      statusUpdateInterval = setInterval(() => {
        store.dispatch('status/update')
      }, 5000)
    })

    onUnmounted(() => {
      clearInterval(statusUpdateInterval)
    })

    return {
      status,
      title
    }
  }
})
</script>
