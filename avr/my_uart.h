/*
 * my_uart.h
 *
 * Created: 14-Jan-20 10:22:49 PM
 *  Author: Zac
 */ 

#ifndef MY_UART_H_
#define MY_UART_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <avr/io.h>
#include <stdint.h>

void init_uart      (uint16_t baudrate);
void uart_putc      (unsigned char c);
void uart_puts      (char *s);
void serial_print   (char *s);
void serial_println (char *s);

#ifdef __cplusplus
}
#endif

#endif /* MY_UART_H_ */