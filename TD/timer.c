#include "timer.h"

void timer_init(uint32_t max_us)
{
    // Activate the TIM2 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    // Clean the flags
    TIM2->SR = 0;

    // Clean the counter
    TIM2->CNT = 0;

    // Configure the Timer 2
    TIM2->CR1 &= ~(TIM_CR1_CKD | TIM_CR1_CMS | TIM_CR1_DIR | TIM_CR1_OPM);

    // Define the prescale: CK_CNT = fCK_PSC / (PSC[15:0] + 1).
    TIM2->PSC = 79;

    // Define the time to be counted
    TIM2->ARR = max_us;

    // Enable update interruption
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable USART1 interruption handler
    NVIC_EnableIRQ(TIM2_IRQn);

    // Enable Timer 2
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler()
{
    TIM2->SR &= ~TIM_SR_UIF;

    // toogle_led_g();
    canUpdate = 1;
}