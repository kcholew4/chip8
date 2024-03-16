<script setup lang="ts">
import { Chip8 } from '@/services/chip8';
import { ref, onMounted, onUnmounted } from 'vue';

// import ControlPanel from '@/components/ControlPanel.vue';
import { useVMStore } from '@/stores/vm';

const canvas = ref(null);
const executable = ref<null | File>(null);

const execute = async () => {
  if (!executable.value) {
    return console.log('nothing to execute');
  }

  if (!canvas.value) {
    return console.log(`canvas doesn't exist`);
  }

  const instance = await Chip8.createInstance(canvas.value);

  const rom = new Uint8Array(await executable.value.arrayBuffer());

  for (let i = 0; i < rom.length; i++) {
    instance.memoryWriteByte(0x200 + i, rom[i]);
  }

  setInterval(() => instance.oneIter(), 1000 / 60);
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
      <!-- <ControlPanel /> -->
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
