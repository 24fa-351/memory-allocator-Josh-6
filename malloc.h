#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

// Public functions
void* xmalloc(size_t size);
//char* xmalloc(size_t size);
void xfree(void* ptr);
void* xrealloc(void* ptr, size_t size);
//char* xrealloc(void* ptr, size_t size);

#endif