<script setup lang="ts">
const props = defineProps<{
  V: number[];
  I: number;
  // DT: number;
  // ST: number;
  PC: number;
  SP: number;
}>();
</script>

<template>
  <div>{{ props.V[0xf] }}</div>
</template>
