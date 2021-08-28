#include "fb.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "serial.h"
#include "timer.h"

struct multiboot;

void kmain(struct multiboot *mboot_ptr)
{
   (void)mboot_ptr;
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

   // Initialize timer to 50Hz, which is 50 cycles per second (20ms).
   timer_init(50);
}
