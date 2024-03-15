import { defineStore } from 'pinia';
import { ref } from 'vue';

export const useVMStore = defineStore('vm', () => {
  const stepExecution = ref(false);

  const toggleStepExecution = () => (stepExecution.value = !stepExecution.value);

  return { stepExecution, toggleStepExecution };
});
