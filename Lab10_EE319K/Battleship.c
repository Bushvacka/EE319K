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

// Determines which board is master
#define Tx 1
// ISR Constants
#define PORTE_PRIO 4
#define SYSTICK_PRIO 3
#define TIMER1_PRIO 2
// Game Constants
#define GRID_SIZE 7
#define COL_SEA ST7735_Color565(65,105,225)
#define COL_MARKER ST7735_Color565(255,69,0)

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int PLL_Init(void);

typedef enum {English, Spanish} Language_t;
Language_t language = English;
typedef enum {Start, Wait, LANGUAGE} phrase_t;
const char Start_English[] ="Press Button";
const char Start_Spanish[] ="Presione el Bot\xA2n";
const char Wait_English[]="Waiting for Other Player";
const char Wait_Spanish[]="Esperando a Otro Jugador";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char *Phrases[3][2]={
  {Start_English,Start_Spanish},
  {Wait_English,Wait_Spanish},
  {Language_English,Language_Spanish}
};
uint8_t blinker = 0, button1 = 0;

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

// Handle a button press (lock-in/shoot or change language)
void GPIOPortE_Handler(void){
  GPIO_PORTE_ICR_R = 0xC; // Ack
	if ((GPIO_PORTE_DATA_R & 0x8) == 0x8) {
		button1 = 1;
	} else {
		if (language == English) {language = Spanish;} 
		else {language = English;}
	}
}

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Ack
	if (blinker) {
		blinker = 0;
	} else {
		blinker = 1;
	}
}

// Convert ADC input to a square
uint32_t Convert(uint32_t data){
	return (7*data)/4096;
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

// Display title sequence.
// Wait until both players are ready.
void titleScreen(void) {
	ST7735_FillScreen(ST7735_BLACK);
	//ST7735_DrawBitmap(13, 119, title, 39, 79);
	while(button1 == 0) { // Wait for button press
			ST7735_SetCursor(0, 0);
			if (blinker) {
				ST7735_OutString((char *)Phrases[0][language]);
			} else {
				ST7735_OutString("                 ");
			}
	}
	button1 = 0; // Ignore
	char data = 0;
	char *datapt = &data;
	while(data != 'R') { // Wait for ready signal
		UART_OutChar('R');
		Fifo_Get(datapt); 
		ST7735_SetCursor(0, 0);
		if (blinker) {
			ST7735_OutString((char *)Phrases[Wait][language]);
		} else {
			ST7735_OutString("                        ");
		}
	}
	ST7735_FillScreen(ST7735_BLACK);
	TIMER1_CTL_R = 0x0; // Disable Timer1
}

void drawGrid(void) {
	ST7735_FillRect(1, 1, 125, 125, COL_SEA);
	for (int i = 0; i < GRID_SIZE + 1; i++) {
		ST7735_DrawFastVLine(18*i, 0, 127, ST7735_WHITE);
		ST7735_DrawFastHLine(0, 18*i, 127, ST7735_WHITE);
	}
}

void drawMarker(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_DrawFastVLine(18*x, 18*y, 19, color); // Left
	ST7735_DrawFastVLine(18*x + 18, 18*y, 19, color); // Right
	ST7735_DrawFastHLine(18*x, 18*y, 19, color); // Top
	ST7735_DrawFastHLine(18*x, 18*y + 18, 19, color); // Bottom
}
void fillSquare(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_FillRect(18*x + 1, 18*y + 1, 17, 17, color);
}

void drawTack(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_DrawCircle(18*x + 9, 18*y + 9, color);
}

void fireShot(void) {
	uint8_t markerX = 7, markerY = 7, newMarker; // 7 - Not yet selected
	// Select x position
	while (button1 == 0 || markerX == 7) {
		if (ADC_Flag) {
			ADC_Flag = 0;
			newMarker = Convert(ADC_Data);
			if (newMarker != markerX && markerX != 7) {
				drawMarker(markerX, 0, ST7735_WHITE);
			}
			markerX = newMarker;
			drawMarker(markerX, 0, COL_MARKER);
		}
	}
	button1 = 0;
	// Select y position
	while (button1 == 0 || markerY == 7) {
		if (ADC_Flag) {
			ADC_Flag = 0;
			newMarker = Convert(ADC_Data);
			if (newMarker != markerY && markerY != 7) {
				drawMarker(markerX, markerY, ST7735_WHITE);
			} else if (markerY == 7) {
				drawMarker(markerX, 0, ST7735_WHITE);
			}
			markerY = newMarker;
			drawMarker(markerX, markerY, COL_MARKER);
		}
	}
	button1 = 0; // Ack
	// X & Y locked in, time to fire
	char shot_msg[] = {'F', (char)markerX, (char)markerY};
	UART_OutString(shot_msg);
}
int main(void){
  DisableInterrupts();
	//PLL_Init();
  TExaS_Init(NONE); // Bus clock is 80 MHz 
	PortE_Init(PORTE_PRIO);
	PortF_Init();
  ST7735_InitR(INITR_REDTAB);
	ST7735_SetRotation(3);
	Timer1_Init(80000000/2, TIMER1_PRIO);
	ADC_Init();
	Fifo_Init();
	UART_Init();
	Sound_Init();
	SysTick_Init(80000000/30, SYSTICK_PRIO);
	Random_Init(NVIC_ST_CURRENT_R);
	EnableInterrupts();
	// Game start
	titleScreen();
	drawGrid();
	fireShot();
	char data = 0;
	char *datapt = &data;
	
	while (Fifo_Get(datapt) == 0 || data != 'F') {}
		
	Fifo_Get(datapt);
	uint8_t x = data;
	Fifo_Get(datapt);
	uint8_t y = data;
	if (x == 3 && y == 1) {
		UART_OutChar('H');
	} else {
		UART_OutChar('M');
	}
	
	while (Fifo_Get(datapt) == 0 || (data != 'H' && data != 'M')){}
		
	uint8_t hit = data == 'H' ? 1:0;
		
	if (hit) {
		drawTack(x, y, ST7735_RED);
	} else {
		drawTack(x, y, ST7735_WHITE);
	}
}
