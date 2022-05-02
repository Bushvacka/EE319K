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
#define COL_SEA ST7735_Color565(0,148,255)
#define COL_MARKER ST7735_Color565(255,69,0)
#define COL_SHIP ST7735_Color565(255,215,0)
#define NUM_SHIPS 3
#define ROTATION 3

// Grid points
struct Point {
	uint8_t x;
	uint8_t y;
};
typedef struct Point Point_t;

// Ship
struct Ship {
	Point_t origin;
	uint8_t dir;
	uint8_t length;
	const uint16_t *image;
	uint8_t w, h;
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
	for (int i = 0; i < GRID_SIZE + 1; i++) {
		ST7735_DrawFastVLine(18*i, 0, 127, ST7735_WHITE);
		ST7735_DrawFastHLine(0, 18*i, 127, ST7735_WHITE);
	}
}
// Display player scores
void displayScores(uint8_t x, uint8_t y) {
	ST7735_SetCursor(x, y);
	ST7735_OutString("1:");
	ST7735_OutChar((char)(score1 + 0x30));
	ST7735_SetCursor(x, y + 2);
	ST7735_OutString("2:");
	ST7735_OutChar((char)(score2 + 0x30));
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
// Draws one ship to the screen
void drawShip(Ship_t ship) {
	if (ship.dir) { // Vertical
		ST7735_SetRotation((ROTATION + 3) % 3); // Vertical Rotation
		ST7735_DrawBitmap(18*ship.origin.y + 1, 158 - 18*ship.origin.x, ship.image, ship.w, ship.h);
		ST7735_SetRotation(ROTATION); // Reset rotation
	} else {
		ST7735_DrawBitmap(18*ship.origin.x + 1, 18*ship.origin.y + ship.h, ship.image, ship.w, ship.h);
	}
	if (ship.dir)  { // Reset rotation
		ST7735_SetRotation(ROTATION);
	}
}
// Draws all ships to the screen
void drawShips(void) {
	for (int i = 0; i < NUM_SHIPS; i++) {
		drawShip(ships[i]);
	}
}
// Returns true if the ship overlaps with another ship already on the grid
uint8_t shipOverlap(Ship_t ship) {
	for (int i = 0; i < ship.length; i++) {
		uint8_t x = ship.dir ? ship.origin.x : ship.origin.x + i;
		uint8_t y = ship.dir ? ship.origin.y + i : ship.origin.y;
		if (grid[y][x]) {
			return 1; // Overlap
		}
	}
	return 0; // No overlap
}
// Returns the index of the ship that occupies a grid position
uint8_t position2Ship(uint8_t x, uint8_t y) {
	for (int i = 0; i < NUM_SHIPS; i++) {
		Ship_t ship = ships[i];
		for (int j = 0; j < ship.length; j++) {
			uint8_t segmentX = ship.dir ? ship.origin.x : ship.origin.x + j;
			uint8_t segmentY = ship.dir ? ship.origin.y + j : ship.origin.y;
			if (segmentX == x && segmentY == y) { // Ship found
				return i;
			}
		}
	}
	return 0; // Error
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
		uint8_t x = 7, y = 7, newPos;
		Ship_t ship;
		ship.origin.y = 0;
		ship.dir = 0;
		ship.length = length;
		if (length == 2) {
			ship.image = PatrolBoat;
			ship.w = 35;
			ship.h = 17;
		} else if (length == 3) {
			ship.image = Cruiser;
			ship.w = 53;
			ship.h = 17;
		} else {
			ship.image = BattleShip;
			ship.w = 71;
			ship.h =   17;
		}
		
		// Select x position
		while (button1 == 0 || x == 7) {
			if (button2) {
				if (ship.dir && x == GRID_SIZE - 1) { // Vertical and at right edge
					button2 = 0; // Ignore
				} else {
					ship.dir = (ship.dir + 1) % 2; // Update direction
				}
			}
			
			if (ADC_Flag) { // Updated position available
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Check bounds
				if (ship.dir == 0 && newPos + length - 1 >= GRID_SIZE) { // Horizontal
					continue;
				}
				
				// Clean up old position
				if ((newPos != x && x != 7) || button2) {
					if (button2) {
						ship.dir = (ship.dir + 1) % 2; // Old direction
					}
					for (int i = 0; i < ship.length; i++) {
						uint8_t segmentX = ship.dir ? ship.origin.x : ship.origin.x + i;
						uint8_t segmentY = ship.dir ? ship.origin.y + i : ship.origin.y;
						if (grid[segmentY][segmentX]) { // Redraw old ship
							// Find old ship
							uint8_t j = position2Ship(segmentX, segmentY);
							drawShip(ships[j]);
						} else { // Replace with sea
							fillSquare(segmentX, segmentY, COL_SEA); 
						}
					}
					if (button2) {
						button2 = 0; // Ack
						ship.dir = (ship.dir + 1) % 2; // Restore direction
					}
				}
				// Update ship position
				ship.origin.x = newPos;	
				x = newPos;
				// Draw ship
				drawShip(ship);
				drawGrid();
			}
		}
		button1 = 0; // Ack
		
		// Select y position
		uint8_t finishedPlacing = 0;
		while (!finishedPlacing) {
			if (button2) {
				if (ship.dir == 0 && y == GRID_SIZE - 1) { // Horizontal and at bottom edge
					button2 = 0; // Ignore
				} else if(ship.dir && x == GRID_SIZE - 1) {
					button2 = 0;
				}
				else {
					ship.dir = (ship.dir + 1) % 2;
				}
			}
				
			if (ADC_Flag) {
				ADC_Flag = 0;
				newPos = Convert(ADC_Data);
				
				// Check bounds
				if (ship.dir && newPos + length - 1 >= GRID_SIZE) { // Horizontal
					continue;
				}
				
				// Clean up old position
				if (newPos != y || button2) {
					if (button2) {
						ship.dir = (ship.dir + 1) % 2; // Old direction
					}
					for (int i = 0; i < ship.length; i++) {
						uint8_t segmentX = ship.dir ? ship.origin.x : ship.origin.x + i;
						uint8_t segmentY = ship.dir ? ship.origin.y + i : ship.origin.y;
						if (grid[segmentY][segmentX]) { // Redraw old ship
							// Find old ship
							drawShip(ships[position2Ship(segmentX, segmentY)]);
						} else { // Replace with sea
							fillSquare(segmentX, segmentY, COL_SEA); 
						}
					}
					if (button2) {
						button2 = 0; // Ack
						ship.dir = (ship.dir + 1) % 2; // Restore direction
					}
				}
				
				// Update ship position
				ship.origin.y = newPos;
				y = newPos;
				drawShip(ship); // Draw ship
				drawGrid();
			}
			
			// Check if ship placement is done
			if (button1 == 1 && y != 7) {
				if (shipOverlap(ship)) {
					button1 = 0; // Ignore
				} else {
					finishedPlacing = 1;
				}
			} else {
				finishedPlacing = 0;
			}
		}
		button1 = 0; // ACk
		
		// Save ship
		for (int i = 0; i < ship.length; i++) {
			uint8_t segmentX = ship.dir ? ship.origin.x : ship.origin.x + i;
			uint8_t segmentY = ship.dir ? ship.origin.y + i : ship.origin.y;
			grid[segmentY][segmentX] = 1;
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
	// Initializations
  DisableInterrupts();
  TExaS_Init(NONE);
	PortE_Init();
	PortF_Init();
  ST7735_InitR(INITR_REDTAB);
	ST7735_SetRotation(ROTATION);
	ADC_Init();
	Fifo_Init();
	UART_Init();
	Sound_Init();
	Timer1_Init(80000000/30, TIMER1_PRIO);
	SysTick_Init(80000000/5, SYSTICK_PRIO);
	Random_Init(NVIC_ST_CURRENT_R);
	EnableInterrupts();
	char data = 0;
	char *datapt = &data;
	
	// Main Menu
	titleScreen();
	// Ship Placement
	ST7735_FillRect(1, 1, 125, 125, COL_SEA);
	drawGrid();
	placeShips();
	waitForSync();
	// Game Begins
	while (!(score1 >= 9 || score2 >= 9)) {
			if (player) {
				uint8_t hit;
				do {
					// Display layers
					ST7735_FillScreen(ST7735_BLACK);
					ST7735_FillRect(1, 1, 125, 125, COL_SEA);
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
					
					data = 0;
					while (Fifo_Get(datapt) == 0 || (data != 'H' && data != 'M')){} // Await response

					hit = data == 'H' ? 1 : 0; // Response recieved
					
					// Clear FIFO
					while (data != '>') {
						Fifo_Get(datapt);
					}
					
					if (hit) { // Hit
						// Store & draw marker
						markerGrid[shot.y][shot.x] = 1;
						drawMarker(shot.x, shot.y, ST7735_RED);
						
						// Add to respective score
						if (Tx) {
							score1++;
						} else {
							score2++;
						}
						
						// Play hit sound
						if (Tx) {Play_Hit_Sound();}
					} else { // Miss
						// Store & draw marker
						markerGrid[shot.y][shot.x] = 2;
						drawMarker(shot.x, shot.y, ST7735_WHITE);
						
						// Play miss sound
						if (Tx) {Play_Miss_Sound();}
					}
					displayScores(22, 0); // Display player scores
				} while (hit && !(score1 >= 9 || score2 >= 9)); // Continue until player misses or game is over
				player = (player + 1) % 2; // Swap player
			} else {
					uint8_t hit;
					displayScores(22, 0);
					do {
						// Draw layers
						ST7735_FillScreen(ST7735_BLACK);
						ST7735_FillRect(1, 1, 125, 125, COL_SEA);
						drawShips();
						drawGrid();
						drawMarkers(selfMarkerGrid);
						displayScores(22, 0); // Display player scores

						data = 0;
						while (Fifo_Get(datapt) == 0 || data != 'F') {} // Await fire message
						
						// Shot position recieved
						Fifo_Get(datapt);
						uint8_t x = data - 0x30;
						Fifo_Get(datapt);
						uint8_t y = data - 0x30;
						
						// Clear FIFO
						while (data != '>') {
							Fifo_Get(datapt);
						}
						hit = grid[y][x] == 0 ? 0:1;
						
						// Fire sound
						
						if (hit) { // Hit
							// Store & draw marker
							selfMarkerGrid[y][x] = 1;
							drawMarker(x, y, ST7735_RED);
							
							// Add to respective score
							if (Tx) {
								score2++;
							} else {
								score1++;
							}
							
							// Play hit sound
							if (Tx) {Play_Hit_Sound();}
							
							// Send hit response
							UART_OutString("H..>");
						} else {
							// Store & draw marker
							selfMarkerGrid[y][x] = 2;
							drawMarker(x, y, ST7735_WHITE);
							
							// Play miss sound
							if (Tx) {Play_Miss_Sound();}
							// Send miss response
							UART_OutString("M..>");
						}
					} while (hit && !(score1 >= 9 || score2 >= 9)); // Continue until player misses or game is over
					player = (player + 1) % 2; // Swap player
			}
	}		
	
	// Game Over
	ST7735_FillScreen(ST7735_BLACK);
	displayScores(1, 1); // Display player scores
	
	// Display winner
	ST7735_SetCursor(6, 6);
	if (score1 >= 9) { // Player 1 win
		ST7735_OutString((char *)Phrases[P1][language]);
	} else { // Player 2 win
		ST7735_OutString((char *)Phrases[P2][language]);
	}
	
	while (1) {
		if (button3) { // Language change
			button3 = 0; // Ack
			if (language == English) {
				language = Spanish;
			} else {
				language = English;
			}
			
			// Update displayed message
			ST7735_SetCursor(6, 6);
			ST7735_OutString("                 ");
			if (score1 >= 9) { // Player 1 win
				ST7735_SetCursor(6, 6);
				ST7735_OutString((char *)Phrases[P1][language]);
			} else { // Player 2 win
				ST7735_SetCursor(6, 6);
				ST7735_OutString((char *)Phrases[P2][language]);
			}
		}
	}
}
