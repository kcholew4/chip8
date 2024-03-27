<script setup lang="ts">
import ProgramDetails from './ProgramDetails.vue';
import InlineKey from './InlineKey.vue';

const emit = defineEmits<{
  (e: 'select', executable: File): void;
}>();

const romsCache: Map<string, File> = new Map(); // Maybe move this to global state?

const fetchRom = (name: string) => fetch(`${import.meta.env.BASE_URL}/games/${name}`);

const handleProgramClick = async (name: string) => {
  if (romsCache.has(name)) {
    return emit('select', romsCache.get(name)!);
  }

  const rom = await fetchRom(name);

  if (!rom.ok) {
    throw new Error(`Error fetching: ${name}`);
  }

  const file = new File([await rom.blob()], name);
  romsCache.set(name, file);
  emit('select', file);
};
</script>

<template>
  <div class="programs">
    <!-- <h2>Programs</h2> -->
    <div class="programs__container">
      <ProgramDetails name="INVADERS" title="Space Invaders" @click="handleProgramClick">
        Destroy the invaders with your ship. Shoot with <InlineKey virtual_key="5" symbol="w" />,
        move with <InlineKey virtual_key="4" symbol="q" /> and
        <InlineKey virtual_key="6" symbol="e" />. Press <InlineKey virtual_key="5" symbol="w" /> to
        begin a game.
      </ProgramDetails>
      <ProgramDetails name="BRIX" title="Brix" @click="handleProgramClick">
        You have 5 lives, and your goal is the destruction of all the brixs. Use
        <InlineKey virtual_key="4" symbol="q" /> and <InlineKey virtual_key="6" symbol="e" /> to
        move your paddle. The game ends when all the brixs are destroyed.
      </ProgramDetails>
      <ProgramDetails name="PONG2" title="Pong" @click="handleProgramClick">
        Use keys <InlineKey virtual_key="1" symbol="1" /> and
        <InlineKey virtual_key="4" symbol="q" /> move left player and
        <InlineKey virtual_key="c" symbol="4" /> and <InlineKey virtual_key="d" symbol="r" /> move
        right player.
      </ProgramDetails>
      <!-- Not working as expected :D-->
      <!-- <ProgramDetails name="TETRIS" title="Tetris" @click="handleProgramClick">
        The 4 key is left rotate, 5 - left move, 6 - right move, 1 - drop, ENTER - restart, DROP -
        end. After every 5 lines, the speed increases slightly and peaks at 45 lines.
      </ProgramDetails> -->
      <ProgramDetails name="MAZE" title="Maze" @click="handleProgramClick">
        Draws a random maze.
      </ProgramDetails>
      <ProgramDetails name="MISSILE" title="Missile" @click="handleProgramClick">
        Use <InlineKey virtual_key="8" symbol="s" /> to shoot, with each shot the game gets faster.
      </ProgramDetails>
    </div>
  </div>
</template>

<style lang="scss" scoped>
.programs {
  overflow: auto;
  scrollbar-color: #232323 #100c10;
  border: 2px solid #1d191e;
  // padding: 25px;
  width: 400px;
  min-height: 400;
  max-height: 480px;

  @media (max-width: 1400px) {
    width: auto;
    height: auto;
    max-height: 400px;
    max-width: 800px;
  }
}
</style>
