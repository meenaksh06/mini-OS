#include "math/math.h"
#include "string/string.h"
#include "memory/memory.h"
#include "screen/screen.h"
#include "keyboard/keyboard.h"
#include "security/security.h"
#include "vfs/vfs.h"
#include "process/process.h"

/* ── Background task demos ── */

static int counter_ticks = 0;

void task_counter() {
    while (1) {
        counter_ticks++;
        if (counter_ticks % 500 == 0) {
            print_string("\r\n[counter] ticks: ");
            print_string(int_to_str(counter_ticks));
            print_string("\r\nmini-os >> ");
        }
        process_yield();
    }
}

void task_ping() {
    for (int i = 1; i <= 5; i++) {
        print_string("\r\n[ping] pong ");
        print_string(int_to_str(i));
        print_string("\r\nmini-os >> ");
        for (int j = 0; j < 200; j++) process_yield();
    }
    print_string("\r\n[ping] done\r\nmini-os >> ");
}

/* ── Shell ── */

void show_boot_screen() {
    clear_screen();
    move_cursor(1, 1);
    print_string("========================================\n");
    print_string("         MINI OS v1.0 - PHASE 1        \n");
    print_string("========================================\n");
    print_string("Status: System Booting...\n");
    print_string("Memory: Initializing 1MB Virtual RAM...\n");
    print_string("Kernel: Loading Custom Libraries...\n");
    print_string("Shell:  Command Prompt Ready.\n");
    print_string("----------------------------------------\n");
    print_string("Type 'help' for available commands.\n");
}

int main() {
    init_memory();
    vfs_init();
    process_init();
    scheduler_init();
    show_boot_screen();

    while (1) {
        print_string("\nmini-os >> ");

        char* input = read_line_bg();
        if (str_len(input) == 0) continue;

        if (!validate_input(input)) {
            print_string("Security Alert: Invalid or malicious input detected!\n");
            goto next_cmd;
        }

        int arg_count = 0;
        char** args = str_split(input, ' ', &arg_count);
        char* cmd = args[0];

        // ---------------- HELP ----------------
        if (str_compare(cmd, "help")) {
            print_string("Available Commands:\n");
            print_string("  echo <text>            - Print back text\n");
            print_string("  add/sub/mul/div/mod    - Math operations (e.g. add 5 10)\n");
            print_string("  ls                     - List files in VFS\n");
            print_string("  touch <name> [text]    - Create a new file\n");
            print_string("  read <name>            - Print file contents\n");
            print_string("  write <name> <text>    - Append text to a file\n");
            print_string("  rm <name>              - Delete a file\n");
            print_string("  run <counter|ping>     - Spawn a background task\n");
            print_string("  ps                     - List running processes\n");
            print_string("  kill <pid>             - Kill a background process\n");
            print_string("  status                 - Show system/memory status\n");
            print_string("  clear                  - Clear the screen\n");
            print_string("  exit                   - Shutdown OS\n");
        }

        // ---------------- ECHO ----------------
        else if (str_compare(cmd, "echo")) {
            for (int i = 1; i < arg_count; i++) {
                print_string(args[i]);
                print_string(" ");
            }
            print_string("\n");
        }

        // ---------------- MATH OPERATIONS ----------------
        else if (str_compare(cmd, "add") || str_compare(cmd, "sub") ||
                 str_compare(cmd, "mul") || str_compare(cmd, "div") ||
                 str_compare(cmd, "mod")) {

            if (arg_count < 3) {
                print_string("Error: Missing arguments. Use: <cmd> <a> <b> [c] ...\n");
                goto next_cmd;
            }

            if (!is_numeric(args[1])) {
                print_string("Error: Argument 1 is not a valid number.\n");
                goto next_cmd;
            }

            int res = str_to_int(args[1]);

            for (int i = 2; i < arg_count; i++) {

                if (!is_numeric(args[i])) {
                    print_string("Error: Argument ");
                    print_string(int_to_str(i));
                    print_string(" is not a valid number.\n");
                    goto next_cmd;
                }

                int next_val = str_to_int(args[i]);

                if (str_compare(cmd, "add")) {
                    res = add(res, next_val);
                }
                else if (str_compare(cmd, "sub")) {
                    res = subtract(res, next_val);
                }
                else if (str_compare(cmd, "mul")) {
                    res = multiply(res, next_val);
                }
                else if (str_compare(cmd, "div")) {
                    if (next_val == 0) {
                        print_string("Error: Division by zero at argument ");
                        print_string(int_to_str(i));
                        print_string("\n");
                        goto next_cmd;
                    }
                    res = divide(res, next_val);
                }
                else if (str_compare(cmd, "mod")) {
                    if (next_val == 0) {
                        print_string("Error: Modulo by zero at argument ");
                        print_string(int_to_str(i));
                        print_string("\n");
                        goto next_cmd;
                    }
                    res = mod(res, next_val);
                }
            }

            print_string("Result: ");
            print_string(int_to_str(res));
            print_string("\n");
        }

        // ---------------- VFS COMMANDS ----------------
        else if (str_compare(cmd, "ls")) {
            vfs_list();
        }
        else if (str_compare(cmd, "touch") || str_compare(cmd, "write")) {
            if (arg_count < 2) {
                print_string("Error: Missing file name.\n");
                goto next_cmd;
            }
            char* name = args[1];

            int content_len = 0;
            for (int i = 2; i < arg_count; i++)
                content_len += str_len(args[i]) + 1;
            if (content_len == 0) content_len = 1;

            char* content = (char*)alloc(content_len);
            if (!content) {
                print_string("Error: Out of memory.\n");
                goto next_cmd;
            }

            int pos = 0;
            for (int i = 2; i < arg_count; i++) {
                int len = str_len(args[i]);
                for (int k = 0; k < len; k++)
                    content[pos++] = args[i][k];
                if (i < arg_count - 1) content[pos++] = ' ';
            }
            content[pos] = '\0';

            if (str_compare(cmd, "touch")) {
                vfs_create(name, content);
            } else {
                vfs_update(name, content);
            }
            dealloc(content);
        }
        else if (str_compare(cmd, "read")) {
            if (arg_count < 2) {
                print_string("Error: Missing file name.\n");
                goto next_cmd;
            }
            vfs_read(args[1]);
        }
        else if (str_compare(cmd, "rm")) {
            if (arg_count < 2) {
                print_string("Error: Missing file name.\n");
                goto next_cmd;
            }
            vfs_delete(args[1]);
        }

        // ---------------- RUN ----------------
        else if (str_compare(cmd, "run")) {
            if (arg_count < 2) {
                print_string("Usage: run <counter|ping>\n");
            } else if (str_compare(args[1], "counter")) {
                int pid = process_spawn("counter", task_counter);
                if (pid < 0) {
                    print_string("Error: process table full\n");
                } else {
                    print_string("Spawned counter [pid ");
                    print_string(int_to_str(pid));
                    print_string("]\n");
                }
            } else if (str_compare(args[1], "ping")) {
                int pid = process_spawn("ping", task_ping);
                if (pid < 0) {
                    print_string("Error: process table full\n");
                } else {
                    print_string("Spawned ping [pid ");
                    print_string(int_to_str(pid));
                    print_string("]\n");
                }
            } else {
                print_string("Unknown task. Available: counter, ping\n");
            }
        }

        // ---------------- PS ----------------
        else if (str_compare(cmd, "ps")) {
            process_print_all();
        }

        // ---------------- KILL ----------------
        else if (str_compare(cmd, "kill")) {
            if (arg_count < 2 || !is_numeric(args[1])) {
                print_string("Usage: kill <pid>\n");
            } else {
                int pid = str_to_int(args[1]);
                process_kill(pid);
                print_string("Killed process ");
                print_string(int_to_str(pid));
                print_string("\n");
            }
        }

        // ---------------- STATUS ----------------
        else if (str_compare(cmd, "status")) {
            print_string("System Status:\n");
            print_string("  OS: Mini OS v1.0\n");
            print_string("  Memory Total:     1048576 bytes\n");
            print_string("  Memory Used:      ");
            print_string(int_to_str(mem_used()));
            print_string(" bytes\n");
            print_string("  Memory Available: ");
            print_string(int_to_str(mem_available()));
            print_string(" bytes\n");
            if (mem_overflow()) {
                print_string("  WARNING: Last allocation failed (memory overflow)\n");
            }
            print_string("  Storage: VFS Active\n");
        }

        // ---------------- CLEAR ----------------
        else if (str_compare(cmd, "clear")) {
            clear_screen();
            show_boot_screen();
        }

        // ---------------- EXIT ----------------
        else if (str_compare(cmd, "exit")) {
            print_string("Shutting down Mini OS...\n");
            break;
        }

        // ---------------- UNKNOWN ----------------
        else {
            print_string("Unknown command: ");
            print_string(cmd);
            print_string("\nType 'help' for list of commands.\n");
        }

    next_cmd:
        dealloc(input);
    }

    return 0;
}
