#ifndef _STDLIB_H
#define _STDLIB_H

#include <stdlibc/include/stdint.h>

size_t strlen(const char* str);
char* itoa(int num, char* str, int base);
void reverse(char str[], int length);
void* memset(void* start_addr, char value, int32_t size);

#endif // _STDLIB_H
