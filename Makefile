CPU_FILES = $(wildcard src/cpu/*.c)

cpu_test: test/cpu.test.c $(CPU_FILES)
	clang -g $^ -o cpu_test -Isrc/