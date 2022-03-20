// Key.c
// This software configures the off-board piano keys
// Lab 6 requires a minimum of 4 keys, but you could have more
// Runs on LM4F120 or TM4C123
// Program written by: Akhil Giridhar & Troy Dutton
// Date Created: 3/6/17 
// Last Modified: 1/11/22  
// Lab number: 6
// Hardware connections
// Keys: PA5-2
// DAC: PB5-0

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// **************Key_Init*********************
// Initialize piano key inputs on PA5-2
// Input: none 
// Output: none
void Key_Init(void){
	// Initialize PORTA clock
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x1;
	delay = SYSCTL_RCGCGPIO_R;
	// Enable PA5-2
	GPIO_PORTA_DIR_R &= ~0x3C;
	GPIO_PORTA_DEN_R |= 0x3C;
}
// **************Key_In*********************
// Input from piano key inputs on PA5-2
// Input: none 
// Output: 0 to 15 depending on keys
//   0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2, 0x08 is just Key3
uint32_t Key_In(void){ 
	return (GPIO_PORTA_DATA_R >> 2);
}

//------------LaunchPad_Init------------
// Initialize Switch input and LED output
// Input: none
// Output: none
void LaunchPad_Init(void){
// implement if needed
}


//------------LaunchPad_Input------------
// Input from Switches, 
// Convert hardware negative logic to software positive logic 
// Input: none
// Output: 0x00 none
//         0x01 SW2 pressed (from PF4)
//         0x02 SW1 pressed (from PF1)
//         0x03 both SW1 and SW2 pressed
uint8_t LaunchPad_Input(void){
// implement if needed
  return 0; // replace   
}
//------------LaunchPad__Output------------
// Output to LaunchPad LEDs 
// Positive logic hardware and positive logic software
// Input: 0 off, bit0=red,bit1=blue,bit2=green
// Output: none
void LaunchPad_Output(uint8_t data){  // write three outputs bits of PORTF
// implement if needed

}

