#ifndef MEMORY_H
#define MEMORY_H

void init_memory();
void* alloc(int size);
void dealloc(void* ptr);

#endif