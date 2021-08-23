#ifndef GDT_H
#define GDT_H

#include "types.h"

struct gdt_entry_struct {
   u16int limit_low;
   u16int base_low;
   u8int base_middle;

   // Data or code.
   unsigned char segment_type : 4;
   // 1 for yes. 0 for the null segment.
   unsigned char is_segment_present : 1;
   // 0 for Ring 0, 3 for Ring 3.
   unsigned char descriptor_privilege_level : 2;
   unsigned char descriptor_type : 1;

   unsigned char limit_high : 4;
   unsigned char available_for_system_use : 1;
   unsigned char always_zero : 1;
   // Operand size:
   // 0 - 16 bit
   // 1 - 32 bit
   unsigned char operand_size : 1;
   // Granularity:
   // 0 -> 1 byte
   // 1 -> 1 kbyte
   unsigned char granularity : 1;
   u8int base_high;
} __attribute ((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct {
	// The upper 16 bits of all selector limits.
	// Should be size of table - 1.
	u16int limit;
	// Address of the first gdt_entry_struct.
	u32int base;
} __attribute__ ((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void gdt_init();

#endif
