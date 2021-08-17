#ifndef FB_H
#define FB_H

#define FB_BLACK 0
#define FB_BLUE 1
#define FB_GREEN 2
#define FB_CYAN 3
#define FB_RED 4
#define FB_DARK_GREY 8
#define FB_WHITE 15

typedef struct _FbContext {
   unsigned char row, col;
} FbContext;

void fb_init_context(FbContext* fb_context);
void fb_write_text(const char* str, FbContext* fb_context);

void fb_clear();

void fb_write_cell(unsigned int row, unsigned int col,
		char c, unsigned char fg, unsigned char bg);

#endif
