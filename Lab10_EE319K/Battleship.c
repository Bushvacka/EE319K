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
#include "SysTick.h"

#define PORTE_PRIO 4
#define SYSTICK_PRIO 3
#define TIMER1_PRIO 2

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int PLL_Init(void);

typedef enum {English, Spanish} Language_t;
Language_t language = English;
typedef enum {Start, Wait, LANGUAGE} phrase_t;
const char Start_English[] ="Start";
const char Start_Spanish[] ="Comienzo";
const char Wait_English[]="Waiting for Player";
const char Wait_Spanish[]="Esperando al Jugador";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char *Phrases[3][2]={
  {Start_English,Start_Spanish},
  {Wait_English,Wait_Spanish},
  {Language_English,Language_Spanish}
};

// Initialize PortF for heartbeat
void PortF_Init(void){
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x20; // Port F
	delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |=  0x0E; // Output
  GPIO_PORTF_DEN_R |=  0x0E; // Digital Enable
}

// Initialize PortE for edge-triggered buttons
void PortE_Init(uint32_t priority){
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
  NVIC_PRI11_R = (NVIC_PRI11_R&0xFFFFFF1F) | (priority << 5); // Priority 3
  NVIC_EN0_R = 0x10;   			 // Enable IRQ 4
}

uint8_t shoot;
// Handle a button press (lock-in/shoot or change language)
void GPIOPortE_Handler(void){
  GPIO_PORTE_ICR_R = 0xC; // Ack
	if ((GPIO_PORTE_DATA_R & 0x8) == 0x8) {
		shoot = 1;
	} else {
		if (language == English) {
			language = Spanish;
		} else {
			language = English;
		}
	}
}

uint8_t blinker = 0;
void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Ack
	if (blinker) {
		blinker = 0;
	} else {
		blinker = 1;
	}
}

// Convert ADC input to a square
uint32_t Convert(uint32_t x){
	return ((1843*x)/4096) + 103;
}

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}

// Display title sequence until the shoot button is pressed
void startingSequence(void) {
	
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawBitmap(13, 119, title, 39, 79);
	while(shoot == 0) { // Wait for button press
			if (blinker) {
				ST7735_SetCursor(0, 0);
				ST7735_OutString((char *)Phrases[Start][language]);
			} else {
				ST7735_SetCursor(0, 0);
				ST7735_OutString("        ");
			}
	}
	shoot = 0; // Ignore
	UART_OutChar('R'); // Ready
	char data = 0;
	char *datapt = &data;
	while(data != 'R') { 
		Fifo_Get(datapt); 
		if (blinker) {
			ST7735_SetCursor(0, 0);
			ST7735_OutString((char *)Phrases[Wait][language]);
		} else {
			ST7735_SetCursor(0, 0);
			ST7735_OutString("        ");
		}
	} // Wait for ready signal
	TIMER1_CTL_R = 0x0; // Disable Timer1
}

int main(void){
  DisableInterrupts();
	PLL_Init();
  //TExaS_Init(NONE); // Bus clock is 80 MHz 
	PortE_Init(PORTE_PRIO);
	PortF_Init();
  ST7735_InitR(INITR_REDTAB);
	Timer1_Init(80000000/10, TIMER1_PRIO);
	ADC_Init();
	Fifo_Init();
	UART_Init();
	Sound_Init();
	SysTick_Init(80000000/30, SYSTICK_PRIO);
	Random_Init(NVIC_ST_CURRENT_R);
	EnableInterrupts();
	startingSequence();
	while (1) {
		Delay100ms(5);
	}
}

