#ifndef VFS_H
#define VFS_H

typedef struct {
    char name[32];
    char* content;
    int size;
    int is_used;
} File;

void vfs_init();
void vfs_create(char* name, char* content);
void vfs_read(char* name);
void vfs_update(char* name, char* added_content);
void vfs_delete(char* name);
void vfs_list();

#endif
