
# Chip8 Virtual Machine

Chip8 Virtual Machine written in C with SDL2 library, compiled to WASM using Emscripten. Uses web-based user interface created in Typescript + Vue. Live version: <https://kcholew4.github.io/chip8/>

## What doesn't work
Majority of programs seem to work correctly, but some have weird problems, most likely caused by some of the opcodes not working correctly. (There are some ambiguities in how certain instructions behave between various Chip8 documentation sources available online.) Sound is currently not working, as I skipped its implementation.

## TODO
- [ ] Better opcodes test coverage.
- [ ] Working sound
- [ ] Live registers, stack and memory view
- [ ] Ability to change VM speed
- [ ] Compile target for standalone version

## Installation
Make sure you have `emsdk` and `sdl2-config` installed and available in your path.

```
git clone https://github.com/kcholew4/chip8.git
cd chip8
source ./wasm_build.sh
cd app
npm install
npm run build
```

## Usage

<https://kcholew4.github.io/chip8/>

When step execution in on, there is some debugging info (CPU state before and after each instruction) in browser's console.

![](https://raw.githubusercontent.com/kcholew4/chip8/main/gifs/breakout.gif)

## Sources
- <http://devernay.free.fr/hacks/chip8/C8TECH10.HTM>
- <https://github.com/mattmikolay/chip-8/wiki>
