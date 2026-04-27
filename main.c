#include "math/math.h"
#include "string/string.h"
#include "memory/memory.h"
#include "screen/screen.h"
#include "keyboard/keyboard.h"
#include "security/security.h"

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
    show_boot_screen();

    while (1) {
        print_string("\nmini-os >> ");

        char* input = read_line();
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
            print_string("  echo <text>      - Print back text\n");
            print_string("  add/sub/mul/div/mod - Math operations (e.g. sub 10 5)\n");
            print_string("  status              - Show system status\n");
            print_string("  clear               - Clear the screen\n");
            print_string("  exit                - Shutdown OS\n");
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

        // ---------------- STATUS ----------------
        else if (str_compare(cmd, "status")) {
            print_string("System Status:\n");
            print_string("  OS: Mini OS v1.0\n");
            print_string("  Memory: Virtual RAM Active\n");
            print_string("  Storage: VFS Initialized (Empty)\n");
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