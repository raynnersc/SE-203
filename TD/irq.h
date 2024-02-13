#ifndef IRQ_H
#define IRQ_H
#define STM32L475xx

#include <stdint.h>
#include <stm32l4xx.h>
#include <core_cm4.h>

#define MAKE_DEFAULT_HANDLER(IRQ_Handler)               \
    void __attribute__((weak)) IRQ_Handler(void) {      \
        __disable_irq();                                \
        for(;;);                                        \
    }                                                   \

extern uint8_t* _stack_start;    /* Top of stack  (initial value of the SP register) */

void irq_init();

#endif // IRQ_H