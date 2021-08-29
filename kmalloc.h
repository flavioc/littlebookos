#ifndef KMALLOC_H
#define KMALLOC_H

#include "types.h"

u32int kmalloc(u32int sz);
u32int kmalloc_a(u32int sz);

u32int kmalloc_p(u32int sz, u32int *phys);
u32int kmalloc_ap(u32int sz, u32int *phys);

#endif
