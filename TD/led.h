#ifndef LED_H
#define LED_H
#define STM32L475xx

#include <stdint.h>
#include <stm32l4xx.h>

typedef enum
{
    LED_OFF,
    LED_YELLOW,
    LED_BLUE
} State;

void led_init();
void led_g_on();
void led_g_off();
void led(State state);
void test_led();
void toogle_led_g();

#endif // LED_H