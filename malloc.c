#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include "malloc.h"


// Block structure for tracking memory blocks
typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

static Block* free_list = NULL;  // Linked list to track free memory blocks

#define ALIGN_SIZE 8  // Align to 8 bytes for better performance

// Align memory size to the nearest multiple of ALIGN_SIZE
size_t align_size(size_t size) {
    return (size + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1);
}

// Function to request memory using sbrk()
void* get_me_blocks(ssize_t how_much) {
    void* ptr = sbrk(0);  // Get the current end of the heap
    if (sbrk(how_much) == (void*)-1) {  // Request more memory
        return NULL;
    }
    return ptr;  // Return the starting address of the new memory block
}

// Custom malloc function using sbrk
void* xmalloc(size_t size) {
    size = align_size(size);  // Align the requested size

    // Check if there is a free block in the free list
    Block* prev = NULL;
    Block* curr = free_list;
    while (curr != NULL) {
        if (curr->size >= size) {  // Found a large enough free block
            if (prev != NULL) {
                prev->next = curr->next;  // Remove it from the free list
            } else {
                free_list = curr->next;
            }
            return (void*)((char*)curr + sizeof(Block));  // Return memory after metadata
        }
        prev = curr;
        curr = curr->next;
    }
// No suitable free block, use get_me_blocks() to get more memory
    Block* block = (Block*)get_me_blocks(sizeof(Block) + size);  // Allocate space for block and user data
    if (block == NULL) {  // Check if sbrk failed
        return NULL;
    }

    block->size = size;  // Set block size
    block->next = NULL;   // No next block
    return (void*)((char*)block + sizeof(Block));  // Return memory after metadata
}


// Custom free function
void xfree(void* ptr) {
    if (ptr == NULL) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));  // Get block metadata
    block->next = free_list;  // Add block to free list
    free_list = block;  // Insert at the front of free list
}


// Custom realloc function
void* xrealloc(void* ptr, size_t new_size) {
    if (ptr == NULL) {
        return xmalloc(new_size);  // If pointer is NULL, just allocate new memory
    }
    if (new_size == 0) {
        xfree(ptr);  // If new size is 0, free memory
        return NULL;
    }
}