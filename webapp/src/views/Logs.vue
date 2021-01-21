<template>
  <div class="container" style="height: calc(100% - 1em)">
    <textarea v-model="logs" style="width: 100%; height: 100%" readonly></textarea>
  </div>
</template>

<script language="ts">
import axios from 'axios'
import { defineComponent, ref, onMounted, onBeforeUnmount } from 'vue'
import { useStore } from 'vuex'

export default defineComponent({
  setup () {
    const store = useStore()

    const update = ref(false)
    const logs = ref('')

    async function updateLogs () {
      try {
        const res = await axios.get('/api/logs')

        logs.value = res.data
      } catch (error) {
        console.error(error)
      }

      if (update.value) {
        setTimeout(updateLogs, 10000)
      }
    }

    onMounted(() => {
      store.commit('setTitle', 'Logs')

      update.value = true
      updateLogs()
    })

    onBeforeUnmount(() => {
      update.value = false
    })

    return {
      logs
    }
  }
})
</script>
