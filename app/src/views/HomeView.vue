<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, watch } from 'vue';
import { useVMStore } from '@/stores/vm';
import ControlPanel from '@/components/ControlPanel.vue';

const store = useVMStore();

let interval: ReturnType<typeof window.setInterval>;
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
  // @ts-ignore - Type 'number' is not assignable to type 'Timeout'
  interval = window.setInterval(() => instance.oneIter(), 1000 / store.speed);

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
      // @ts-ignore
      interval = window.setInterval(() => instance.oneIter(), 1000 / speed);
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
    <div class="header">
      <h1>Chip8 Virtual Machine</h1>
      <p>
        Classic Chip8 VM that runs in the browser. Select program from the list or upload from a
        file to start.
      </p>
    </div>
    <div class="display">
      <canvas ref="canvas" id="canvas"></canvas>
    </div>
    <div class="container">
      <input type="file" @change="handleFileUpload" />
      <button @click="execute()">Execute</button>
    </div>
    <div class="controls">
      <ControlPanel @step="handleStepClick"></ControlPanel>
    </div>
  </main>
</template>

<style lang="scss" scoped>
.container {
  max-width: 640px;
  margin-left: auto;
  margin-right: auto;
}

.display {
  display: flex;
  justify-content: center;
  padding: 50px 0 20px;
}

.display canvas {
  width: 640px;
  height: 320px;
  background-color: black;
  border: 1px solid #1d191e;
}

h1 {
  font-family: 'Pixelify Sans', sans-serif;
  font-style: normal;
  font-weight: 700;
  margin-bottom: 0.5em;
}

.header {
  margin-top: 50px;

  p {
    font-family: 'Noto Sans Mono', monospace;
    font-weight: 400;
    max-width: 600px;
  }
}

.controls {
  margin-top: 50px;
}
</style>
