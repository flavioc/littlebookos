#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "idt.h"

#define IRQ_INTERRUPT_BASE 32
#define KEYBOARD_INTERRUPT (IRQ_INTERRUPT_BASE + 1)

void keyboard_handler(registers_t registers);

#endif
