SOURCE = $(wildcard src/*.c)

CC = clang
INCLUDES = -I/opt/homebrew/include/

build/chip8: $(SOURCE)
	$(CC) -g $^ -o build/chip8 -Isrc/ $(INCLUDES) `sdl2-config --cflags --libs`

test: build/cpu.test build/display.test

build/cpu.test: test/cpu.test.c $(SOURCE)
	$(CC) -D TEST_ENV -g $^ -o build/cpu.test -Isrc/ $(INCLUDES) -L/opt/homebrew/Cellar/criterion/2.4.2/lib -lcriterion `sdl2-config --cflags --libs`

build/display.test: test/display.test.c $(SOURCE)
	$(CC) -D TEST_ENV -g $^ -o build/display.test -Isrc/ $(INCLUDES) -L/opt/homebrew/Cellar/criterion/2.4.2/lib -lcriterion `sdl2-config --cflags --libs`