// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: Akhil Giridhar & Troy Dutton
// Date Created: 3/6/17 
// Last Modified: 1/11/22  
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
#include <math.h>

// put both EIDs in the next two lines
char EID1[] = "ag78233"; //  ;replace abc123 with your EID
char EID2[] = "thd432"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);          // your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;


// lab video Lab6_voltmeter, Program 6.1
// A simple program that outputs sixteen DAC values. Use this main if you have a voltmeter.
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){                
    Testdata = Inputs[i];
    DAC_Out(Testdata); // your lab 6 solution
    i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}

// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
    time = 6665;  // 1msec, tuned at 80 MHz
    while(time){
      time--;
    }
    n--;
  }
}
// lab video Lab6_static. Program 6.2
// A simple program that outputs sixteen DAC values. 
// Use this main if you do not have a voltmeter. 
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    // bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){                
    now = LaunchPad_Input();
    if((last != now)&&now){
      Testdata = Inputs[i];
      DAC_Out(Testdata); // your lab 6 solution
      i=(i+1)&0x0F;
    }
    last = now;
    DelayMs(25);   // debounces switch
  }
}

uint32_t input; // Holds data from PA5-2
uint8_t switchHandled = 0; // Determines whether a change in inputs has been handled
uint32_t period; // Holds the calculated period for a note
const double frequencies[4] = {246.9, 311.1, 370.0, 415.3}; // Key0=246.9, Key1=311.1, Key2=370.0, Key3=415.3 Hz
     
int main(void){       
  DisableInterrupts();
  TExaS_Init(SIMULATIONGRADER);    // bus clock at 80 MHz
  Key_Init();
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
  EnableInterrupts();
	
	// Enable PF2 for heartbeat
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTF_DIR_R |= 0x4;
	GPIO_PORTF_DEN_R |= 0x4;
	// Set initial input to 0
  while(1){                
		input = Key_In();
		if (input != 0 && switchHandled == 0) { // If a key has been pressed
				switchHandled = 1; // Mark the event as handled
				period = (1/frequencies[(int) log2((double) input)])*1000000; // Period = 1/f µs
				Sound_Start(period);
		} else if (input == 0 && switchHandled == 1) { // If a key is released
			switchHandled = 0; // Clear the handled flag for future events
			Sound_Off(); // Turn off the note
		}
  }             
}


