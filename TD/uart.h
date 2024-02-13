#ifndef UART_H
#define UART_H
#define STM32L475xx

#include <stm32l4xx.h>
#include <unistd.h>
#include <core_cm4.h>
#include "irq.h"

extern uint8_t frames[192];

void uart_init(unsigned int baudrate);
void uart_putchar(uint8_t c);
uint8_t uart_getchar();
void uart_puts(const char *s);
void uart_gets(char *s, size_t size);
void checksum();

#endif // UART_H