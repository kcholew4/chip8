#!/bin/bash
EMCC=emcc

if [ -n "$1" ]; then
  EMCC=$1
fi

if ! [ -d ./tmp ]; then
  mkdir tmp
fi

"$EMCC" src/*.c -o tmp/chip8.js -sMODULARIZE -sEXPORT_ES6 -sUSE_SDL=2 -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

cp tmp/chip8.wasm app/public
cp tmp/chip8.js app/src

rm -rf tmp/