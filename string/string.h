#ifndef STRING_H
#define STRING_H

int str_len(char* str);
void str_copy(char* dest, char* src);
int str_compare(char* a, char* b);
int str_to_int(char* str);
char* int_to_str(int num);
char** str_split(char* str, char delim, int* count);
int str_distance(char* a, char* b);
int str_contains(char* haystack, char* needle);
void str_to_lower(char* str);

#endif