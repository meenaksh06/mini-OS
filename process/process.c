#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include "process.h"
#include "../string/string.h"
#include "../screen/screen.h"
#include <signal.h>
#include <sys/time.h>
#include <stddef.h>

static Process    table[MAX_PROCESSES];
static int        next_pid    = 1;
static int        running_idx = -1;
static ucontext_t sched_ctx;

/* ── Phase 1: process table ── */

void process_init() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        table[i].pid   = 0;
        table[i].state = PROC_TERMINATED;
    }
    next_pid    = 1;
    running_idx = -1;
}

/* Entry point for every process — wraps func() so we can mark it
   terminated when it returns, then fall back to the scheduler. */
static void process_wrapper() {
    table[running_idx].func();
    table[running_idx].state = PROC_TERMINATED;
    running_idx = -1;
    /* uc_link returns control to sched_ctx */
}

int process_spawn(char* name, void (*func)()) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table[i].state == PROC_TERMINATED) {
            table[i].pid   = next_pid++;
            table[i].state = PROC_READY;
            table[i].func  = func;
            int j = 0;
            while (name[j] && j < PROCESS_NAME_LEN - 1)
                table[i].name[j] = name[j], j++;
            table[i].name[j] = '\0';

            getcontext(&table[i].ctx);
            table[i].ctx.uc_stack.ss_sp   = table[i].stack;
            table[i].ctx.uc_stack.ss_size = STACK_SIZE;
            table[i].ctx.uc_link          = &sched_ctx;
            makecontext(&table[i].ctx, process_wrapper, 0);

            return table[i].pid;
        }
    }
    return -1;
}

void process_kill(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table[i].pid == pid && table[i].state != PROC_TERMINATED) {
            table[i].state = PROC_TERMINATED;
            return;
        }
    }
}

void process_print_all() {
    int found = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (table[i].state == PROC_TERMINATED) continue;
        if (!found) {
            print_string("PID  STATE    NAME\n");
            found = 1;
        }
        print_string(int_to_str(table[i].pid));
        print_string("    ");
        if      (table[i].state == PROC_RUNNING) print_string("RUNNING  ");
        else if (table[i].state == PROC_READY)   print_string("READY    ");
        print_string(table[i].name);
        print_string("\n");
    }
    if (!found) print_string("No active processes.\n");
}

/* ── Phase 2: scheduler ── */

void scheduler_init() {
    getcontext(&sched_ctx);
}

/* Round-robin: find the next READY process and switch to it.
   Returns immediately if nothing is ready. */
void scheduler_tick() {
    int start = running_idx < 0 ? 0 : (running_idx + 1) % MAX_PROCESSES;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        int idx = (start + i) % MAX_PROCESSES;
        if (table[idx].state == PROC_READY) {
            running_idx      = idx;
            table[idx].state = PROC_RUNNING;
            swapcontext(&sched_ctx, &table[idx].ctx);
            return;
        }
    }
}

/* Called by a background task to hand the CPU back to the scheduler. */
void process_yield() {
    if (running_idx >= 0 && table[running_idx].state == PROC_RUNNING) {
        table[running_idx].state = PROC_READY;
        swapcontext(&table[running_idx].ctx, &sched_ctx);
    }
}

static void timer_handler(int sig) {
    (void)sig;
    process_yield();
}

void scheduler_start_preemption() {
    struct sigaction sa;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100000; // 100ms quantum
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000;
    setitimer(ITIMER_REAL, &timer, NULL);
}
