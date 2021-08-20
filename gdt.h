#ifndef GDT_H
#define GDT_H

struct gdt_entry_struct {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute ((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct {
	// The upper 16 bits of all selector limits.
	// Should be size of table - 1.
	unsigned short limit;
	// Address of the first gdt_entry_struct.
	unsigned int base;
} __attribute__ ((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

void gdt_init();

#endif
