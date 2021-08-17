#include "fb.h"

static char* fb = (char*) 0x000B8000;

#define FB_COL_LIMIT 80
#define FB_ROW_LIMIT 80

void fb_init_context(FbContext* context) {
   context->row = 0;
   context->col = 0;
}

void fb_write_char(const char c, FbContext* context) {
   if (context->row >= FB_ROW_LIMIT) {
      return;
   }
   if (c == '\n') {
      context->row = context->row + 1;
      context->col = 0;
      return;
   }
   fb_write_cell(context->row, context->col, c, FB_WHITE, FB_BLACK);
   if (context->col == FB_COL_LIMIT) {
      context->col = 0;
      context->row = context->row + 1;
   } else {
      context->col = context->col + 1;
   }
}

void fb_write_text(const char* str, FbContext* context) {
   for (int i = 0; *(str + i); ++i) {
      fb_write_char(*(str + i), context);
   }
}

void fb_clear() {
   for (int i = 0; i < FB_ROW_LIMIT; ++i) {
      for (int j = 0; j < FB_COL_LIMIT; ++j) {
         fb_write_cell(i, j, ' ', FB_WHITE, FB_BLACK);
      }
   }
}

void fb_write_cell(unsigned int row, unsigned int col,
      char c, unsigned char fg, unsigned char bg) {
   unsigned int position = row * FB_COL_LIMIT + col;
   fb[position * 2] = c;
   fb[position * 2 + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}
