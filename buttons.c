/*
 * buttons.c
 *
 * This file configures the on board buttons.
 * 
 */
#include <MKL46Z4.h>
#include "buttons.h"
#include <stdbool.h>

const int SWITCH_1_PIN = 3;
const int SWITCH_3_PIN = 12;

// initialize switches, LEDs
void init_sw(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;  // enable clock
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_MUX(0b111); // clear PCR Mux bits for PTC3
	PORTC->PCR[SWITCH_3_PIN] &= ~PORT_PCR_MUX(0b111); // clear PCR Mux bits for PTC12

	// set up as GPIO
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_MUX(0b001);
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_MUX(0b001);

	// make inputs
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_1_PIN);
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_3_PIN);

	// turn on pull enable, enable pull-up resistor
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PE(1);
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_PE(1);

	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PS(1);
	PORTC->PCR[SWITCH_3_PIN] |= PORT_PCR_PS(1);
}

bool is_button1_pressed(void){
	return (GPIOC->PDIR & (1 << SWITCH_1_PIN)) == 0;
}
bool is_button3_pressed(void){
		return (GPIOC->PDIR & (1 << SWITCH_3_PIN)) == 0;
}

