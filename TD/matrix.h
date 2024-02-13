#ifndef MATRIX_H
#define MATRIX_H
#define STM32L475xx

#include <stm32l4xx.h>
#include <stdint.h>

#define SYSCLK 80000000

/*
    RST	PC3
    SB	PC5
    LAT	PC4
    SCK	PB1
    SDA	PA4
    C0	PB2
    C1	PA15
    C2	PA2
    C3	PA7
    C4	PA6
    C5	PA5
    C6	PB0
    C7	PA3
*/

#define RST(output) GPIOC->BSRR = (output ? GPIO_BSRR_BS3 : GPIO_BSRR_BR3)
#define SB(output) GPIOC->BSRR = (output ? GPIO_BSRR_BS5 : GPIO_BSRR_BR5)
#define LAT(output) GPIOC->BSRR = (output ? GPIO_BSRR_BS4 : GPIO_BSRR_BR4)
#define SCK(output) GPIOB->BSRR = (output ? GPIO_BSRR_BS1 : GPIO_BSRR_BR1)
#define SDA(output) GPIOA->BSRR = (output ? GPIO_BSRR_BS4 : GPIO_BSRR_BR4)

void ROW0(int output);
void ROW1(int output);
void ROW2(int output);
void ROW3(int output);
void ROW4(int output);
void ROW5(int output);
void ROW6(int output);
void ROW7(int output);

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

typedef enum
{
    BLUE,
    GREEN,
    RED
} Color;

extern volatile uint8_t canUpdate;

void matrix_init();
void pulse_SCK();
void pulse_LAT();
void deactivate_rows();
void activate_row(int row);
void send_byte(uint8_t val, int bank);
void mat_set_row(int row, const rgb_color *val);
void init_bank0();
void test_pixels();
void test_image();

#endif // MATRIX_H