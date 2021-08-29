#include "fb.h"
#include "idt.h"
#include "kmalloc.h"
#include "paging.h"
#include "serial.h"
#include "utils.h"

#define GENERAL_PROTECTION_FAULT_INTERRUPT 14
#define PAGE_SIZE_BYTES 0x1000

// Bitset of frames - used or free.
static u32int *frames;
static u32int nframes;
static page_directory_t *current_directory = 0;
static page_directory_t *kernel_directory = 0;

extern u32int placement_address;
extern u32int code;
extern u32int data;
extern u32int bss;
extern u32int start;
extern u32int end;

#define INDEX_FROM_BIT(a) (a/(8*sizeof(u32int)))
#define OFFSET_FROM_BIT(a) (a%(8*sizeof(u32int)))

static void memset(void* ptr, u8int what, int size) {
   u8int* p = ptr;
   for (; size > 0; --size) {
      *p++ = what;
   }
}

static void set_frame(u32int frame_addr) {
   u32int frame = frame_addr/PAGE_SIZE_BYTES;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);

   frames[idx] |= (0x1 << off);
}

static void clear_frame(u32int frame_addr) {
   u32int frame = frame_addr/PAGE_SIZE_BYTES;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);

   frames[idx] &= ~(0x1 << off);
}

u32int test_frame(u32int frame_addr) {
   u32int frame = frame_addr/PAGE_SIZE_BYTES;
   u32int idx = INDEX_FROM_BIT(frame);
   u32int off = OFFSET_FROM_BIT(frame);
   return frames[idx] & (0x1 << off);
}

static u32int first_frame() {
   for (u32int i = 0; i < INDEX_FROM_BIT(nframes); i++) {
      if (frames[i] != 0xFFFFFFFF) {
         for (u32int j = 0; j < sizeof(u32int)*8; j++) {
            u32int to_test = 0x1 << j;
            if (!(frames[i] & to_test)) {
               return i*sizeof(u32int)*8+j;
            }
         }
      }
   }
   return -1;
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
   if (page->frame != 0) {
      // Frame already allocated
      return;
   }
   u32int idx = first_frame();
   if (idx == (u32int)-1) {
      PANIC("No free frames");
      return;
   }
   set_frame(idx*PAGE_SIZE_BYTES);
   page->present = 1;
   page->rw = is_writeable ? 1 : 0;
   page->user = is_kernel ? 0 : 1;
   page->frame = idx;
}

void free_frame(page_t *page) {
   u32int frame = page->frame;
   if (!frame) {
      return;
   }
   clear_frame(frame * PAGE_SIZE_BYTES);
   page->frame = 0;
}

void paging_init() {
   // 16MB end of page.
   u32int mem_end_page = 0x1000000;

   nframes = mem_end_page / PAGE_SIZE_BYTES;
   frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));
   memset(frames, 0, INDEX_FROM_BIT(nframes));

   kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
   memset(kernel_directory, 0, sizeof(page_directory_t));

   u32int i = 0;
   while (i < placement_address) {
      alloc_frame(paging_get_page(i, /*make=*/1, kernel_directory), /*is_kernel=*/0, /*is_writeable=*/0);
      i += PAGE_SIZE_BYTES;
   }
   register_interrupt_handler(GENERAL_PROTECTION_FAULT_INTERRUPT, paging_fault);

   paging_switch_directory(kernel_directory);
}

void paging_switch_directory(page_directory_t *dir) {
   current_directory = dir;
   __asm__ volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
   u32int cr0;
   __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000;
   __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *paging_get_page(u32int address, int make, page_directory_t *dir) {
   // Divide by 0x1000
   address /= PAGE_SIZE_BYTES;

   // We will be left with 10 remaining bits, which fit in 1024.
   u32int page_directory_index  = address >> 10;
   u32int page_table_offset = address & 0x3FF;
   if (dir->tables[page_directory_index]) {
      return &dir->tables[page_directory_index]->pages[page_table_offset];
   }
   if (make) {
      u32int tmp;
      dir->tables[page_directory_index] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
      memset(dir->tables[page_directory_index], 0, sizeof(page_table_t));
      dir->tables_physical[page_directory_index] = tmp | 0x7; // PRESENT, RW, US
      return &dir->tables[page_directory_index]->pages[page_table_offset];
   }
   return 0;
}

void paging_fault(registers_t regs) {
   // Faulting address is at CR2 register.
   u32int faulting_address;
   __asm__ volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present = !(regs.err_code & 0x1); // page not present
   int rw = regs.err_code & 0x2; // write operation?
   int us = regs.err_code & 0x4; // Processor was in user-mode?
   int reserved = regs.err_code & 0x8; // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10; // Caused by an instruction fetch?

   serial_print(SERIAL_COM1_BASE,"Page fault! ( ");
   if (present) {
      serial_print(SERIAL_COM1_BASE,"present ");
   }
   if (rw) {
      serial_print(SERIAL_COM1_BASE,"read-only ");
   }
   if (us) {
      serial_print(SERIAL_COM1_BASE,"user-mode ");
   }
   if (reserved) {
      serial_print(SERIAL_COM1_BASE,"reserved ");
   }
   if (id) {
      serial_print(SERIAL_COM1_BASE,"instruction-fetch ");
   }
   serial_print(SERIAL_COM1_BASE,") at ");
   serial_write_int(SERIAL_COM1_BASE, faulting_address);
   serial_print(SERIAL_COM1_BASE,"\n");
   PANIC("Page fault");
}
