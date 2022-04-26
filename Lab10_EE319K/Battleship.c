// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 1/12/2022 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "Fifo.h"
#include "Uart.h"

#define PORTE_PRIO 3

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int PLL_Init(void);

// Initialize PortF for heartbeat
void PortF_Init(void){
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x20; // Port F
	delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |=  0x0E; // Output
  GPIO_PORTF_DEN_R |=  0x0E; // Digital Enable
}

// Initialize PortE for edge-triggered buttons
void PortE_Init(void){
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x10; // Port E
	delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0xC; // Input
  GPIO_PORTE_DEN_R |= 0xC;  // Digital enable
  GPIO_PORTE_IS_R &= ~0xC;  // Edge-sensitive
  GPIO_PORTE_IBE_R &= ~0xC; // Not both edges
  GPIO_PORTE_IEV_R |= 0xC;  // Rising edge trigger
  GPIO_PORTE_ICR_R = 0xC;   // Clear flag
  GPIO_PORTE_IM_R |= 0xC;   // Eanble interrupt
  NVIC_PRI11_R = (NVIC_PRI11_R&0xFFFFFF1F)| (PORTE_PRIO << 5); // Priority 3
  NVIC_EN0_R = 0x10;   			 // Enable IRQ 4
}

uint32_t ADC_Data;
uint8_t ADC_Flag;
// Initialize SysTick for sampling the ADC
void SysTick_Init(uint32_t period){
	NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
	NVIC_ST_CURRENT_R = 0; // Reset counter
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x0FFFFFFF)|0x40000000; // Set priority to 2
	NVIC_ST_RELOAD_R = period - 1; // Load period
	NVIC_ST_CTRL_R = 0x07; // Enable interrupts & core clock
	ADC_Flag = 0;
}

void GPIOPortE_Handler(void){
  GPIO_PORTE_ICR_R = 0xC; // Ack
	GPIO_PORTF_DATA_R ^= 0x4; // Heartbeat
}

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// Ack
}

// Sample the ADC and alert the main program
void SysTick_Handler(void) {
	ADC_Data = ADC_In();
	ADC_Flag = 1;
}

// Convert ADC input to a square
uint32_t Convert(uint32_t x){
	return ((1843*x)/4096) + 103;
}

void Delay100ms(uint32_t count){uint32_t volatile time;
// Language setup - English default
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish} Language_t;
Language_t myLanguage = English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char *Phrases[3][2]={
  {Hello_English,Hello_Spanish},
  {Goodbye_English,Goodbye_Spanish},
  {Language_English,Language_Spanish}
};

int main(void){
  DisableInterrupts();
	PLL_Init();
  //TExaS_Init(NONE); // Bus clock is 80 MHz 
	PortE_Init();
	PortF_Init();
  Output_Init();
	ADC_Init();
	Fifo_Init();
	UART_Init();
	Sound_Init();
	SysTick_Init(80000000/30);
	Random_Init(NVIC_ST_CURRENT_R);
	EnableInterrupts();
  ST7735_FillScreen(0x0000); // Black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= Spanish; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
	ST7735_FillScreen(0x0000); // Black
	while (1) {
		if (ADC_Flag) {
			ST7735_SetCursor(0, 0);
			ADC_Flag = 0;
			LCD_OutFix(ADC_Data);
		}
	}
}

