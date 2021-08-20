
#include "gdt.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

extern void gdt_flush(unsigned int gdt);

static void gdt_set_gate(unsigned int num, unsigned int base, unsigned int limit,
      unsigned char access, unsigned char granularity) {
   gdt_entries[num].base_low = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low = (limit & 0xFFFF);

   gdt_entries[num].granularity = (limit >> 16) & 0x0F;
   gdt_entries[num].granularity |= granularity & 0xF0;

   gdt_entries[num].access = access;
}

void gdt_init() {
   gdt_ptr.limit = sizeof(gdt_entry_t) * 5 - 1;
   gdt_ptr.base = (unsigned int)&gdt_entries;
#define MAX_LIMIT 0xFFFFFFFF

   gdt_set_gate(0, 0, 0, 0, 0); // Null segment
   gdt_set_gate(1, 0, MAX_LIMIT, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, MAX_LIMIT, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, MAX_LIMIT, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, MAX_LIMIT, 0xF2, 0xCF); // User mode data segment
   
   gdt_flush((unsigned int)&gdt_ptr);
}
