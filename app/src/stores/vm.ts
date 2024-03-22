import { defineStore } from 'pinia';
import { ref } from 'vue';

export const useVMStore = defineStore('vm', () => {
  const stepExecution = ref(false);
  const speed = ref(60);

  const active = ref(false);
  const showKeyboard = ref(false);

  const toggleStepExecution = () => (stepExecution.value = !stepExecution.value);

  return { stepExecution, speed, toggleStepExecution, active, showKeyboard };
});
