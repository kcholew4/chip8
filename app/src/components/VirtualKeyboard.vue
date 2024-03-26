<script setup lang="ts">
import { onMounted, ref } from 'vue';
import { useVMStore } from '@/stores/vm';

const store = useVMStore();

const keys = new Map<number, HTMLButtonElement>();

const keysMapping = new Map([
  [0, 120],
  [1, 49],
  [2, 50],
  [3, 51],
  [4, 113],
  [5, 119],
  [6, 101],
  [7, 97],
  [8, 115],
  [9, 100],
  [0xa, 122],
  [0xb, 99],
  [0xc, 52],
  [0xd, 114],
  [0xe, 102],
  [0xf, 118]
]);

const getVirtualKeyboardCode = (code: number) => {
  for (const [key, value] of keysMapping.entries()) {
    if (value === code) {
      return key;
    }
  }
};

const keysPressedState = ref([...keysMapping.keys()].map(() => false));

onMounted(() => {
  document.addEventListener('keydown', (e) => {
    const code = e.key.charCodeAt(0);
    const virtualCode = getVirtualKeyboardCode(code);

    if (virtualCode === undefined) {
      return;
    }

    keysPressedState.value[virtualCode] = true;
  });

  document.addEventListener('keyup', (e) => {
    const code = e.key.charCodeAt(0);
    const virtualCode = getVirtualKeyboardCode(code);

    if (virtualCode === undefined) {
      return;
    }

    keysPressedState.value[virtualCode] = false;
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

const handleClick = (number: number, type: 'up' | 'down') => {
  const key = keysMapping.get(number);

  if (!key) {
    return;
  }

  if (type === 'down') {
    return store.instance?.keyEvent(key, 'down');
  }

  store.instance?.keyEvent(key, 'up');
};
</script>
<template>
  <div class="keyboard">
    <button
      v-for="(_, index) in 0x10"
      :key="index"
      :style="keyGridAlignment(index)"
      :ref="(el) => keys.set(index, el as HTMLButtonElement)"
      @mousedown="handleClick(index, 'down')"
      @mouseup="handleClick(index, 'up')"
      @touchstart="handleClick(index, 'down')"
      @touchend="handleClick(index, 'up')"
      :class="{ pressed: keysPressedState[index] }"
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
  touch-action: manipulation;
  -webkit-user-select: none;
  user-select: none;
}

button {
  border: 1px solid rgb(95, 95, 95);

  &:focus,
  &:hover {
    background-color: #161116;
  }

  &:active {
    background-color: #2d252d;
  }

  &.pressed {
    background-color: #2d252d;
  }
}
</style>
