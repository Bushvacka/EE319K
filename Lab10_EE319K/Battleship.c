// Battleship.c
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
#define SYSTICK_PRIO 3
#define TIMER1_PRIO 2
// Game Constants
#define GRID_SIZE 7
#define COL_SEA ST7735_Color565(65,105,225)
#define COL_MARKER ST7735_Color565(255,69,0)
#define COL_SHIP ST7735_Color565(255,215,0)
#define NUM_SHIPS 3

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
typedef enum {Start, Wait, P1, P2} phrase_t;
const char Start_English[] ="Press Button";
const char Start_Spanish[] ="Presione el Bot\xA2n";
const char Wait_English[]="Waiting for Other Player";
const char Wait_Spanish[]="Esperando a Otro Jugador";
const char P1_English[]="Player 1 Wins";
const char P1_Spanish[]="Jugador uno Gana!";
const char P2_English[]="Player 2 Wins!";
const char P2_Spanish[]="Jugador dos Gana!";
const char *Phrases[4][2]={
  {Start_English,Start_Spanish},
  {Wait_English,Wait_Spanish},
  {P1_English,P1_Spanish},
	{P2_English,P2_Spanish}
};

// Globals
uint8_t grid[GRID_SIZE][GRID_SIZE], markerGrid[GRID_SIZE][GRID_SIZE], selfMarkerGrid[GRID_SIZE][GRID_SIZE];
Ship_t ships[3];
uint32_t ADC_Data;
uint8_t ADC_Flag = 0;
uint8_t player = Tx;
uint8_t score1 = 0, score2 = 0;

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
void PortE_Init(void){
	volatile uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x10; // Port E
	delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0xE; // Input
  GPIO_PORTE_DEN_R |= 0xE;  // Digital enable
  /*GPIO_PORTE_IS_R &= ~0xE;  // Edge-sensitive
  GPIO_PORTE_IBE_R &= ~0xE; // Not both edges
  GPIO_PORTE_IEV_R |= 0xE;  // Rising edge trigger
  GPIO_PORTE_ICR_R = 0xE;   // Clear flag
  GPIO_PORTE_IM_R |= 0xE;   // Eanble interrupt
  NVIC_PRI11_R = (NVIC_PRI11_R&0xFFFFFF1F) | (priority << 5); // Priority 3
  NVIC_EN0_R = 0x10;   			 // Enable IRQ 4
	*/
}
// Sample ADC
void Timer1A_Handler(void){
	ADC_Data = ADC_In();
	ADC_Flag = 1;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Ack
}

// Convert ADC input to a grid position
uint32_t Convert(uint32_t data){
	return (7*data)/4096;
}
// Wait for the other board
void waitForSync(void) {
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(1, 6);
	ST7735_OutString((char *)Phrases[Wait][language]);
	char data = 0;
	char *datapt = &data;
	UART_OutString("R>>>");
	while (data != 'R') {
		Fifo_Get(datapt);
		if (button3) {
			button3 = 0; // Ack
			// Change language
			if (language == English) {
				language = Spanish;
			} else {
				language = English;
			}
			// Output new phrase
			ST7735_SetCursor(1, 6);
			ST7735_OutString("               ");
			ST7735_SetCursor(1, 6);
			ST7735_OutString((char *)Phrases[Wait][language]);
		}
	} // Wait for ready signal 
	// Clear FIFO
	uint8_t success;
	do {
		success = Fifo_Get(datapt);
	} while (data == '>' && success);
	button1 = 0; // Ack
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
	ST7735_DrawCircle(18*x + 5, 18*y + 5, color);
}
// Draw the players view of the enemy 
void drawMarkers(uint8_t mGrid[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (mGrid[i][j] == 1) {
				drawMarker(j, i, ST7735_RED);
			} else if (mGrid[i][j] == 2) {
				drawMarker(j, i, ST7735_WHITE);
			}
		}
	}
}
// Draws all ships to the screen
void drawShips(void) {
	for (int i = 0; i < NUM_SHIPS; i++) {
		for (int j = 0; j < ships[i].length; j++) {
			fillSquare(ships[i].squares[j].x, ships[i].squares[j].y, COL_SHIP);
		}
	}
}
// Returns 1 if game is over
uint8_t gameDone(void) {
	uint8_t hit_counter = 0, enemy_hit_counter = 0;
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (markerGrid[i][j]) {
				hit_counter++;
			}
			if (selfMarkerGrid[i][j]) {
				enemy_hit_counter++;
			}
		}
	}
	if (hit_counter >= 9 || enemy_hit_counter >= 9) {
		return 1;
	} else {
		return 0;
	}
}
// Returns 1 if player 1(Tx) has won, and 2 if player 2 has won
// Assumes game is complete
uint8_t winner(void) {
	uint8_t hit_counter = 0, enemy_hit_counter = 0;
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (markerGrid[i][j]) {
				hit_counter++;
			}
			if (selfMarkerGrid[i][j]) {
				enemy_hit_counter++;
			}
		}
	}
	if (Tx) {
		if (hit_counter >= 9) {
			return 1;
		} else {
		}
	} else {
		if (hit_counter >= 9) {
			return 2;
		}
	}
	return 0; // Error
}
// Returns true if the ship overlaps with another ship already on the grid
uint8_t shipOverlap(Ship_t ship) {
	for (int i = 0; i < ship.length; i++) {
		uint8_t x = ship.squares[i].x;
		uint8_t y = ship.squares[i].y;
		if (grid[y][x]) {
			return 1; // Overlap
		}
	}
	return 0; // No overlap
}
// Display the title sequence.
void titleScreen(void) {
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_DrawBitmap(38, 65, title, 79, 39);
	ST7735_SetCursor(7, 7);
	ST7735_OutString((char *)Phrases[0][language]);
	while(button1 == 0) {
		if (button3) {
			button3 = 0; // Ack
			// Change language
			if (language == English) {
				language = Spanish;
			} else {
				language = English;
			}
			// Output new phrase
			ST7735_SetCursor(7, 7);
			ST7735_OutString("                  ");
			ST7735_SetCursor(7, 7);
			ST7735_OutString((char *)Phrases[Start][language]);
		}
	} // Wait for button press
	button1 = 0; // Ack
	ST7735_FillScreen(ST7735_BLACK); // Clear Screen
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
				if (dir && x == GRID_SIZE - 1) { // Vertical and at right edge
					button2 = 0; // Ignore
				} else {
					dir = (dir + 1) % 2;
				}
			}
			
			if (ADC_Flag) {
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Check bounds
				if (dir == 0 && newPos + length - 1 >= GRID_SIZE) { // Horizontal
					continue;
				}
				
				// Clean up old position
				if ((newPos != x && x != 7) || button2) {
					if (button2) {
						button2 = 0; // Ack
					}
					for (int i = 0; i < ship.length; i++) {
						fillSquare(ship.squares[i].x, ship.squares[i].y, grid[ship.squares[i].y][ship.squares[i].x] ? COL_SHIP : COL_SEA);
					}
				}
				if (dir) { // Vertical
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = newPos;
						ship.squares[i].y = i;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SHIP);
					}
				} else { // Horizontal
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = newPos + i;
						ship.squares[i].y = 0;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SHIP);
					}
				}
				
				x = newPos;		
			}
		}
		button1 = 0; // Ack
		uint8_t nextAxis = 0; // Ready to place?
		while (!nextAxis) {
			if (button2) {
				if (dir == 0 && y == GRID_SIZE - 1) { // Horizontal and at bottom edge
					button2 = 0; // Ignore
				} else if(dir && x == GRID_SIZE - 1) {
					button2 = 0;
				}
				else {
					dir = (dir + 1) % 2;
				}
			}
				
			if (ADC_Flag) {
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Check bounds
				if (dir && newPos + length - 1 >= GRID_SIZE) { // Horizontal
					continue;
				}
				// Clean up old position
				if ((newPos != y) || button2) {
					if (button2) {
						button2 = 0; // Ack
					}
					for (int i = 0; i < ship.length; i++) {
						fillSquare(ship.squares[i].x, ship.squares[i].y, grid[ship.squares[i].y][ship.squares[i].x] ? COL_SHIP : COL_SEA);
					}
				}
				if (dir) { // Vertical
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = x;
						ship.squares[i].y = newPos + i;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SHIP);
					}
				} else { // Horizontal
					for (int i = 0; i < length; i++) {
						ship.squares[i].x = x + i;
						ship.squares[i].y = newPos;
						fillSquare(ship.squares[i].x, ship.squares[i].y, COL_SHIP);
					}
				}
				y = newPos;
			}
			
			// Check if ship placement is done
			if (button1 == 1 && y != 7) {
				if (shipOverlap(ship)) {
					button1 = 0; // Ignore
				} else {
					nextAxis = 1;
				}
			} else {
				nextAxis = 0;
			}
		}
		button1 = 0; // ACk
		
		// Save ship
		for (int i = 0; i < ship.length; i++) {
			grid[ship.squares[i].y][ship.squares[i].x] = 1;
		}
		ships[length - 2] = ship;
	}
}
// Get a grid position from the user
Point_t selectGrid(void) {
	uint8_t markerX = 7, markerY = 7, newMarker; // 7 - Not yet selected
	// Select x position
	// Display score
	ST7735_SetCursor(22, 0);
	ST7735_OutString("1:");
	ST7735_OutChar((char)(score1 + 0x30));
	ST7735_SetCursor(22, 2);
	ST7735_OutString("2:");
	ST7735_OutChar((char)(score2 + 0x30));
	while (button1 == 0 || markerX == 7) {
		if (ADC_Flag) {
			ADC_Flag = 0;
			newMarker = Convert(ADC_Data);
			// Clean up old crosshair
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
	PortE_Init();
	PortF_Init();
  ST7735_InitR(INITR_REDTAB);
	ST7735_SetRotation(3);
	ADC_Init();
	Fifo_Init();
	UART_Init();
	Sound_Init();
	Timer1_Init(80000000/30, TIMER1_PRIO);
	SysTick_Init(80000000/5, SYSTICK_PRIO);
	Random_Init(NVIC_ST_CURRENT_R);
	EnableInterrupts();
	
	// Main Menu
	titleScreen();
	// Ship Placement
	drawGrid();
	placeShips();
	waitForSync();
	// Game Begins
	char data = 0;
	char *datapt = &data;
	while (!gameDone()) {
			if (player) {
				uint8_t hit;
				do {
					// Display marker grid
					ST7735_FillScreen(ST7735_BLACK);
					drawGrid();
					drawMarkers(markerGrid);
					// Get shot selection
					Point_t shot = selectGrid();
					// Send fire command
					UART_OutChar('F');
					char msg = shot.x + 0x30;
					UART_OutChar(msg);
					msg = shot.y + 0x30;
					UART_OutChar(msg);
					UART_OutChar('>');
					// Await response
					data = 0;
					while (Fifo_Get(datapt) == 0 || (data != 'H' && data != 'M')){}
					hit = data == 'H' ? 1 : 0;
					// Clear FIFO
					while (data != '>') {
						Fifo_Get(datapt);
					}
					// 
					if (hit) {
						markerGrid[shot.y][shot.x] = 1;
						drawMarker(shot.x, shot.y, ST7735_RED);
						if (Tx) {
							score1++;
						} else {
							score2++;
						}
					} else {
						markerGrid[shot.y][shot.x] = 2;
						drawMarker(shot.x, shot.y, ST7735_WHITE);
					}
					// Display score
					ST7735_SetCursor(22, 0);
					ST7735_OutString("1:");
					ST7735_OutChar((char)(score1 + 0x30));
					ST7735_SetCursor(22, 2);
					ST7735_OutString("2:");
					ST7735_OutChar((char)(score2 + 0x30));
				} while (hit && !gameDone());
				player = (player + 1) % 2;
			} else {
					uint8_t hit;
					ST7735_FillScreen(ST7735_BLACK);
					drawGrid();
					drawShips();
					drawMarkers(selfMarkerGrid);
					do {
						// Await shot position
						data = 0;
						while (Fifo_Get(datapt) == 0 || data != 'F') {}
						Fifo_Get(datapt);
						uint8_t x = data - 0x30;
						Fifo_Get(datapt);
						uint8_t y = data - 0x30;
						// Clear FIFO
						while (data != '>') {
							Fifo_Get(datapt);
						}
						hit = grid[y][x] == 0 ? 0:1;
						
						if (hit) {
							selfMarkerGrid[y][x] = 1;
							drawMarker(x, y, ST7735_RED);
							UART_OutString("H..>");
							if (Tx) {
								score2++;
							} else {
								score1++;
							}
						} else {
							selfMarkerGrid[y][x] = 2;
							drawMarker(x, y, ST7735_WHITE);
							UART_OutString("M..>");
						}
						// Display score
						ST7735_SetCursor(22, 0);
						ST7735_OutString("1:");
						ST7735_OutChar((char)(score1 + 0x30));
						ST7735_SetCursor(22, 2);
						ST7735_OutString("2:");
						ST7735_OutChar((char)(score1 + 0x30));
					} while (hit && !gameDone());
					player = (player + 1) % 2;
			}
	}		
	uint8_t winning_player = winner();
	ST7735_FillScreen(ST7735_BLACK);
	// Display score
	ST7735_SetCursor(1, 1);
	ST7735_OutString("1:");
	ST7735_OutChar((char)(score1 + 0x30));
	ST7735_SetCursor(1, 3);
	ST7735_OutString("2:");
	ST7735_OutChar((char)(score1 + 0x30));
	// Display winner
	ST7735_SetCursor(7, 7);
	while (1) {
		if (button3) {
			button3 = 0; // Ack
			// Change language
			if (language == English) {
				language = Spanish;
			} else {
				language = English;
			}
			if (winning_player == 1) {
				ST7735_OutString((char *)Phrases[P1][language]);
			} else {
				ST7735_OutString((char *)Phrases[P2][language]);
			}
		}
	}
}
