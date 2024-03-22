<script setup lang="ts">
import { onMounted } from 'vue';

const keys = new Map<number, HTMLButtonElement>();

onMounted(() => {
  keys.forEach((value) => {
    // console.log(value);
  });
});

const formatKeyNumber = (number: number) => {
  return number.toString(16).toUpperCase();
};

const keyGridAlignment = (number: number) => {
  const gridPosition = (c1: number, c2: number, r1: number, r2: number) => ({
    gridColumn: `${c1} / ${c2}`,
    gridRow: `${r1} / ${r2}`
  });

  if (number >= 1 && number <= 9) {
    const row = Math.floor((number - 1) / 3) + 1;
    const column = ((number - 1) % 3) + 1;

    return gridPosition(column, column + 1, row, row + 1);
  }

  switch (number) {
    case 0xa:
      return gridPosition(1, 2, 4, 5);
    case 0x0:
      return gridPosition(2, 3, 4, 5);
    case 0xb:
      return gridPosition(3, 4, 4, 5);
    case 0xf:
      return gridPosition(4, 5, 4, 5);
    // Rest of the keys fall nicely in place
  }
};

const handleClick = (number: number) => {
  console.log(number);
  switch (number) {
    case 4:
      return window.dispatchEvent(new KeyboardEvent('keydown', { key: 'q' }));
    case 6:
      return window.dispatchEvent(new KeyboardEvent('keypress', { key: 'e' }));
  }
};
</script>
<template>
  <div class="keyboard">
    <button
      v-for="(_, index) in 0x10"
      :key="index"
      :style="keyGridAlignment(index)"
      :ref="(el) => keys.set(index, el as HTMLButtonElement)"
      @click="handleClick(index)"
    >
      {{ formatKeyNumber(index) }}
    </button>
  </div>
</template>
<style scoped lang="scss">
.keyboard {
  display: grid;
  grid-template-columns: repeat(4, 50px);
  grid-template-rows: repeat(4, 50px);
  gap: 0.25em;
}

button {
  border: 1px solid rgb(95, 95, 95);
  // font-size: 1.25em;
}
</style>
