<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, watch } from 'vue';
import { useVMStore } from '@/stores/vm';
import ControlPanel from '@/components/ControlPanel.vue';
import ProgramsList from '@/components/ProgramsList.vue';

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

const handleProgramSelect = (file: File) => {
  executable.value = file;
  execute();
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
    <div class="main-section">
      <div class="display">
        <canvas ref="canvas" id="canvas"></canvas>
        <div class="upload-program">
          <input type="file" @change="handleFileUpload" />
          <button @click="execute()">Execute</button>
        </div>
      </div>
      <div class="programs-list">
        <h2>Programs</h2>
        <p>Some public domain roms.</p>
        <ProgramsList @select="handleProgramSelect"></ProgramsList>
      </div>
    </div>

    <div class="controls">
      <ControlPanel @step="handleStepClick"></ControlPanel>
    </div>
  </main>
</template>

<style lang="scss" scoped>
.programs-list {
  h2 {
    margin-bottom: 0.5em;
  }

  p {
    margin-bottom: 1.5rem;
  }
}

.container {
  max-width: 640px;
  margin-left: auto;
  margin-right: auto;
}

.main-section {
  display: flex;
  justify-content: center;
  gap: 5rem;
  padding: 60px 0 60px;
  align-items: center;
}

.display {
  padding: 20px 0 20px;
  canvas {
    width: 640px;
    height: 320px;
    background-color: black;
    border: 2px solid #1d191e;
  }

  .upload-program {
    margin-top: 20px;
    position: absolute;
  }
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
  margin-top: 40px;
}

h1 {
  margin-bottom: 0.5em;
}

p {
  color: #b8b8b8;
}
</style>
