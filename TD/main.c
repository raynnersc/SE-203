#include "led.h"
#include "clocks.h"
#include "uart.h"
#include "matrix.h"
#include "irq.h"
#include "button.h"
#include "timer.h"

extern uint8_t _binary_image_raw_start;
extern uint8_t frames[192];

int __attribute__((section(".text"))) main()
{
	clocks_init();
	led_init();
	uart_init(38400);
	matrix_init();
	irq_init();
	button_init();

	timer_init(16667);
	test_image(&frames);

	while (1);

	return 0;
}
