// SysTick.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize the SysTick module and handles interrupts
// Student names: Akhil Giridhar and Troy Dutton
// Last modification date: 4/26/2022

#include <stdint.h>
#include "SysTick.h"
#include "ADC.h"
#include "../inc/tm4c123gh6pm.h"

uint32_t ADC_Data;
uint8_t ADC_Flag = 0;

// Initialize SysTick for sampling the ADC
void SysTick_Init(uint32_t period, uint32_t priority){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_CURRENT_R = 0; // Reset counter
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x0FFFFFFF)|(priority << 29); // Set priority
	NVIC_ST_RELOAD_R = period - 1; // Load period
	NVIC_ST_CTRL_R = 0x07; // Enable interrupts & core clock
	ADC_Flag = 0;
}

// Sample the ADC and alert the main program
void SysTick_Handler(void) {
	ADC_Data = ADC_In();
	ADC_Flag = 1;
}
