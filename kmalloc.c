#include "kmalloc.h"

extern u32int end;
u32int placement_address = (u32int)&end;

u32int kmalloc(u32int sz) {
	u32int tmp = placement_address;
	placement_address += sz;
	return tmp;
}

static void align_placement_address() {
	if (placement_address & 0xFFF) {
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
}

u32int kmalloc_a(u32int sz) {
	align_placement_address();
	return kmalloc(sz);
}

u32int kmalloc_p(u32int sz, u32int *phys) {
	if (phys) {
		*phys = placement_address;
	}
	return kmalloc(sz);
}

u32int kmalloc_ap(u32int sz, u32int *phys) {
	align_placement_address();
	if (phys) {
		*phys = placement_address;
	}
	return kmalloc(sz);
}
