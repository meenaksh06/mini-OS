#include "math/math.h"
#include "string/string.h"
#include "memory/memory.h"
#include "screen/screen.h"
#include "keyboard/keyboard.h"

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

        int arg_count = 0;
        char** args = str_split(input, ' ', &arg_count);
        char* cmd = args[0];

        if (str_compare(cmd, "help")) {
            print_string("Available Commands:\n");
            print_string("  echo <text>      - Print back text\n");
            print_string("  add/mul/div/mod  - Math operations (e.g. add 5 10)\n");
            print_string("  status           - Show memory status\n");
            print_string("  clear            - Clear the screen\n");
            print_string("  exit             - Shutdown OS\n");
        } 
        else if (str_compare(cmd, "echo")) {
            for (int i = 1; i < arg_count; i++) {
                print_string(args[i]);
                print_string(" ");
            }
            print_string("\n");
        }
        else if (str_compare(cmd, "add") || str_compare(cmd, "mul") || str_compare(cmd, "div") || str_compare(cmd, "mod")) {
            if (arg_count < 3) {
                print_string("Error: Missing arguments. Use: <cmd> <a> <b>\n");
            } else {
                int a = str_to_int(args[1]);
                int b = str_to_int(args[2]);
                int res = 0;
                if (str_compare(cmd, "add")) res = add(a, b);
                if (str_compare(cmd, "mul")) res = multiply(a, b);
                if (str_compare(cmd, "div")) res = divide(a, b);
                if (str_compare(cmd, "mod")) res = mod(a, b);
                
                print_string("Result: ");
                print_string(int_to_str(res));
                print_string("\n");
            }
        }
        else if (str_compare(cmd, "status")) {
            print_string("System Status:\n");
            print_string("  OS: Mini OS v1.0\n");
            print_string("  Memory: Virtual RAM Active\n");
            print_string("  Storage: VFS Initialized (Empty)\n");
        }
        else if (str_compare(cmd, "clear")) {
            clear_screen();
            show_boot_screen();
        }
        else if (str_compare(cmd, "exit")) {
            print_string("Shutting down Mini OS...\n");
            break;
        }
        else {
            print_string("Unknown command: ");
            print_string(cmd);
            print_string("\nType 'help' for list of commands.\n");
        }

        // Simple cleanup for Phase 1 simulation (real dealloc in Phase 2)
        dealloc(input);
    }

    return 0;
}