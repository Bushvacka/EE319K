// ADC.h
// Runs on TM4C123
// Provide functions that initialize ADC0

// Student names: Troy Dutton
// Last modification date: 4/10/2022
#ifndef ADC_H
#define ADC_H
#include <stdint.h>

// ADC initialization function 
// Input: none
// Output: none
// measures from PD2, analog channel 5
void ADC_Init(void);

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void);
#endif
