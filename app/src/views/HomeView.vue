<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, watch, nextTick } from 'vue';
import { useVMStore } from '@/stores/vm';
import ControlPanel from '@/components/ControlPanel.vue';
import ProgramsList from '@/components/ProgramsList.vue';
import VirtualDisplay from '@/components/VirtualDisplay.vue';

const store = useVMStore();

let mainLoopDestroy: () => void;
let instance: Chip8;

const canvas = ref<HTMLCanvasElement>();
const executable = ref<null | File>(null);

const execute = async () => {
  store.active = true;
  await nextTick();

  if (instance !== undefined) {
    mainLoopDestroy();
  }

  if (!executable.value) {
    return console.log('nothing to execute');
  }

  if (!canvas.value) {
    return console.log(`canvas doesn't exist`);
  }

  instance = await Chip8.createInstance(canvas.value);
  await instance.loadProgram(executable.value);
  mainLoopDestroy = instance.createMainLoop();

  if (store.stepExecution) {
    instance.stepExecution();
  }
};

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
        <VirtualDisplay @ready="(_canvas) => (canvas = _canvas)"></VirtualDisplay>
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
.upload-program {
  input {
    margin-right: 0.5em;
  }
}

.main-section {
  display: flex;
  justify-content: center;
  gap: 5rem;
  padding: 60px 0 60px;
  align-items: center;

  @media (max-width: 1400px) {
    display: block;
  }
}

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

.display {
  padding: 20px 0 20px;
  flex-basis: 640px;

  @media (max-width: 1400px) {
    max-width: 640px;
    margin: auto;
    margin-bottom: 7rem;
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
