<script setup lang="ts">
import { onMounted, ref, watch } from 'vue';
import { useVMStore } from '@/stores/vm';

const store = useVMStore();

const canvas = ref<HTMLCanvasElement>();
const video = ref<HTMLVideoElement>();

watch(
  () => store.active,
  () => {
    if (store.active) {
      const stream = canvas.value!.captureStream();
      video.value!.srcObject = stream;
    }
  }
);

const emit = defineEmits<{
  (e: 'ready', canvas: HTMLCanvasElement): void;
}>();

onMounted(() => {
  if (!video.value || !canvas.value) {
    throw new Error('Something went terribly wrong');
  }

  emit('ready', canvas.value!);
});

const handlePictureInPicture = async () => {
  if (!document.pictureInPictureEnabled) {
    return console.log('Picture in picture is not allowed');
  }

  if (document.pictureInPictureElement) {
    return document.exitPictureInPicture();
  }

  await video.value!.requestPictureInPicture(); // idk why it's not working on safari
};
</script>

<template>
  <div class="video-container">
    <div class="button-container">
      <button :disabled="!store.active" @click="handlePictureInPicture">Picture in Picture</button>
    </div>
    <video ref="video" autoplay></video>
  </div>
  <canvas ref="canvas" id="canvas"></canvas>
</template>

<style scoped lang="scss">
#canvas {
  display: none;
}

.video-container {
  .button-container {
    margin-bottom: 0.5em;
    display: flex;
    justify-content: end;

    button {
      border: none;
      background-color: transparent;
      padding: 0;
      font-size: 0.75em;
      color: #deb557;

      &:hover,
      &:focus {
        text-decoration: underline;
        cursor: pointer;
      }

      &:disabled {
        color: #525252;
        &:hover,
        &:focus {
          text-decoration: none;
          cursor: inherit;
        }
      }
    }
  }

  video {
    box-sizing: content-box;
    width: 640px;
    height: 320px;
    border: 2px solid #1d191e;
    background-color: black;
  }
}
</style>
