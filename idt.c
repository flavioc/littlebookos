#include "fb.h"
#include "idt.h"
#include "serial.h"

static idt_entry_t idt_entries[256] = {0};
static idt_ptr_t idt_ptr;
extern void load_idt (unsigned int);
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void isr_handler(registers_t regs) {
   serial_print(SERIAL_COM1_BASE, "Got interrupt ");
   serial_write(SERIAL_COM1_BASE, '0' + regs.int_no);
}

static void idt_set_gate(unsigned char num, unsigned int base, unsigned short segment_selector, unsigned char privilege_level)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].segment_selector     = segment_selector;
   idt_entries[num].always_zero = 0;
#define INTERRUPT_GATE 0x0E
   idt_entries[num].gate_type = INTERRUPT_GATE;
   idt_entries[num].descriptor_privilege_level = privilege_level;
   idt_entries[num].entry_is_present = 1;
}

void idt_init() {
   idt_set_gate(0, (unsigned int)isr0 , 0x08, /*privilege_level=*/0);
   idt_set_gate(1, (unsigned int)isr1 , 0x08, /*privilege_level=*/0);
   idt_set_gate(2, (unsigned int)isr2 , 0x08, /*privilege_level=*/0);
   idt_set_gate(3, (unsigned int)isr3 , 0x08, /*privilege_level=*/0);
   idt_set_gate(4, (unsigned int)isr4 , 0x08, /*privilege_level=*/0);
   idt_set_gate(5, (unsigned int)isr5 , 0x08, /*privilege_level=*/0);
   idt_set_gate(6, (unsigned int)isr6 , 0x08, /*privilege_level=*/0);
   idt_set_gate(7, (unsigned int)isr7 , 0x08, /*privilege_level=*/0);
   idt_set_gate(8, (unsigned int)isr8 , 0x08, /*privilege_level=*/0);
   idt_set_gate(9, (unsigned int)isr9 , 0x08, /*privilege_level=*/0);
   idt_set_gate(10, (unsigned int)isr10, 0x08, /*privilege_level=*/0);
   idt_set_gate(11, (unsigned int)isr11, 0x08, /*privilege_level=*/0);
   idt_set_gate(12, (unsigned int)isr12, 0x08, /*privilege_level=*/0);
   idt_set_gate(13, (unsigned int)isr13, 0x08, /*privilege_level=*/0);
   idt_set_gate(14, (unsigned int)isr14, 0x08, /*privilege_level=*/0);
   idt_set_gate(15, (unsigned int)isr15, 0x08, /*privilege_level=*/0);
   idt_set_gate(16, (unsigned int)isr16, 0x08, /*privilege_level=*/0);
   idt_set_gate(17, (unsigned int)isr17, 0x08, /*privilege_level=*/0);
   idt_set_gate(18, (unsigned int)isr18, 0x08, /*privilege_level=*/0);
   idt_set_gate(19, (unsigned int)isr19, 0x08, /*privilege_level=*/0);
   idt_set_gate(20, (unsigned int)isr20, 0x08, /*privilege_level=*/0);
   idt_set_gate(21, (unsigned int)isr21, 0x08, /*privilege_level=*/0);
   idt_set_gate(22, (unsigned int)isr22, 0x08, /*privilege_level=*/0);
   idt_set_gate(23, (unsigned int)isr23, 0x08, /*privilege_level=*/0);
   idt_set_gate(24, (unsigned int)isr24, 0x08, /*privilege_level=*/0);
   idt_set_gate(25, (unsigned int)isr25, 0x08, /*privilege_level=*/0);
   idt_set_gate(26, (unsigned int)isr26, 0x08, /*privilege_level=*/0);
   idt_set_gate(27, (unsigned int)isr27, 0x08, /*privilege_level=*/0);
   idt_set_gate(28, (unsigned int)isr28, 0x08, /*privilege_level=*/0);
   idt_set_gate(29, (unsigned int)isr29, 0x08, /*privilege_level=*/0);
   idt_set_gate(30, (unsigned int)isr30, 0x08, /*privilege_level=*/0);
   idt_set_gate(31, (unsigned int)isr31, 0x08, /*privilege_level=*/0);
   idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
   idt_ptr.base = (unsigned int)&idt_entries[0];
   load_idt((unsigned int)&idt_ptr);
}

