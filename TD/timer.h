#ifndef TIMER_H
#define TIMER_H
#define STM32L475xx

#include <stdint.h>
#include <stm32l4xx.h>
#include <core_cm4.h>
#include "led.h"
#include "matrix.h"

void timer_init(uint32_t max_us);

#endif // TIMER_H