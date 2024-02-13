#include "led.h"

void led_init()
{
    // Activate GPIOB and GPIOC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
    // Define the IOs
    GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE14_1)) | GPIO_MODER_MODE14_0;
    GPIOC->MODER &= ~GPIO_MODER_MODE9;
}

void led_g_on()
{
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);
}

void led_g_off()
{
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);
}

void toogle_led_g()
{
    if (GPIOB->ODR & GPIO_ODR_OD14)
        led_g_off();
    else
        led_g_on();
}

void led(State state)
{
    switch (state)
    {
    case LED_YELLOW:
        GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE9_1) | GPIO_MODER_MODE9_0;
        GPIOC->BSRR = GPIO_BSRR_BS9;
        break;
    case LED_BLUE:
        GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE9_1) | GPIO_MODER_MODE9_0;
        GPIOC->BSRR = GPIO_BSRR_BR9;
        break;
    default:
        GPIOC->MODER &= ~GPIO_MODER_MODE9;
        break;
    }
}

void test_led()
{
    led(LED_OFF);
    led_g_on();
    for (int i = 0; i < 200000 * 20; i++)
        asm volatile("nop");
    led_g_off();
    led(LED_YELLOW);
    for (int i = 0; i < 200000 * 20; i++)
        asm volatile("nop");
    led_g_off();
    led(LED_BLUE);
    for (int i = 0; i < 200000 * 20; i++)
        asm volatile("nop");
}