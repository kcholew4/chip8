<script setup lang="ts">
const props = defineProps<{
  symbol: string;
  virtual_key: string;
}>();
</script>

<template>
  <div class="key">
    {{ props.symbol.toUpperCase() }}
  </div>
</template>

<style lang="scss" scoped>
.key {
  font-size: 0.75rem;
  display: inline-block;
  padding: 0.1em 0.25em;
  background-color: rgb(33, 33, 33);
  border: 1px solid rgb(52, 52, 52);
  border-radius: 4px;
  line-height: 1.2;
}
</style>
