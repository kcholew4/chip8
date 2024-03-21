<script setup lang="ts">
import { useVMStore } from '@/stores/vm';

const store = useVMStore();

const pc_keyboard = `
-----------------<br/>
| 1 | 2 | 3 | 4 |<br/>
-----------------<br/>
| Q | W | E | R |<br/>
-----------------<br/>
| A | S | D | F |<br/>
-----------------<br/>
| Z | X | C | V |<br/>
-----------------<br/>
`;

const chip8_keyboard = `
-----------------<br/>
| 1 | 2 | 3 | C |<br/>
-----------------<br/>
| 4 | 5 | 6 | D |<br/>
-----------------<br/>
| 7 | 8 | 9 | E |<br/>
-----------------<br/>
| A | 0 | B | F |<br/>
-----------------<br/>
`;
</script>

<template>
  <div>
    <h1>About</h1>
    <p>Chip8 Virtual Machine that runs in a browser.</p>
    <h1>Keyboard layout</h1>
    <div class="keyboard-layout">
      <div v-html="pc_keyboard"></div>
      <div>--&gt;</div>
      <div v-html="chip8_keyboard"></div>
    </div>
  </div>
</template>

<style scoped>
h1 {
  margin-top: 1em;
  margin-bottom: 1em;
}

.keyboard-layout {
  display: flex;
  align-items: center;
  gap: 5rem;
}
</style>
