#include "security.h"
#include "../string/string.h"

#define MAX_INPUT_LEN 100

// Returns 1 if input is safe, 0 if it contains violations
int validate_input(char* str) {
    if (str == 0) return 0;
    
    int len = str_len(str);
    
    // Check for buffer overflow attempt
    if (len > MAX_INPUT_LEN) {
        return 0;
    }
    
    // Check for weird non-printable characters
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (c < 32 || c > 126) {
            // allows standard printable ascii (space to tilde)
            // returning 0 because we don't want weird control chars
            return 0;
        }
    }
    
    return 1;
}

// Returns 1 if string only contains digits, optionally starting with a minus sign
int is_numeric(char* str) {
    if (str == 0) return 0;
    
    int len = str_len(str);
    if (len == 0) return 0;
    
    int start = 0;
    if (str[0] == '-') {
        if (len == 1) return 0; // Just a minus sign is not a number
        start = 1;
    }
    
    for (int i = start; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    
    return 1;
}
