#include "memfuncs.h"

void __attribute__((section(".bootloader"))) *memcpy(void *dst, const void *src, size_t num)
{
    uint8_t *_dst = dst;
    const uint8_t *_src = src;

    for (size_t i = 0; i < num; i++)
        _dst[i] = _src[i];

    return dst;
}

void __attribute__((section(".bootloader"))) *memmove(void *dst, const void *src, size_t num)
{
    uint8_t *_dst = dst;
    const uint8_t *_src = src;

    if (_dst < _src)
    {
        for (size_t i = 0; i < num; i++)
            _dst[i] = _src[i];
    }
    else
    {
        for (size_t i = num; i > 0; i--)
            _dst[i - 1] = _src[i - 1];
    }

    return dst;
}

void __attribute__((section(".bootloader"))) *memset(void *ptr, int value, size_t num)
{
    uint8_t *_ptr = ptr;

    for (size_t i = 0; i < num; i++)
        _ptr[i] = (uint8_t) value;

    return ptr;
}

int __attribute__((section(".bootloader"))) memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const uint8_t *_ptr1 = ptr1;
    const uint8_t *_ptr2 = ptr2;

    for (size_t i = 0; i < num; i++)
    {
        if (_ptr1[i] > _ptr2[i])
            return 1;
        else if (_ptr1[i] < _ptr2[i])
            return -1;
    }

    return 0;
}