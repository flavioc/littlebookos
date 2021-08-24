#ifndef IDT_H
#define IDT_H

#include "types.h"

// Struct to handle a particular interrupt.
struct idt_entry_struct {
   u16int base_lo : 16;
   u16int segment_selector : 16;
   u8int always_zero : 8;
   u8int gate_type : 5;
   u8int descriptor_privilege_level : 2;
   u8int entry_is_present : 1;
   u16int base_hi : 16;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// Struct containing a pointer to an array of interrupt handlers.
// This will be passed to the lidt instruction.
struct idt_ptr_struct
{
   u16int limit;
   u32int base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

typedef struct registers {
   u32int ds; // data segment selector
   u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha.
   u32int int_no, err_code; // interrupt number and error code
   u32int eip, cs, eflags, useresp, ss; // pushed by the processor
} registers_t;

void idt_init();

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(u8int n, isr_t handler);

#endif
