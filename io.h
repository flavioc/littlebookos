#ifndef IO_H
#define IO_H

#include "types.h"

unsigned char inb(u16int port);
void outb(u16int port, u8int data);

#endif
