#include "math/math.h"
#include "string/string.h"
#include "memory/memory.h"
#include "screen/screen.h"
#include "keyboard/keyboard.h"

int main() {
    init_memory();
    clear_screen();

    print_string("Mini OS Started\n");

    while (1) {
        print_string("\n>> ");

        char* input = read_line();

        char cmd[10];
        char a_str[10];
        char b_str[10];

        int i = 0, j = 0;

        while (input[i] != ' ' && input[i] != '\0') {
            cmd[j++] = input[i++];
        }
        cmd[j] = '\0';
        i++;

        j = 0;
        while (input[i] != ' ' && input[i] != '\0') {
            a_str[j++] = input[i++];
        }
        a_str[j] = '\0';
        i++;

        j = 0;
        while (input[i] != '\0') {
            b_str[j++] = input[i++];
        }
        b_str[j] = '\0';

        int a = str_to_int(a_str);
        int b = str_to_int(b_str);

        int result = 0;

        if (str_compare(cmd, "add")) {
            result = add(a, b);
        } else if (str_compare(cmd, "mul")) {
            result = multiply(a, b);
        } else if (str_compare(cmd, "div")) {
            result = divide(a, b);
        } else if (str_compare(cmd, "mod")) {
            result = mod(a, b);
        } else {
            print_string("Unknown command\n");
            continue;
        }

        char* result_str = int_to_str(result);
        print_string("Result: ");
        print_string(result_str);
        print_string("\n");
    }

    return 0;
}