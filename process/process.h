#ifndef PROCESS_H
#define PROCESS_H

#define _XOPEN_SOURCE 700
#include <ucontext.h>

#define MAX_PROCESSES    16
#define PROCESS_NAME_LEN 32
#define STACK_SIZE       65536

typedef enum {
    PROC_READY,
    PROC_RUNNING,
    PROC_TERMINATED
} ProcessState;

typedef struct {
    int          pid;
    char         name[PROCESS_NAME_LEN];
    ProcessState state;
    void         (*func)();
    ucontext_t   ctx;
    char         stack[STACK_SIZE];
} Process;

/* Phase 1 — process table */
void process_init();
int  process_spawn(char* name, void (*func)());
void process_kill(int pid);
void process_print_all();

/* Phase 2 — scheduler */
void scheduler_init();
void scheduler_tick();
void scheduler_start_preemption();
void process_yield();

#endif
