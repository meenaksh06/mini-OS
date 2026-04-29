#include "vfs.h"
#include "../memory/memory.h"
#include "../string/string.h"
#include "../screen/screen.h"

#define MAX_FILES 10

static File fs[MAX_FILES];

void vfs_init() {
    for (int i = 0; i < MAX_FILES; i++) {
        fs[i].is_used = 0;
        fs[i].size = 0;
    }
}

void vfs_create(char* name, char* content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_used && str_compare(fs[i].name, name)) {
            print_string("Error: File already exists.\n");
            return;
        }
    }

    int slot = -1;

    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs[i].is_used) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        print_string("Error: VFS is full.\n");
        return;
    }

    str_copy(fs[slot].name, name);
    int c_len = str_len(content);
    
    fs[slot].content = (char*) alloc(c_len + 1);
    if (!fs[slot].content) {
        print_string("Error: Out of memory.\n");
        return;
    }

    str_copy(fs[slot].content, content);
    fs[slot].size = c_len;
    fs[slot].is_used = 1;

    print_string("File created: ");
    print_string(name);
    print_string("\n");
}

void vfs_read(char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_used && str_compare(fs[i].name, name)) {
            print_string(fs[i].content);
            print_string("\n");
            return;
        }
    }
    print_string("Error: File not found.\n");
}

void vfs_update(char* name, char* content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_used && str_compare(fs[i].name, name)) {
            dealloc(fs[i].content);
            int c_len = str_len(content);
            fs[i].content = (char*) alloc(c_len + 1);
            if (!fs[i].content) {
                print_string("Error: Out of memory.\n");
                return;
            }
            str_copy(fs[i].content, content);
            fs[i].size = c_len;
            print_string("File updated.\n");
            return;
        }
    }
    print_string("Error: File not found.\n");
}

void vfs_delete(char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_used && str_compare(fs[i].name, name)) {
            fs[i].is_used = 0;
            dealloc(fs[i].content);
            print_string("File deleted.\n");
            return;
        }
    }
    print_string("Error: File not found.\n");
}

void vfs_list() {
    int count = 0;
    print_string("VFS Contents:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs[i].is_used) {
            print_string("  - ");
            print_string(fs[i].name);
            print_string(" (");
            print_string(int_to_str(fs[i].size));
            print_string(" bytes)\n");
            count++;
        }
    }
    if (count == 0) {
        print_string("  (Empty)\n");
    }
}
