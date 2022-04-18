// ADC.c
// Runs on TM4C123
// Provide functions that initialize ADC0
// Last Modified: 1/12/2021  
// Student names: Akhil Giridhar & Troy Dutton
// Last modification date: 4/10/2022
// Labs 8 and 9 specify PD2
#include <stdint.h>
#include "ADC.h"
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function using PD2 
// Input: none
// Output: none
void ADC_Init(void){
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x8;      // 1) activate clock for Port D 
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTD_DIR_R &= ~0x4;      // 2) make PD2 input
  GPIO_PORTD_AFSEL_R |= 0x4;     // 3) enable alternate fun on PD2 
  GPIO_PORTD_DEN_R &= ~0x4;      // 4) disable digital I/O on PD2 
  GPIO_PORTD_AMSEL_R |= 0x4;     // 5) enable analog fun on PD2 
  SYSCTL_RCGCADC_R |= 0x1;       // 6) activate ADC0 
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
  delay = SYSCTL_RCGCADC_R;
  ADC0_PC_R = 0x01;               // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Seq 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5;  // 11) Ain9 (PE4)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PD2, analog channel 5
uint32_t ADC_In(void){  
	uint32_t data;
  ADC0_PSSI_R = 0x0008;            
  while((ADC0_RIS_R&0x08)==0){};   
  data = ADC0_SSFIFO3_R&0xFFF; 
  ADC0_ISC_R = 0x0008;
	return data;
}

