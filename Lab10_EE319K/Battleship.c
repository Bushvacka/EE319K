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

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int PLL_Init(void);

// Master/Slave
#define Tx 1
// ISR Constants
#define PORTE_PRIO 4
#define SYSTICK_PRIO 3
#define TIMER1_PRIO 2
// Game Constants
#define GRID_SIZE 7
#define COL_SEA ST7735_Color565(65,105,225)
#define COL_MARKER ST7735_Color565(255,69,0)

// Grid points
struct Point {
	uint8_t x;
	uint8_t y;
};
typedef struct Point Point_t;

// Ship
struct Ship {
	Point_t squares[4];
	uint8_t hit[4];
	uint8_t length;
};
typedef struct Ship Ship_t;

// Languages & Phrases
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

// Globals
uint8_t display = 0, button1 = 0, button2 = 0;
uint8_t grid[GRID_SIZE][GRID_SIZE], markerGrid[GRID_SIZE][GRID_SIZE];
Ship_t ships[3];

// Delay for 100ms count times
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
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
  GPIO_PORTE_DIR_R &= ~0xE; // Input
  GPIO_PORTE_DEN_R |= 0xE;  // Digital enable
  GPIO_PORTE_IS_R &= ~0xE;  // Edge-sensitive
  GPIO_PORTE_IBE_R &= ~0xE; // Not both edges
  GPIO_PORTE_IEV_R |= 0xE;  // Rising edge trigger
  GPIO_PORTE_ICR_R = 0xE;   // Clear flag
  GPIO_PORTE_IM_R |= 0xE;   // Eanble interrupt
  NVIC_PRI11_R = (NVIC_PRI11_R&0xFFFFFF1F) | (priority << 5); // Priority 3
  NVIC_EN0_R = 0x10;   			 // Enable IRQ 4
}

// Handle button presses
void GPIOPortE_Handler(void){
  GPIO_PORTE_ICR_R = 0xE; // Ack
	uint32_t data = GPIO_PORTE_DATA_R;
	if ((data & 0x8) == 0x8) {button1 = 1;} 		 // Button 1 - Fire/Lock in/Ready up
	else if ((data & 0x4) == 0x4) {button2 = 1;} // Button 2 - Rotate ship
	else { 																			 // Button 3 - Language Change
		if (language == English) {language = Spanish;} 
		else {language = English;}
	}
}

// Flash messages during the title sequence
void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Ack
	if (display) {
		display = 0;
	} else {
		display = 1;
	}
}

// Convert ADC input to a grid position
uint32_t Convert(uint32_t data){
	return (7*data)/4096;
}
// Wait for the other board
void waitForSync(void) {
	TIMER1_CTL_R = 0x1; // Enable Blinking
	char data = 0;
	char *datapt = &data;
	UART_OutChar('R');
	while(data != 'R') { // Wait for ready signal
		Fifo_Get(datapt); 
		ST7735_SetCursor(0, 0);
		if (display) {
			ST7735_OutString((char *)Phrases[Wait][language]);
		} else {
			ST7735_OutString("                        ");
		}
	}
	button1 = 0; // Ack
	TIMER1_CTL_R = 0x0; // Disable Blinking
}
// Draw Battleship sea and gridlines.
void drawGrid(void) {
	ST7735_FillRect(1, 1, 125, 125, COL_SEA);
	for (int i = 0; i < GRID_SIZE + 1; i++) {
		ST7735_DrawFastVLine(18*i, 0, 127, ST7735_WHITE);
		ST7735_DrawFastHLine(0, 18*i, 127, ST7735_WHITE);
	}
}

// Highlight a grid square to indicate the selected grid position
void drawCrosshair(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_DrawFastVLine(18*x, 18*y, 19, color); // Left
	ST7735_DrawFastVLine(18*x + 18, 18*y, 19, color); // Right
	ST7735_DrawFastHLine(18*x, 18*y, 19, color); // Top
	ST7735_DrawFastHLine(18*x, 18*y + 18, 19, color); // Bottom
}

// Fill a grid square with a color
void fillSquare(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_FillRect(18*x + 1, 18*y + 1, 17, 17, color);
}
// Draw a shot marker to indicate a hit or miss
void drawMarker(uint8_t x, uint8_t y, uint16_t color) {
	ST7735_DrawCircle(18*x + 9, 18*y + 9, color);
}
// Display the title sequence.
void titleScreen(void) {
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawBitmap(13, 119, title, 39, 79);
	while(button1 == 0) { // Wait for button press
			ST7735_SetCursor(0, 0);
			if (display) {
				ST7735_OutString((char *)Phrases[0][language]);
			} else {
				ST7735_OutString("                 ");
			}
	}
	button1 = 0; // Ack
	ST7735_FillScreen(ST7735_BLACK); // Clear Screen
	TIMER1_CTL_R = 0x0; // Disable Timer1
}

// Place ships on the grid
void placeShips(void) {
	// Clear both grids
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[i][j] = 0;
			markerGrid[i][j] = 0;
		}
	}
	
	// 3 Ships - lengths ranging from 2 to 4
	for (int length = 2; length <= 4; length++) {
		uint8_t x = 7, y = 7, newPos, dir = 0;
		Ship_t ship;
		ship.length = length;
		for (int i = 0; i < length; i++) {
			ship.hit[i] = 0;
		}
		
		// Select x position
		while (button1 == 0 || x == 7) {
			if (button2) {
				dir = (dir + 1) % 2;
			}
			
			if (ADC_Flag) {
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Clean up old position
				if ((newPos != x && x != 7) || button2) {
					if (button2) {
						button2 = 0; // Ack
					}
					for (int i = 0; i < ship.length; i++) {
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SEA);
					}
				}
				if (dir) { // Vertical
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = newPos;
						ship.squares[i].y = i;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_MARKER);
					}
				} else { // Horizontal
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = newPos + i;
						ship.squares[i].y = 0;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_MARKER);
					}
				}
				
				x = newPos;		
			}
		}
		
		button1 = 0; // Ack
		while (button1 == 0 || y == 7) {
			if (button2) {
				dir = (dir + 1) % 2;
			}
			
			if (ADC_Flag) {
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Clean up old position
				if ((newPos != y && y != 7) || button2) {
					if (button2) {
						button2 = 0; // Ack
					}
					for (int i = 0; i < ship.length; i++) {
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SEA);
					}
				}
				if (dir) { // Vertical
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = x;
						ship.squares[i].y = newPos + i;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_MARKER);
					}
				} else { // Horizontal
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = x;
						ship.squares[i].y = newPos + i;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_MARKER);
					}
				}
				
				y = newPos;		
			}
		};
		
		// Save ship
		ships[length - 2] = ship;
	}
}
// Get a grid position from the user
Point_t selectGrid(void) {
	uint8_t markerX = 7, markerY = 7, newMarker; // 7 - Not yet selected
	// Select x position
	while (button1 == 0 || markerX == 7) {
		if (ADC_Flag) {
			ADC_Flag = 0;
			newMarker = Convert(ADC_Data);
			if (newMarker != markerX && markerX != 7) {
				drawCrosshair(markerX, 0, ST7735_WHITE);
			}
			markerX = newMarker;
			drawCrosshair(markerX, 0, COL_MARKER);
		}
	}
	button1 = 0;
	// Select y position
	while (button1 == 0 || markerY == 7) {
		if (ADC_Flag) {
			ADC_Flag = 0;
			newMarker = Convert(ADC_Data);
			if (newMarker != markerY && markerY != 7) {
				drawCrosshair(markerX, markerY, ST7735_WHITE);
			} else if (markerY == 7) {
				drawCrosshair(markerX, 0, ST7735_WHITE);
			}
			markerY = newMarker;
			drawCrosshair(markerX, markerY, COL_MARKER);
		}
	}
	button1 = 0; // Ack
	Point_t shot = {markerX, markerY};
	return shot;
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
	
	// Main Menu
	titleScreen();
	// Ship Placement
	drawGrid();
	placeShips();
	waitForSync();
	// Game Begins
	uint8_t completed = 0, player = Tx, hit;
	char data = 0;
	char *datapt = &data;
	while (!completed) {
			if (player) {
				do {
					// Get shot selection
					Point_t shot = selectGrid();
					// Send fire command
					char shot_msg[] = {'F', (char)shot.x, (char)shot.y};
					UART_OutString(shot_msg);
					// Await response
					data = 0;
					while (Fifo_Get(datapt) == 0 || (data != 'H' && data != 'M')){}
					// Draw marker
					if (data == 'H') {
						drawMarker(shot.x, shot.y, ST7735_RED);
					} else {
						drawMarker(shot.x, shot.y, ST7735_WHITE);
					}
				} while (data == 'H' && !completed);
			} else {
					do {
						// Await shot position
						data = 0;
						while (Fifo_Get(datapt) == 0 || data != 'F') {}
						Fifo_Get(datapt);
						uint8_t x = data;
						Fifo_Get(datapt);
						uint8_t y = data;
						hit = grid[y][x] == 0 ? 0:1;
						if (hit) {
							ST7735_OutChar('H');
						} else {
							ST7735_OutChar('M');
						}
					} while (hit && !completed);
			}
	}		
}
