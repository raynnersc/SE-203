#include <stdint.h>
#include <unistd.h>
#include "memfuncs.h"

extern uint8_t _bss_start, _bss_end;
extern uint8_t _data_src, _data_dst_start, _data_dst_end;
extern uint8_t _irq_src, _irq_dst_start, _irq_dst_end;
extern uint8_t _text_src, _text_dst_start, _text_dst_end;

void __attribute__((section(".bootloader"))) init_bss()
{
    memset(&_bss_start, 0, &_bss_end - &_bss_start);
}

void __attribute__((section(".bootloader"))) init_data()
{
    memcpy(&_data_dst_start, &_data_src, &_data_dst_end - &_data_dst_start);
}

void __attribute__((section(".bootloader"))) init_irq()
{
    memcpy(&_irq_dst_start, &_irq_src, &_irq_dst_end - &_irq_dst_start);
}

void __attribute__((section(".bootloader"))) init_text()
{
    memcpy(&_text_dst_start, &_text_src, &_text_dst_end - &_text_dst_start);
}
