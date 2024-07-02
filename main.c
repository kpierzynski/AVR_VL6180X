#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "vl6180x.h"

int main()
{
	// Delay for humans
	_delay_ms(2000);

	// Debug led
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);

	i2c_init();
	uart_init();

	vl6180x_init();

	sei();
	uart_puts("\r\nSTARTING...\r..\n");

	while (1)
	{
		_delay_ms(1000);
		uint8_t mm = vl6180x_measure();
		uart_puts("\r\nValue: ");
		uart_putd(mm);
		uart_puts("mm.\r\n");
	}

	return -1;
}
