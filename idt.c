#include "fb.h"
#include "idt.h"
#include "io.h"
#include "serial.h"

static idt_entry_t idt_entries[256] = {0};
static idt_ptr_t idt_ptr;
isr_t interrupt_handlers[256] = {0};

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
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void isr_handler(registers_t regs) {
   serial_print(SERIAL_COM1_BASE, "Got interrupt ");
   serial_write(SERIAL_COM1_BASE, '0' + regs.int_no);
}

static void idt_set_gate(u8int num, u32int base, u16int segment_selector, u8int privilege_level)
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

#define PIC1 0x20 // IO base address for master PIC
#define PIC2 0xA0 // IO base address for slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

void irq_handler(registers_t regs) {
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40) {
      // Send reset signal to slave.
      outb(PIC2, PIC_EOI);
   }
   // Send reset signal to master.
   outb(PIC1,  PIC_EOI);

   if (interrupt_handlers[regs.int_no] != 0) {
      isr_t handler = interrupt_handlers[regs.int_no];
      handler(regs);
   }
}

void register_interrupt_handler(u8int n, isr_t handler) {
   interrupt_handlers[n] = handler;
}

static void pic_remap(int offset1, int offset2) {
   unsigned char a1, a2;

   // Get masks.
   a1 = inb(PIC1_DATA);
   a2 = inb(PIC2_DATA);

   // Start initialization sequence.
   outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
   outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
   // Set offsets for pic1 and pic2
   outb(PIC1_DATA, offset1);
   outb(PIC2_DATA, offset2);
   // Tell master where the slave is
   outb(PIC1_DATA, ICW1_INTERVAL4);
   // Tell slave its cascade identity.
   outb(PIC2_DATA, ICW1_SINGLE);
   // Enable 8086/88 mode.
   outb(PIC1_DATA, ICW4_8086);
   outb(PIC2_DATA, ICW4_8086);
   // Restore masks
   outb(PIC1_DATA, a1);
   outb(PIC2_DATA, a2);
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
   // Remap the irq table.
   // PIC1 mapped to [32, ..., 39]
   // PIC2 mapped to [40, ..., 47]
   pic_remap(32, 40);
   idt_set_gate(32, (u32int)irq0, 0x08, /*privilege_level=*/0);
   idt_set_gate(33, (u32int)irq1, 0x08, /*privilege_level=*/0);
   idt_set_gate(34, (u32int)irq2, 0x08, /*privilege_level=*/0);
   idt_set_gate(35, (u32int)irq3, 0x08, /*privilege_level=*/0);
   idt_set_gate(36, (u32int)irq4, 0x08, /*privilege_level=*/0);
   idt_set_gate(37, (u32int)irq5, 0x08, /*privilege_level=*/0);
   idt_set_gate(38, (u32int)irq6, 0x08, /*privilege_level=*/0);
   idt_set_gate(39, (u32int)irq7, 0x08, /*privilege_level=*/0);
   idt_set_gate(40, (u32int)irq8, 0x08, /*privilege_level=*/0);
   idt_set_gate(41, (u32int)irq9, 0x08, /*privilege_level=*/0);
   idt_set_gate(42, (u32int)irq10, 0x08, /*privilege_level=*/0);
   idt_set_gate(43, (u32int)irq11, 0x08, /*privilege_level=*/0);
   idt_set_gate(44, (u32int)irq12, 0x08, /*privilege_level=*/0);
   idt_set_gate(45, (u32int)irq13, 0x08, /*privilege_level=*/0);
   idt_set_gate(46, (u32int)irq14, 0x08, /*privilege_level=*/0);
   idt_set_gate(47, (u32int)irq15, 0x08, /*privilege_level=*/0);
   idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
   idt_ptr.base = (unsigned int)&idt_entries[0];
   load_idt((unsigned int)&idt_ptr);
}

