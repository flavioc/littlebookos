#ifndef FB_H
#define FB_H

#include "types.h"

// Framebuffer colors.
#define FB_BLACK 0
#define FB_BLUE 1
#define FB_GREEN 2
#define FB_CYAN 3
#define FB_RED 4
#define FB_DARK_GREY 8
#define FB_WHITE 15

// The I/O ports.
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

// The I/O port commands.
#define FB_CURSOR_START_HIGH_COMMAND 0xA
#define FB_CURSOR_START_LOW_COMMAND 0xB
#define FB_CURSOR_LOCATION_HIGH_COMMAND 0xE
#define FB_CURSOR_LOCATION_LOW_COMMAND 0xF

void fb_write_text(const char* str);
void fb_write_char(char c);
void fb_backspace(u8int overwrite);

void fb_clear();

void fb_write_cell(u8int row, u8int col, char c, u8int fg, u8int bg);

void fb_enable_cursor();
void fb_move_cursor(u8int row, u8int col);

#endif
