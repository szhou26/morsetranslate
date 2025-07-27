/*
 * uart.c
 *
 * This file uses parts of the ECE 3140 / CS 3420  UART0 Example code
 *
 * This UART is configured to print to the virtual serial port that is
 * created by the USB Debug cable.
 *
 */

#include <pin_mux.h>
#include <clock_config.h>
#include <stdio.h>
#include <board.h>
#include <MKL46Z4.h>
#include <fsl_debug_console.h>

#include "uart.h"

/*------------------------------*/
/* UART communication functions */
/*------------------------------*/
/* Initialize the UART for TX (115200, 8N1) */

void init_uart(void)
{
	//Use UART port through debug interface
	// Connect to UART with TX (115200, 8N1)

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
 	BOARD_InitDebugConsole();
}

void uart_putc (char ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART0->S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART0->D = (uint8_t)ch;
}

void uart_puts(char *ptr_str)
{
    while(*ptr_str){
			/* Replace newlines with \r\n carriage return */
			if(*ptr_str == '\n') { uart_putc('\r'); }
      uart_putc(*ptr_str++);
		}
}

void short_delay()
{
	for(int i=1000000; i>0; i--){}
}
