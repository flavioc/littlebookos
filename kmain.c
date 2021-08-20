#include "fb.h"
#include "gdt.h"
#include "serial.h"

void kmain()
{
	gdt_init();

	serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_print(SERIAL_COM1_BASE, "Hello from the little book about OS development\nThis is the serial port\n");

	fb_enable_cursor();
	fb_clear();
	FbContext ctx;
	fb_init_context(&ctx);
	fb_write_text("Hello from the little book about OS development\nThis is the framebuffer", &ctx);
}
