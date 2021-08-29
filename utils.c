#include "serial.h"
#include "utils.h"

void panic(const char* message, const char* file, u32int line) {
   __asm__ volatile("cli");
   serial_print(SERIAL_COM1_BASE, "PANIC(");
   serial_print(SERIAL_COM1_BASE, message);
   serial_print(SERIAL_COM1_BASE, ") at file ");
   serial_print(SERIAL_COM1_BASE, file);
   serial_print(SERIAL_COM1_BASE, " line ");
   serial_write_int(SERIAL_COM1_BASE, line);
   for (;;);
}
