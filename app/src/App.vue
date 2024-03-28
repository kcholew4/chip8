<script setup lang="ts">
import { RouterLink, RouterView } from 'vue-router';
</script>

<template>
  <header>
    <div class="container">
      <nav>
        <RouterLink to="/" activeClass="active-route">#Home</RouterLink>
        <RouterLink to="/about" activeClass="active-route">#About</RouterLink>
      </nav>
    </div>
  </header>

  <div class="content">
    <RouterView />
  </div>
</template>

<style scoped lang="scss">
nav > * + * {
  margin-left: 10px;
}

nav {
  padding: 2em;
  background-color: black;

  a {
    font-family: 'Noto Sans Mono', monospace;
    font-size: 1rem;
    color: inherit;
  }

  .active-route {
    // color: #fffe88;
    color: white;
    font-weight: 600;
  }
}

.content {
  padding: 2em;
  padding-left: 2em;
  padding-right: 2em;
}
</style>
