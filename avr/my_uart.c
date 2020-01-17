/*
 * my_uart.c
 *
 * Created: 14-Jan-20 10:24:49 PM
 *  Author: Zac
 */ 
#ifndef MY_UART_H_
#define MY_UART_H_

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#define UART_IS_CLEAR (UCSR0A & (1U << UDRE0))
#define UART_DATA     UDR0

#include "my_uart.h"
#include <avr/io.h>
#include <stdint.h>


void init_uart(uint16_t baudrate)
{
	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	/* Write upper bit of baudrate to register */
	UBRR0H = UBRR_val >> 8;
	
	/* Write lower bit of baudrate to register */
	UBRR0L = UBRR_val;

	/* Enable transmitter, receiver, and receiver interrupt */
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);  
	
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); 
}

void uart_putc(unsigned char c)
{
	while(!UART_IS_CLEAR); /* Wait until sending is possible */
	UART_DATA = c;         /* Output character saved in c */
}

void uart_puts(char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

void serial_print(char *s)
{
	uart_puts(s);
}
	
void serial_println(char *s)
{
	uart_puts(s);
}

#endif /* MY_UART_H_ */