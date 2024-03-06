#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void emulation_init(char executable[]);

void init_devices();
void emulation_start();
void emulation_end();

extern bool isRunning;