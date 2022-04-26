// SysTick.h
// Runs on TM4C123
// Provide functions that initialize the SysTick module and handles interrupts
// Student names: Akhil Giridhar and Troy Dutton
// Last modification date: 4/26/2022


#ifndef __systick_h
#define __systick_h
#include <stdint.h>

extern uint32_t ADC_Data;
extern uint8_t ADC_Flag;

// Initialize SysTick for sampling the ADC
void SysTick_Init(uint32_t period, uint32_t priority);

// Sample the ADC and alert the main program
void SysTick_Handler(void);

#endif
