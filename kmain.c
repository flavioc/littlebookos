#include "fb.h"

void kmain()
{
	fb_enable_cursor();
	fb_clear();
	FbContext ctx;
	fb_init_context(&ctx);
	fb_write_text("Hello from the little book about OS development\nThis is the framebuffer", &ctx);
}
