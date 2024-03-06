#!/bin/bash
if [ ! -d "app/build" ]; then
  mkdir app/build
fi

if [ -n "$1" ]; then
  "$1" src/*.c -o app/build/chip8.js -sUSE_SDL=2 -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
else
  emcc src/*.c -o app/build/chip8.js -sUSE_SDL=2 -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
fi