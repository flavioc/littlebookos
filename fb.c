#include "fb.h"
#include "io.h"

static char* fb = (char*) 0x000B8000;

#define FB_COL_LIMIT 80
#define FB_ROW_LIMIT 25

typedef struct _FbContext {
   unsigned char row, col;
} FbContext;

static FbContext context = {0};

void fb_init_context(FbContext* context) {
   context->row = 0;
   context->col = 0;
}

static void fb_do_write_char(const char c) {
   if (context.row >= FB_ROW_LIMIT) {
      return;
   }
   if (c == '\n') {
      context.row = context.row + 1;
      context.col = 0;
      return;
   }
   fb_write_cell(context.row, context.col, c, FB_WHITE, FB_BLACK);
   if (context.col == FB_COL_LIMIT) {
      context.col = 0;
      context.row = context.row + 1;
   } else {
      context.col = context.col + 1;
   }
}

void fb_write_text(const char* str) {
   for (int i = 0; *(str + i); ++i) {
      fb_write_char(*(str + i));
   }
   fb_move_cursor(context.row, context.col);
}

void fb_write_char(char c) {
   fb_do_write_char(c);
   fb_move_cursor(context.row, context.col);
}

void fb_clear() {
   for (int i = 0; i < FB_ROW_LIMIT; ++i) {
      for (int j = 0; j < FB_COL_LIMIT; ++j) {
         fb_write_cell(i, j, ' ', FB_WHITE, FB_BLACK);
      }
   }
}

void fb_write_cell(u8int row, u8int col, char c, u8int fg, u8int bg) {
   unsigned int position = row * FB_COL_LIMIT + col;
   fb[position * 2] = c;
   fb[position * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void fb_enable_cursor() {
   // The difference between end and start represents the thickness of the cursor.
   // See pcjs.org/blog/2018/03/20/.
#define CURSOR_SCANLINE_START 10
#define CURSOR_SCANLINE_END 15
   outb(FB_COMMAND_PORT, FB_CURSOR_START_HIGH_COMMAND);
   // Note that we & with 0xC0 to not disable the cursor:
   // http://osdever.net/FreeVGA/vga/crtcreg.htm#0A
   outb(FB_DATA_PORT, (inb(FB_DATA_PORT) & 0xC0) | CURSOR_SCANLINE_START);
   outb(FB_COMMAND_PORT, FB_CURSOR_START_LOW_COMMAND);
   // We keep cursor skew settings.
   outb(FB_DATA_PORT, (inb(FB_DATA_PORT) & 0xE0) | CURSOR_SCANLINE_END);
}

void fb_move_cursor(u8int row, u8int col) {
   if (row >= FB_ROW_LIMIT || col >= FB_COL_LIMIT) {
      return;
   }
   unsigned short pos = row * FB_COL_LIMIT + col;
   outb(FB_COMMAND_PORT, FB_CURSOR_LOCATION_HIGH_COMMAND);
   outb(FB_DATA_PORT, (pos >> 8) & 0x00FF);
   outb(FB_COMMAND_PORT, FB_CURSOR_LOCATION_LOW_COMMAND);
   outb(FB_DATA_PORT, pos & 0x00FF);
}
