#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

// Public functions
char* xmalloc(size_t size);
void xfree(void* ptr);

#endif