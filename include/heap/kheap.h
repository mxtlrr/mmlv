/* malloc:
*    Search every block for a size >= n. If size>n, then
*    split the block into 2:
*       | Allocated | Rest |
*         ^^^ use this one to allocate the memory. Then,
*             return the start of that block.
*    If size = n, allocate to that block.
*    If size < n, go to the next block. If all blocks are too
*       small, then return 0. */
#pragma once

#include <stdint.h>
#include "structs.h" // Block_h

#include "libc/output.h"

#define BLOCK_SIZES 256

#define FREE   0x1
#define ALLOC  0x0 // Memory is allocated already.

extern uint64_t usable; // Is 0x7FFFF000 usable? (Call first in kernel.)
extern block_t blocks[BLOCK_SIZES];

// 1. Initialize usable
// 2. Set up blocks to zero
void set_up_kheap(int usable_n);

// Allocate n bytes and return the address of the starting position
// of the memory.
uintptr_t malloc(uint64_t n);

// Does the same thing malloc does, but sets every byte in the location
// of the allocated to zero.
uintptr_t calloc(uintptr_t n);

void free(uint64_t addr);