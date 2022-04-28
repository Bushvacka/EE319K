// SysTick.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize the SysTick module and handles interrupts
// Student names: Akhil Giridhar and Troy Dutton
// Last modification date: 4/26/2022

#include <stdint.h>
#include "SysTick.h"
#include "ADC.h"
#include "../inc/tm4c123gh6pm.h"

uint8_t button1;
uint8_t button2;
uint32_t previous_data;

// Initialize SysTick for sampling the ADC
void SysTick_Init(uint32_t period, uint32_t priority){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_CURRENT_R = 0; // Reset counter
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x0FFFFFFF)|(priority << 29); // Set priority
	NVIC_ST_RELOAD_R = period - 1; // Load period
	NVIC_ST_CTRL_R = 0x07; // Enable interrupts & core clock
	//ADC_Flag = 0;
	button1 = 0;
	button2 = 0;
	previous_data = 0xFFFFFFFF;
}

// Sample the ADC and alert the main program
void SysTick_Handler(void) {
	uint32_t data = GPIO_PORTE_DATA_R;
	if ((data & 0x8) == 0x8 && (previous_data & 0x8) == 0) { // Button 1 - Fire/Lock in/Ready up
	button1 = 1;
	}
	else if ((data & 0x4) == 0x4 && (previous_data & 0x4) == 0) { // Button 2 - Rotate ship
	button2 = 1;
	}
	/*else if((data & 0x2) == 0x2 && (previous_data & 0x2) == 0) { // Button 3 - Language Change
		if (language == English) {language = Spanish;} 
		else {language = English;}
	}*/
	previous_data = data;
}

