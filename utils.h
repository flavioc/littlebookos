#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define PANIC(msg) panic(msg, __FILE__, __LINE__);

void panic(const char* message, const char* file, u32int line);

#endif
