#include "memory.h"

#define RAM_SIZE (1024 * 1024)

typedef struct {
    int size;
    int free;
} Block;

#define HEADER ((int)sizeof(Block))

static char RAM[RAM_SIZE];
static int  free_ptr      = 0;
static int  overflow_flag = 0;

void init_memory() {
    free_ptr      = 0;
    overflow_flag = 0;
}

// Walk the already-committed region for a free block >= size (first-fit).
static Block* find_free_block(int size) {
    int offset = 0;
    while (offset + HEADER <= free_ptr) {
        Block* blk = (Block*)&RAM[offset];
        if (blk->free && blk->size >= size)
            return blk;
        offset += HEADER + blk->size;
    }
    return 0;
}

void* alloc(int size) {
    if (size <= 0 || size > RAM_SIZE) {
        overflow_flag = 1;
        return 0;
    }

    // Reuse a freed block when possible.
    Block* blk = find_free_block(size);
    if (blk) {
        blk->free     = 0;
        overflow_flag = 0;
        return (char*)blk + HEADER;
    }

    // Fall back to bump allocation.
    int needed = HEADER + size;
    if (needed > RAM_SIZE - free_ptr) {
        overflow_flag = 1;
        return 0;
    }

    overflow_flag       = 0;
    Block* new_blk      = (Block*)&RAM[free_ptr];
    new_blk->size       = size;
    new_blk->free       = 0;
    free_ptr           += needed;
    return (char*)new_blk + HEADER;
}

void dealloc(void* ptr) {
    if (ptr == 0) return;
    Block* blk = (Block*)((char*)ptr - HEADER);
    blk->free = 1;
}

// Bytes in live (non-freed) allocations, excluding headers.
int mem_used() {
    int used = 0, offset = 0;
    while (offset + HEADER <= free_ptr) {
        Block* blk = (Block*)&RAM[offset];
        if (!blk->free)
            used += blk->size;
        offset += HEADER + blk->size;
    }
    return used;
}

// Reclaimed freed bytes + completely unallocated tail.
int mem_available() {
    int avail = RAM_SIZE - free_ptr;
    int offset = 0;
    while (offset + HEADER <= free_ptr) {
        Block* blk = (Block*)&RAM[offset];
        if (blk->free)
            avail += blk->size;
        offset += HEADER + blk->size;
    }
    return avail;
}

int mem_overflow() { return overflow_flag; }
