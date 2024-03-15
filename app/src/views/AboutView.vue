<script setup lang="ts">
import { useVMStore } from '@/stores/vm';

const store = useVMStore();
</script>

<template>
  <div>
    <h1>About</h1>
    <p>Chip8 Virtual Machine that runs in a browser.</p>
  </div>
</template>

<style scoped>
h1 {
  margin-top: 1em;
  margin-bottom: 1em;
}
</style>
