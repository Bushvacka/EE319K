// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on TM4C123
// Program written by: Akhil Giridhar & Troy Dutton
// Date Created: March 30, 2018
// Last Modified: 3/25/2022
// Lab number: 7


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

extern void Delay1ms(uint32_t n);

uint32_t heartBeat;
//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
	// PORTF Clock
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;
	delay = SYSCTL_RCGCGPIO_R;
	// PF4 -> Switch input
	// PF2 -> LED output
	GPIO_PORTF_DIR_R &= ~0x10;
	GPIO_PORTF_DIR_R |= 0x04;
	GPIO_PORTF_DEN_R |= 0x14;
	// Initialize heartbeat counter
	heartBeat = 0;
}


//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
	heartBeat++;
	if (heartBeat > 733137){ // ~2Hz
		heartBeat = 0; // Reset counter
		GPIO_PORTF_DATA_R ^= 0x4; // Toggle PF2
	}
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
	// --UUU-- wait for release; delay for 20ms; and then wait for press
	uint8_t data = 0;
	// Wait for switch release
	do {
		data = GPIO_PORTF_DATA_R & 0x10;
	}while (data == 1);
	// Debounce
	Delay1ms(20);
	// Wait for switch press
	do {
		data = GPIO_PORTF_DATA_R & 0x10;
	}while (data == 0);
	}
