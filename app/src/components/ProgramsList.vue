<script setup lang="ts">
const emit = defineEmits<{
  (e: 'select', executable: File): void;
}>();

const roms = [
  {
    executable: 'INVADERS',
    title: 'Space Invaders',
    description: `Destroy the invaders with your ship. Shoot
                  with 5, move with 4 and 6. Press 5 to begin a game.`
  },
  {
    executable: 'BRIX',
    title: 'Brix',
    description: `You have 5 lives, and your goal is the destruction of all the brixs.
                  Use 4 and 6 to move your paddle. The game ends when all the brixs are destroyed.`
  },
  {
    executable: 'TETRIS',
    title: 'Tetris',
    description: `The 4 key is left rotate, 5 - left move, 6 - right move, 1
                  - drop, ENTER - restart, DROP - end.  After every 5 lines, the speed
                  increases slightly and peaks at 45 lines.`
  },
  {
    executable: 'MAZE',
    title: 'Maze',
    description: `Draws a random maze.`
  },
  {
    executable: 'MISSILE',
    title: 'Missile',
    description: `Use 8 to shoot, with each shot the game gets faster.`
  }
];

const fetchRom = (name: string) => fetch(`/chip8/games/${name}`); // TODO: Use dynamic path

const handleProgramClick = async (id: number) => {
  const { executable } = roms[id];
  const rom = await fetchRom(executable);

  if (!rom.ok) {
    throw new Error(`Error fetching: ${executable}`);
  }

  const file = new File([await rom.blob()], executable);
  emit('select', file);
};
</script>

<template>
  <div class="programs">
    <!-- <h2>Programs</h2> -->
    <div class="programs__container">
      <div
        v-for="(rom, index) in roms"
        :key="index"
        class="program"
        @click="handleProgramClick(index)"
      >
        <div class="program__title">{{ rom.title }}</div>
        <div class="program__description">{{ rom.description.trim() }}</div>
      </div>
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
  height: 500px;

  @media (max-width: 1400px) {
    width: auto;
    height: auto;
    max-height: 400px;
    max-width: 800px;
  }
}

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
