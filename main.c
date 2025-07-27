/*
 * main.c
 *
 * Morse Code Input System for FRDM-KL46Z
 *
 * Users input Morse Code sequences by pressing SW1.
 * Users can submit the full sentence or clear the terminal by pressing SW3.
 *
 * Live representations of dots and dashes are printed as they are entered.
 * Decoded letters are accumulated into a sentence. Word boundaries insert spaces.
 * The full sentence is displayed after a timeout/long pause.
 */

#include <pin_mux.h>
#include <clock_config.h>
#include <stdio.h>
#include <board.h>
#include <MKL46Z4.h>
#include <fsl_debug_console.h>

#include "buttons.h"
#include "decoding.h"
#include "uart.h"

// Morse timing thresholds (in milliseconds)
#define UNIT_MS 300
#define DOT_THRESHOLD_MS (UNIT_MS)
#define DASH_THRESHOLD_MS (3 * UNIT_MS)
#define LETTER_SPACE_MS (3 * UNIT_MS)
#define WORD_SPACE_MS (7 * UNIT_MS)
#define SENTENCE_TIMEOUT (10 * UNIT_MS)  // sentence timeout , will decode after this

// Buffer sizes
#define MAX_MORSE_LEN 6
#define MAX_SENTENCE_LEN 100

volatile uint32_t tick = 0; // global millisecond counter

// SysTick interrupt handler, increments every 1ms
void SysTick_Handler(void) {
    tick++;
}

// Main entry point for Morse code input and decoding
int main(void) {
    // Initialize UART, buttons, and SysTick
    init_uart();
    init_sw();
    SysTick_Config(SystemCoreClock / 1000);

    uart_puts("Ready for input.\n");

    // Buffers for Morse and sentence
    char morse[MAX_MORSE_LEN + 1] = {0};
    int morse_idx = 0;

    char sentence[MAX_SENTENCE_LEN + 1] = {0};
    int sent_idx = 0;

    uint32_t last_release = 0; // time of last SW1 release

    while (1) {
        uint32_t idle_time = tick - last_release;

        // pressing SW3 clears the terminal
        if (is_button3_pressed()) {
            uart_puts("\033[2J\033[H");   // escape characters

            // clear the buffer
            morse_idx = 0;
            sent_idx = 0;
            morse[0] = '\0';
            sentence[0] = '\0';
            last_release = tick;  // reset tracking
            while (is_button3_pressed());
        }

        // pressing SW1 inputs Morse Code
        // calculate time button is pressed
        if (is_button1_pressed()) {
            uint32_t press_start = tick;
            while (is_button1_pressed());
            uint32_t duration = tick - press_start;

            // determine if dot or dash, print and put into buffer for Morse character
            if (duration < DOT_THRESHOLD_MS) {
                if (morse_idx < MAX_MORSE_LEN) {
                    morse[morse_idx++] = '.';
                    uart_putc('.');
                }
            } else if (duration < DASH_THRESHOLD_MS) {
                if (morse_idx < MAX_MORSE_LEN) {
                    morse[morse_idx++] = '-';
                    uart_putc('-');
                }
            }

            morse[morse_idx] = '\0';
            last_release = tick;
        }

        uint32_t since_last_release = tick - last_release;
        // Decode letter after 3 units of inactivity
        if (morse_idx > 0 && since_last_release >= LETTER_SPACE_MS) {
            char decoded = decode_morse(morse);
            if (sent_idx < MAX_SENTENCE_LEN) {
                sentence[sent_idx++] = decoded;
                sentence[sent_idx] = '\0';
                uart_putc(' ');
            }

            morse_idx = 0;
            morse[0] = '\0';
        }

        // if 7 units --> space between words
        if (sent_idx > 0 && idle_time >= WORD_SPACE_MS) {
            if (sentence[sent_idx - 1] != ' ' && sent_idx < MAX_SENTENCE_LEN) {
                sentence[sent_idx++] = ' ';
                sentence[sent_idx] = '\0';
                uart_putc(' ');
            }
        }

        // after 10 units, decode word (end of sentence)
        if (sent_idx > 0 && idle_time >= SENTENCE_TIMEOUT) {
            uart_putc('\n');
            uart_putc('\r');
            uart_puts(sentence);
            uart_putc('\n');
            uart_putc('\r');
            sent_idx = 0;
            sentence[0] = '\0';
        }
    }
    return 0;
}


