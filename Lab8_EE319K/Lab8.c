// Lab8.c
// Runs on TM4C123
// Student names: Akhil Giridhar  & Troy Dutton
// Last modification date: 4/11/2022
// Last Modified: 1/12/2021 

// Specifications:
// Measure distance using slide pot, sample at 10 Hz
// maximum distance can be any value from 1.5 to 2cm
// minimum distance is 0 cm
// Calculate distance in fixed point, 0.001cm
// Analog Input connected to PD2=ADC0 channel 5
// displays distance on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats (use them in creative ways)
// 

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"

uint32_t ADCMail;
uint32_t ADCStatus;
//*****the first four main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
//       used to test ADC and slide pot
// main2 adds the LCD to the ADC and slide pot, ADC data is on ST7735
//       used to measure execution time of ADC and LCD
// main3 adds your convert function, plot position on ST7735
//       used to test Nyquist Theorem
// main4 used to test Central Limit Theorem
//       observe noise versus ADC0_SAC_R

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008)) b
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DEN_R |= 0x0E;
}
uint32_t Data;        // 12-bit ADC

// your function to convert 12 bit ADC sample to distance (0.001cm)
// input: x is 12 bit ADC digital sample
// output: integer part of distance in 0.001 resolution
uint32_t Convert(uint32_t x){
	return ((1843*x)/4096) + 103;
}

void SysTick_Init(uint32_t period){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_CURRENT_R = 0; // Reset counter
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // Set priority to 2
	NVIC_ST_RELOAD_R = period - 1; // Interrupt every 100 ms
	NVIC_ST_CTRL_R = 0x07; // Enable interrupts & core clock
}

void SysTick_Handler(void){
	GPIO_PORTF_DATA_R ^= 0x4; // Heartbeat
	ADCMail = ADC_In(); // Sample ADC
	ADCStatus = 1; // Set flag
}

int main(void){
	DisableInterrupts();
	TExaS_Init(SCOPE);
	ST7735_InitR(INITR_REDTAB);
	ADCStatus = 0;
	ADC_Init();
	SysTick_Init(80000000/10);
  PortF_Init();
	EnableInterrupts();
  while(1){
    if (ADCStatus) {
			ADCStatus = 0; // Clear flag
			uint32_t dist = Convert(ADCMail);
			ST7735_SetCursor(0,0);
			ST7735_OutString("Distance: ");
			LCD_OutFix(dist);
			ST7735_OutString(" cm");
		}
  }
}

