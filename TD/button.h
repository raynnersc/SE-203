#ifndef BUTTON_H
#define BUTTON_H
#define STM32L475xx

#include <stdint.h>
#include <stm32l4xx.h>
#include "led.h"
#include "irq.h"

void button_init();

#endif // BUTTON_H