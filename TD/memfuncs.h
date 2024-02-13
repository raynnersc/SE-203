#ifndef MEMFUNCS_H
#define MEMFUNCS_H

#include <stdint.h>
#include <unistd.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif // MEMFUNCS_H
