#include "memory.h"

#define RAM_SIZE 1024 * 1024

static char RAM[RAM_SIZE];
static int free_ptr = 0;

void init_memory() {
    free_ptr = 0;
}

void* alloc(int size) {
    if (free_ptr + size >= RAM_SIZE) {
        return 0;
    }

    void* ptr = &RAM[free_ptr];
    free_ptr += size;
    return ptr;
}