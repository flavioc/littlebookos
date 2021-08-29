#ifndef PAGING_H
#define PAGING_H

#include "types.h"

typedef struct page {
   u32int present : 1;
   u32int rw : 1;
   u32int user : 1;
   u32int accessed : 1;
   u32int dirty : 1;
   u32int unused : 7;
   u32int frame : 20;
} page_t;

typedef struct page_table
{
   page_t pages[1024];
} page_table_t;

typedef struct page_directory {
   page_table_t *tables[1024];
   // Same as above but gives the physical addresses.
   u32int tables_physical[1024];
   // Physical address of tables_physical;
   u32int physical_addr;
} page_directory_t;

void paging_init();

void paging_switch_directory(page_directory_t *new);

page_t* paging_get_page(u32int address, int make, page_directory_t* dir);

void paging_fault(registers_t reg);

#endif
