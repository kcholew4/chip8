<script setup lang="ts">
import { useVMStore } from '@/stores/vm';
import { getExportedFunctions } from '@/services/chip8';
import { watch, ref } from 'vue';
import RegistersMonitor from './RegistersMonitor.vue';

const store = useVMStore();

const v_registers = ref<number[]>(new Array(0xf).map(() => 0));

const updateRegisters = () => {
  const { getV } = getExportedFunctions();
  for (let i = 0; i < 0x10; i++) {
    v_registers.value[i] = getV(i);
  }
};

if (!store.ready) {
  watch(() => store.ready, updateRegisters, { once: true });
}
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
  <div class="registers">
    <template v-if="store.ready">
      <div v-for="index in 0xf + 1" :key="index">
        {{ index - 1 }}: {{ v_registers[index - 1].toString(16).padStart(2, '0') }}
      </div>
    </template>
    <button @click="updateRegisters" style="margin-top: 0.5em">Refresh</button>
  </div>
  <!-- <RegistersMonitor></RegistersMonitor> -->
</template>

<style scoped>
.checkbox {
  display: flex;
  align-content: center;
  gap: 0.25em;
  /* padding: 1em; */
}

.registers {
  margin-top: 2em;
}
</style>
