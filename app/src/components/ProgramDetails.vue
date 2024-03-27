<script setup lang="ts">
const props = defineProps<{
  title: string;
  name: string;
}>();

defineEmits<{
  click: [name: string];
}>();
</script>

<template>
  <div class="program" @click="$emit('click', props.name)">
    <div class="program__title">{{ props.title }}</div>
    <div class="program__description">
      <slot></slot>
    </div>
  </div>
</template>

<style lang="scss" scoped>
.program {
  &:hover,
  &:focus {
    background-color: #131313;
    cursor: pointer;
  }

  &:hover &__title,
  &:focus &__title {
    text-decoration: underline;
  }

  // font-size: 0.8rem;
  padding: 20px 25px;

  &__title {
    font-weight: 700;
    margin-bottom: 0.5em;
    color: #56b6de;
    // text-decoration: underline;
  }

  &__description {
    font-weight: 100;
    font-size: 0.8em;
  }
}
</style>
