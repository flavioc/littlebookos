#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "serial.h"

void kmain()
{
   gdt_init();
   idt_init();

   serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
   serial_configure_line(SERIAL_COM1_BASE);
   serial_print(SERIAL_COM1_BASE, "Hello from the little book about OS development\nThis is the serial port\n");

   fb_enable_cursor();
   fb_clear();
   fb_write_text("Hello from the little book about OS development\nThis is the framebuffer\n> ");

   __asm__ volatile ("int $0x01");
   __asm__ volatile ("int $0x00");
   __asm__ volatile ("sti");
   register_interrupt_handler(KEYBOARD_INTERRUPT, keyboard_handler);
}
