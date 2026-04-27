#ifndef MEMORY_H
#define MEMORY_H

void  init_memory();
void* alloc(int size);
void  dealloc(void* ptr);

int   mem_used();
int   mem_available();
int   mem_overflow();   // returns 1 if the last alloc failed due to OOM

#endif