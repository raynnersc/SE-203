#include "uart.h"

const uint32_t FreqSysClk = 80000000;

void uart_init(unsigned int baudrate)
{
    // Disable the USART for configuration
    USART1->CR1 &= ~USART_CR1_UE;

    // Activate the GPIOB and USART1 clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Specify PCLK as the clock to be used by USART1
    RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;

    // Reset the USART1
    RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;

    // Define PORTB IO as RX/TX
    GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0)) 
                    | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
    GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(GPIO_AFRL_AFSEL7_3 | GPIO_AFRL_AFSEL6_3)) 
                    | GPIO_AFRL_AFSEL7_2 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_0 
                    | GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_0;

    // Define the USART speed as 115200 bps 8N1
    /* USART1->CR1 = (USART1->CR1 & ~(USART_CR1_OVER8 | USART_CR1_M | USART_CR1_PCE)) | USART_CR1_TE | USART_CR1_RE; */
    USART1->CR1 = (USART1->CR1 & ~(USART_CR1_OVER8 | USART_CR1_M | USART_CR1_PCE));
    /* USART2->CR2 = 0x00000000; */
    USART1->CR2 &= ~USART_CR2_STOP;
    USART1->BRR = (uint32_t) FreqSysClk/baudrate;

    // Enable the USART and Interruption for USART Reception
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

    // Enable USART1 interruption handler
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart_putchar(uint8_t c)
{
    while (!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR = c;
}

uint8_t uart_getchar()
{
    while (!(USART1->ISR & USART_ISR_RXNE));
    return USART1->RDR;
}

void uart_puts(const char *s)
{
    while (*s)
        uart_putchar((uint8_t)*s++);

    uart_putchar((uint8_t)'\r');
    uart_putchar((uint8_t)'\n');
}

void uart_gets(char *s, size_t size)
{
    for (; (--size) && (*s != '\n'); s++)
    {
        *s = (char)uart_getchar();
        if (USART1->ISR & (USART_ISR_ORE | USART_ISR_FE))
            while (1);
    }
    *s = '\0';
}

volatile uint32_t checksumVar;
void checksum()
{
    checksumVar = 0;
    for(;;)
        checksumVar = checksumVar + uart_getchar();
}

uint8_t frames[192];
void USART1_IRQHandler()
{
    static uint8_t index = 0;
    uint8_t byte = uart_getchar();

    if(USART1->ISR & (USART_ISR_ORE | USART_ISR_FE))
    {
        USART1->ICR = USART_ICR_ORECF | USART_ICR_FECF;
        USART1->RQR = USART_RQR_RXFRQ;
        return;
    }

    if (byte == 0xFF)
    {
        index = 0;
        return;
    }
    
    frames[index++] = byte; 

    if(index == 192)
        index = 0;
}