#ifndef STRING_H
#define STRING_H

int str_len(char* str);
void str_copy(char* dest, char* src);
int str_compare(char* a, char* b);
int str_to_int(char* str);
char* int_to_str(int num);

#endif