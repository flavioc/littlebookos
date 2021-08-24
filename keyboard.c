#include "fb.h"
#include "io.h"
#include "keyboard.h"
#include "serial.h"

#define KEYBOARD_DATA_PORT 0x60
#define ESC_SCAN_CODE 0x01
#define TAB_SCAN_CODE 0x3A
#define ENTER_SCAN_CODE 0x1C

static int caps_lock = 0;

char kbdus[128] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   0,     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   0,                    /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

void keyboard_handler(registers_t registers) {
   (void) registers;

   for (int i = 1000; i > 0; --i) {
      if ((inb(0x64) & 0x01) == 0) {
         continue;
      }
      u8int scancode = inb(KEYBOARD_DATA_PORT);
      if (scancode & 0x80) {
         // key release
      } else {
         switch (scancode) {
            case TAB_SCAN_CODE:
               caps_lock = !caps_lock;
               break;
            case ENTER_SCAN_CODE:
               fb_write_char('\n');
               break;
            case ESC_SCAN_CODE:
               break;
            default: {
               char key = kbdus[scancode];
               if (key == '\t') {
                  fb_write_text("  ");
                  key = 0;
               } if (key >= 'a' && key <= 'z' && caps_lock) {
                  key = 'A' + (key - 'a');
               }
               if (key != 0) {
                  fb_write_char(key);
               }
               break;
            }
         }
      }
      return;
   }
}

