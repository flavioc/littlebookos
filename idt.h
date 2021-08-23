#ifndef IDT_H
#define IDT_H

// Struct to handle a particular interrupt.
struct idt_entry_struct {
	unsigned short base_lo : 16;
	unsigned short segment_selector : 16;
	unsigned char always_zero : 8;
   unsigned char gate_type : 5;
   unsigned char descriptor_privilege_level : 2;
   unsigned char entry_is_present : 1;
	unsigned short base_hi : 16;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// Struct containing a pointer to an array of interrupt handlers.
// This will be passed to the lidt instruction.
struct idt_ptr_struct
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

typedef struct registers {
   unsigned int ds; // data segment selector
   unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha.
   unsigned int int_no, err_code; // interrupt number and error code
   unsigned int eip, cs, eflags, useresp, ss; // pushed by the processor
} registers_t;

void idt_init();

#endif
