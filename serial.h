#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"

// Serial devices.
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_COM2_BASE 0x2F8

// Serial registers, parameterized by the number.
#define SERIAL_DATA_PORT(base) (base)
// To set the significant byte for divisor value.
#define SERIAL_DATA_PORT1(base) (base + 1)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

void serial_configure_baud_rate(u16int com, u16int divisor);
void serial_configure_line(u16int com);
void serial_write(u16int com, char a);
void serial_write_int(u16int com, int a);
void serial_write_hexa(u16int com, int a);
void serial_print(u16int com, const char* str);

#endif
