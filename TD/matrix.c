#include "matrix.h"

static void sleep(uint32_t time)
{
    for (uint32_t i = 0; i < time; i++)
        asm volatile("nop");
}

static void (*switch_row[])(int) = {ROW0, ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7};

void matrix_init()
{
    // Activate the GPIOA, GPIOB and GPIOC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

    // Define PORTx IO as OUTPUT
    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1 | GPIO_MODER_MODE15_1)) | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE15_0;
    GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1 | GPIO_MODER_MODE2_1)) | GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0;
    GPIOC->MODER = (GPIOC->MODER & ~(GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1)) | GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0;

    // Define the outputs as high-speed outputs
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR15;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5;

    // Initial value
    RST(0);
    LAT(1);
    SB(1);
    SCK(0);
    SDA(0);
    ROW0(0);
    ROW1(0);
    ROW2(0);
    ROW3(0);
    ROW4(0);
    ROW5(0);
    ROW6(0);
    ROW7(0);

    // Wait 100ms to clean the reset bit
    sleep(8000000);

    // Clean the reset bit
    RST(1);

    // Set all BANK0's bits
    init_bank0();
}

void pulse_SCK()
{
    SCK(0);
    sleep(30);
    SCK(1);
    sleep(30);
    SCK(0);
}

void pulse_LAT()
{
    LAT(1);
    sleep(30);
    LAT(0);
    sleep(30);
    LAT(1);
}

void deactivate_rows()
{
    ROW0(0);
    ROW1(0);
    ROW2(0);
    ROW3(0);
    ROW4(0);
    ROW5(0);
    ROW6(0);
    ROW7(0);
}

void activate_row(int row)
{
    switch_row[row](1);
}

void send_byte(uint8_t val, int bank)
{
    SB(bank);
    const uint8_t nBits = bank ? 8 : 6;
    for (int8_t i = nBits - 1; i >= 0; i--)
    {
        SDA(val & (1 << i));
        pulse_SCK();
    }
}

void mat_set_row(int row, const rgb_color *val)
{
    for (uint8_t i = 8; i > 0; i--)
    {
        send_byte(val[i - 1].b, 1);
        send_byte(val[i - 1].g, 1);
        send_byte(val[i - 1].r, 1);
    }
    deactivate_rows();
    sleep(100);
    pulse_LAT();
    activate_row(row);
}

void init_bank0()
{
    for (uint8_t i = 0; i < 24; i++)
        send_byte(0xFF, 0);
    pulse_LAT();
}

static Color setRGB(rgb_color *LED, Color color)
{
    switch (color)
    {
    case BLUE:
        for (uint8_t i = 0; i < 8; i++)
        {
            LED[i].r = ((256 / (i + 1)) - 1);
            LED[i].g = ((256 / (i + 1)) - 1);
            LED[i].b = 255;
        }
        color = GREEN;
        break;
    case GREEN:
        for (uint8_t i = 0; i < 8; i++)
        {
            LED[i].r = ((256 / (i + 1)) - 1);
            LED[i].g = 255;
            LED[i].b = ((256 / (i + 1)) - 1);
        }
        color = RED;
        break;
    case RED:
        for (uint8_t i = 0; i < 8; i++)
        {
            LED[i].r = 255;
            LED[i].g = ((256 / (i + 1)) - 1);
            LED[i].b = ((256 / (i + 1)) - 1);
        }
        color = BLUE;
        break;
    default:
        break;
    }
    return color;
}

void test_pixels()
{
    rgb_color LED[8];
    Color color = BLUE;
    for (;;)
    {
        color = setRGB(LED, color);
        for (uint8_t i = 0; i < 8; i++)
        {
            mat_set_row(i, LED);
            sleep(1000000);
        }
    }
}

volatile uint8_t canUpdate = 0;
void test_image(const uint8_t *byte_start)
{
    rgb_color LED[8];

    for(;;)
    {
        deactivate_rows();
        if(canUpdate){
            for (uint8_t row = 0; row < 8; row++) {
                for (uint8_t i = 0; i < 8; i++)
                {
                    LED[i].r = *byte_start++;
                    LED[i].g = *byte_start++;
                    LED[i].b = *byte_start++;
                }
                mat_set_row(row, LED);
                sleep(5000);
            }
            byte_start -= 192;
            canUpdate = 0;
        }
    }
}

void ROW0(int output) { GPIOB->BSRR = (output ? GPIO_BSRR_BS2 : GPIO_BSRR_BR2); }
void ROW1(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS15 : GPIO_BSRR_BR15); }
void ROW2(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS2 : GPIO_BSRR_BR2); }
void ROW3(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS7 : GPIO_BSRR_BR7); }
void ROW4(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS6 : GPIO_BSRR_BR6); }
void ROW5(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS5 : GPIO_BSRR_BR5); }
void ROW6(int output) { GPIOB->BSRR = (output ? GPIO_BSRR_BS0 : GPIO_BSRR_BR0); }
void ROW7(int output) { GPIOA->BSRR = (output ? GPIO_BSRR_BS3 : GPIO_BSRR_BR3); }