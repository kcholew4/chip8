<script setup lang="ts">
// @ts-ignore - for now...
import Module from "@/chip8.js";
import { ref, onMounted } from "vue";

const canvas = ref(null);
let wasmExported: null | {[key: string]: Function} = null;
const executable = ref<null | File>(null);

onMounted(async () => {
  const module = await Module({
    locateFile: (path: string, prefix: string) => {
      const url = new URL(prefix);
      return url.origin + "/chip8/" + path;
    },
    canvas: canvas.value,
    print: (...args: string[]) => console.log(args.join(" "))
  })

  if (module.value === null) {
    return;
  }

  wasmExported = {
    init_devices: module.cwrap("wasm_init_devices", "null", "null"),
    memory_write_byte: module.cwrap("wasm_memory_write", "null", ["number", "number"]),
    emulation_start: module.cwrap("wasm_emulation_start", "null", "null"),
    emulation_end: module.cwrap("wasm_emulation_end", "null", "null"),
    is_running: module.cwrap("wasm_is_running", "null", "number")
  }
});

const execute = async () => {
  if (!executable.value) {
    return console.log("nothing to execute");
  }

  if (!wasmExported) {
    return console.log("wasm module not initialized");
  }

  if (wasmExported.is_running()) {
    wasmExported.emulation_end();
  }

  const rom = new Uint8Array(await executable.value.arrayBuffer());
  wasmExported.init_devices();

  for (let i = 0; i < rom.length; i++) {
    wasmExported.memory_write_byte(0x200 + i, rom[i]);
  }
  wasmExported.emulation_start();
}

const handleFileUpload = (event: Event) => {
  const target = event.target as HTMLInputElement;
  const files = target.files;

  if (!files) {
    return null;
  }

  executable.value = files[0];
}

</script>

<template>
  <main>
    <div class="display">
      <canvas ref="canvas" id="canvas"></canvas>
    </div>
    <div class="container">
      <input type="file" @change="handleFileUpload" />
      <button @click="execute();">Execute</button>
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