CPU_FILES = $(wildcard src/cpu/*.c)
SOURCE = $(wildcard src/*.c)

CC = clang
INCLUDES = -I/opt/homebrew/include/

build/chip8: $(SOURCE) $(CPU_FILES)
	$(CC) -g $^ -o build/chip8 -Isrc/ $(INCLUDES) `sdl2-config --cflags --libs`

test: build/cpu.test

build/cpu.test: test/cpu.test.c $(CPU_FILES)
	$(CC) -g $^ -o build/cpu.test -Isrc/ $(INCLUDES) -L/opt/homebrew/Cellar/criterion/2.4.2/lib -lcriterion