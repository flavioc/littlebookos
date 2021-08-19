#include "io.h"
#include "serial.h"

void
serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
#define SERIAL_LINE_ENABLE_DLAB 0x80
   outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
   outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
   outb(SERIAL_DATA_PORT1(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com) {
   // Length of 8, no parity bit, one stop bit and break control
   // disabled.
   outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
   // Enable FIFO, clear both receiver and transmission FIFO queues and
   // use 14 bytes as size of queue.
   outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
   outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(unsigned int com) {
   return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write(unsigned short com, char a) {
   while (serial_is_transmit_fifo_empty(com) == 0) {
   }

   outb(SERIAL_DATA_PORT(com), a);
}

void serial_print(unsigned short com, const char* str) {
   for (int i = 0; *(str + i); ++i) {
      serial_write(com, *(str + i));
   }
}
