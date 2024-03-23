<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, watch, nextTick, onUnmounted } from 'vue';
import { useVMStore } from '@/stores/vm';
import ControlPanel from '@/components/ControlPanel.vue';
import ProgramsList from '@/components/ProgramsList.vue';
import VirtualDisplay from '@/components/VirtualDisplay.vue';
import VritualKeybaord from '@/components/VirtualKeyboard.vue';

const store = useVMStore();

onUnmounted(() => {
  store.mainLoopDestroy?.();
});

store.active = false;

const canvas = ref<HTMLCanvasElement>();
const executable = ref<null | File>(null);

const execute = async () => {
  store.active = true;
  await nextTick();

  if (store.instance !== undefined) {
    store.mainLoopDestroy!();
  }

  if (!executable.value) {
    return console.log('nothing to execute');
  }

  if (!canvas.value) {
    return console.log(`canvas doesn't exist`);
  }

  store.instance = await Chip8.createInstance(canvas.value);
  await store.instance.loadProgram(executable.value);
  store.mainLoopDestroy = store.instance.createMainLoop();

  if (store.stepExecution) {
    store.instance.stepExecution();
  }
};

watch(
  () => store.stepExecution,
  (step) => {
    if (store.instance !== undefined) {
      step ? store.instance.stepExecution() : store.instance.normalExecution();
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
  if (store.instance === undefined) {
    return;
  }

  store.instance.cpuStep();
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
      <div class="programs-list">
        <h2>Programs</h2>
        <p>Some public domain roms.</p>
        <ProgramsList @select="handleProgramSelect"></ProgramsList>
      </div>
      <div class="display">
        <VirtualDisplay @ready="(_canvas) => (canvas = _canvas)"></VirtualDisplay>
        <div class="upload-program">
          <input type="file" @change="handleFileUpload" />
          <button @click="execute()">Execute</button>
        </div>
      </div>
    </div>
    <div v-if="store.showKeyboard" class="keyboard-container">
      <VritualKeybaord></VritualKeybaord>
    </div>
    <div class="controls">
      <ControlPanel @step="handleStepClick"></ControlPanel>
    </div>
  </main>
</template>

<style lang="scss" scoped>
.keyboard-container {
  // margin-top: 80px;
  max-width: 1120px;
  margin-left: auto;
  margin-right: auto;
  // border: 2px solid #1d191e;
  display: flex;
  justify-content: center;
  padding: 20px 0;
  background-color: #090d0f;
  border-radius: 8px;
}

.upload-program {
  input {
    margin-right: 0.5em;
  }
}

.main-section {
  max-width: 1120px;
  margin-left: auto;
  margin-right: auto;
  display: flex;
  flex-direction: row-reverse;
  justify-content: space-between;
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
    flex-basis: auto;
    max-width: 640px;
    margin: auto;
    margin-bottom: 2.5rem;
    margin-top: 3rem;
  }
  .upload-program {
    margin-top: 10px;
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
