<script setup lang="ts">
import { useVMStore } from '@/stores/vm';

const store = useVMStore();
</script>

<template>
  <div class="controls">
    <div class="speed">
      <label for="fps">VM Speed [FPS]:</label>
      <div class="speed-slider">
        <input type="range" min="1" max="240" v-model="store.speed" />
        <div>{{ store.speed }}</div>
      </div>
    </div>
    <div class="checkbox">
      <div class="info">
        Execution: <strong>{{ store.stepExecution ? `step` : `normal` }}</strong>
      </div>
      <div class="toggle">
        <input type="checkbox" id="step-execution" v-model="store.stepExecution" />
        <label for="step-execution">Step execution</label>
      </div>
    </div>
    <button v-if="store.stepExecution" @click="$emit('step')">Step</button>
  </div>
</template>

<style scoped lang="scss">
.checkbox {
  .info {
    margin-bottom: 0.25em;
  }

  .toggle {
    display: flex;
    align-items: center;
    gap: 0.25em;
  }
}

#fps {
  width: 4em;
}

.speed {
  label {
    display: block;
    margin-bottom: 0.25em;
  }
}

.speed-slider {
  display: flex;
  align-items: center;
  gap: 0.25em;
}

.controls {
  margin-top: 2em;
  display: flex;
  gap: 1em;
  align-items: center;
}
</style>
