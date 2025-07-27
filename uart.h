/*
 * uart.h
 *
 *  Created on: May 11, 2025
 *      Author: cshel
 */
#ifndef UART_H_
#define UART_H_

void init_uart(void);
void uart_putc (char ch);
void uart_puts(char *ptr_str);
void short_delay();

#endif /* UART_H_ */
