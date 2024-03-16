<script setup lang="ts">
import { createModuleInstance, getExportedFunctions } from '@/services/chip8';
import { ref, onMounted, onUnmounted } from 'vue';

import ControlPanel from '@/components/ControlPanel.vue';
import { useVMStore } from '@/stores/vm';

const canvas = ref(null);
let wasmExported: null | ReturnType<typeof getExportedFunctions> = null;
const executable = ref<null | File>(null);

onMounted(async () => {
  await createModuleInstance(canvas.value!); // Maybe put that in a watcher
  wasmExported = getExportedFunctions();
});

const execute = async () => {
  if (!executable.value) {
    return console.log('nothing to execute');
  }

  if (!wasmExported) {
    return console.log('wasm module not initialized');
  }

  if (wasmExported.isRunning()) {
    wasmExported.emulationEnd();
  }

  const rom = new Uint8Array(await executable.value.arrayBuffer());
  wasmExported.initDevices();

  for (let i = 0; i < rom.length; i++) {
    wasmExported.memoryWriteByte(0x200 + i, rom[i]);
  }
  wasmExported.emulationStart();

  onUnmounted(() => {
    const store = useVMStore();
    store.ready = false; // Hack, change that
  });
};

const handleFileUpload = (event: Event) => {
  const target = event.target as HTMLInputElement;
  const files = target.files;

  if (!files) {
    return null;
  }

  executable.value = files[0];
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
      <ControlPanel />
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
@/chip8_module.js
