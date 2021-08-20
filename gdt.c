
#include "gdt.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

extern void gdt_flush(unsigned int gdt);

static void gdt_set_null_gate() {
   gdt_entries[0].base_low = 0;
   gdt_entries[0].base_middle = 0;
   gdt_entries[0].base_high = 0;

   gdt_entries[0].limit_low = 0;
   gdt_entries[0].limit_high = 0;

   gdt_entries[0].available_for_system_use = 0;
   gdt_entries[0].always_zero = 0;
   gdt_entries[0].operand_size = 0;
   gdt_entries[0].granularity = 0;

   gdt_entries[0].is_segment_present = 0;
   gdt_entries[0].descriptor_privilege_level = 0;
   gdt_entries[0].descriptor_type = 0;
   gdt_entries[0].segment_type = 0;
}

static void gdt_set_gate(unsigned int num, unsigned int base, unsigned int limit,
      unsigned char privilege_level,
      unsigned char segment_type) {
   gdt_entries[num].base_low = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low = (limit & 0xFFFF);
   gdt_entries[num].limit_high = (limit >> 16) & 0x0F;

   gdt_entries[num].available_for_system_use = 0;
   gdt_entries[num].always_zero = 0;
   // 32 bit
   gdt_entries[num].operand_size = 1;
   // 1KB
   gdt_entries[num].granularity = 1;

   gdt_entries[num].is_segment_present = 1;
   gdt_entries[num].descriptor_privilege_level = privilege_level;
   // Storage segment descriptor
   gdt_entries[num].descriptor_type = 1;
   gdt_entries[num].segment_type = segment_type;
}

void gdt_init() {
   gdt_ptr.limit = sizeof(gdt_entry_t) * 5 - 1;
   gdt_ptr.base = (unsigned int)&gdt_entries;
#define MAX_LIMIT 0xFFFFFFFF
#define SEGMENT_CODE 0xA
#define SEGMENT_DATA 0x2

   gdt_set_null_gate();
   gdt_set_gate(1, 0, MAX_LIMIT, /*privilege_level=*/0, /*segment_type=*/SEGMENT_CODE);
   gdt_set_gate(2, 0, MAX_LIMIT, /*privilege_level=*/0, /*segment_type=*/SEGMENT_DATA);
   gdt_set_gate(3, 0, MAX_LIMIT, /*privilege_level=*/3, /*segment_type=*/SEGMENT_CODE);
   gdt_set_gate(4, 0, MAX_LIMIT, /*privilege_level=*/3, /*segment_type=*/SEGMENT_DATA);
   
   gdt_flush((unsigned int)&gdt_ptr);
}
