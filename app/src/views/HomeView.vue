<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, watch } from 'vue';
import { useVMStore } from '@/stores/vm';
import ControlPanel from '@/components/ControlPanel.vue';

const store = useVMStore();

let interval: number;
let instance: Chip8;

const canvas = ref(null);
const executable = ref<null | File>(null);

const execute = async () => {
  if (instance !== undefined) {
    clearInterval(interval);
  }

  if (!executable.value) {
    return console.log('nothing to execute');
  }

  if (!canvas.value) {
    return console.log(`canvas doesn't exist`);
  }

  instance = await Chip8.createInstance(canvas.value);
  await instance.loadProgram(executable.value);
  interval = setInterval(() => instance.oneIter(), 1000 / store.speed);

  if (store.stepExecution) {
    instance.stepExecution();
  }
};

watch(
  () => store.speed,
  (speed) => {
    if (instance !== undefined) {
      console.log(speed);
      clearInterval(interval);
      interval = setInterval(() => instance.oneIter(), 1000 / speed);
    }
  }
);

watch(
  () => store.stepExecution,
  (step) => {
    if (instance !== undefined) {
      step ? instance.stepExecution() : instance.normalExecution();
    }
  }
);

const handleFileUpload = (event: Event) => {
  const target = event.target as HTMLInputElement;
  const files = target.files;

  if (!files) {
    return null;
  }

  executable.value = files[0];
};

const handleStepClick = () => {
  if (instance === undefined) {
    return;
  }

  instance.cpuStep();
};
</script>

<template>
  <main>
    <div class="display">
      <canvas ref="canvas" id="canvas"></canvas>
    </div>
    <div class="container">
      <input type="file" @change="handleFileUpload" />
      <button @click="execute()">Execute</button>
    </div>
    <div>
      <ControlPanel @step="handleStepClick"></ControlPanel>
    </div>
  </main>
</template>

<style>
.container {
  max-width: 640px;
  margin-left: auto;
  margin-right: auto;
}

.display {
  display: flex;
  justify-content: center;
  padding: 50px 0 50px;
}

.display canvas {
  width: 640px;
  height: 320px;
  background-color: black;
}
</style>
