// DAC.c
// This software configures DAC output
// Lab 6 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: Akhil Giridhar & Troy Dutton
// Date Created: 3/6/17 
// Last Modified: 1/11/22 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC on PB5-0, called once 
// Input: none
// Output: none
void DAC_Init(void){
	// Initialize PORTB clock
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x2;
	delay = SYSCTL_RCGCGPIO_R;
	// Enable PB5-0
	GPIO_PORTB_DIR_R |= 0x3F;
	GPIO_PORTB_DEN_R |= 0x3F;
	// Allow for high-current output
	GPIO_PORTB_DR8R_R |= 0x3F;
}

// **************DAC_Out*********************
// Output to DAC on PB5-0
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;
}