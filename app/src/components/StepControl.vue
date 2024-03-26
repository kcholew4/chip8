<script setup lang="ts">
import { useVMStore } from '@/stores/vm';

const store = useVMStore();

const handleStepClick = () => {
  if (store.instance === undefined) {
    return;
  }

  store.instance.cpuStep();
};
</script>

<template>
  <div class="step-control">
    <div class="checkbox">
      <div class="info">
        Execution: <strong>{{ store.stepExecution ? `step` : `normal` }}</strong>
      </div>
      <div class="toggle">
        <input type="checkbox" id="step-execution" v-model="store.stepExecution" />
        <label for="step-execution">Step execution</label>
      </div>
    </div>
    <button v-if="store.stepExecution" @click="handleStepClick">Step</button>
  </div>
</template>

<style scoped lang="scss">
.step-control {
  display: flex;
  gap: 1em;
  align-items: center;

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
}
</style>
