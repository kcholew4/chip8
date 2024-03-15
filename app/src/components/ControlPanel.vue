<script setup lang="ts">
import { useVMStore } from '@/stores/vm';
import RegistersMonitor from './RegistersMonitor.vue';

const store = useVMStore();
</script>

<template>
  <div>
    Execution: <strong>{{ store.stepExecution ? `step` : `normal` }}</strong>
  </div>
  <div>
    <div class="checkbox">
      <input type="checkbox" id="step-execution" v-model="store.stepExecution" />
      <label for="step-execution">Step execution</label>
    </div>
  </div>
  <!-- <RegistersMonitor></RegistersMonitor> -->
</template>

<style>
.checkbox {
  display: flex;
  align-content: center;
  gap: 0.25em;
  /* padding: 1em; */
}
</style>
