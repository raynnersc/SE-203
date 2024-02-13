#include "button.h"
#include "irq.h"

void button_init()
{
    // Activate the GPIOC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Define PORTC IO as INPUT
    GPIOC->MODER &= ~GPIO_MODER_MODE13;

    // Define PC13 as source of IRQ
    SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~SYSCFG_EXTICR4_EXTI13) | SYSCFG_EXTICR4_EXTI13_PC;

    // Configure EXTI
    EXTI->IMR1 |= EXTI_IMR1_IM13;
    EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
    EXTI->FTSR1 |= EXTI_FTSR1_FT13;

    // Activate the IRQ
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler()
{
    toogle_led_g();
    EXTI->PR1 |= EXTI_PR1_PIF13;
}