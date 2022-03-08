// Lab5.c starter program EE319K Lab 5, Spring 2022
// Runs on TM4C123
// Put your names here
// Last Modified: 1/11/2021

/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)
#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
// put both EIDs in the next two lines
char EID1[] = "thd432"; //  ;replace abc123 with your EID
char EID2[] = "ag78233"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);

#define time 150

typedef enum StateId {SouthG, SouthY, StopS, WestG, WestY, StopW, Walk, Flash1, Flash2, Flash3, Flash4, Flash5} StateId_t;

struct State{
	uint8_t Out_Lights;
	uint8_t Out_Walk;
	uint32_t Wait;
	StateId_t Next[8];
};

typedef const struct State StateType;

StateType FSM[12]={
  {0x21, 0x2, time, {SouthG, SouthY, SouthG, SouthY, SouthY, SouthY, SouthY, SouthY}},
	{0x22, 0x2, time, {StopS, StopS, StopS, StopS, StopS, StopS, StopS, StopS}},
	{0x24, 0x2, time, {StopS, WestG, SouthG, WestG, Walk, WestG, Walk, WestG}},
	{0xC, 0x2, time, {WestG, WestG, WestY, WestY, WestY, WestY, WestY, WestY}},
	{0x14, 0x2, time, {StopW, StopW, StopW, StopW, StopW, StopW, StopW, StopW}},
	{0x24, 0x2, time, {StopW, WestG, SouthG, SouthG, Walk, Walk, Walk, Walk}},
	{0x24, 0xE, time, {Walk, Flash1, Flash1, Flash1, Walk, Flash1, Flash1, Flash1}},
	{0x24, 0x2, time, {Flash2, Flash2, Flash2, Flash2, Flash2, Flash2, Flash2, Flash2}},
	{0x24, 0x0, time, {Flash3, Flash3, Flash3, Flash3, Flash3, Flash3, Flash3, Flash3}},
	{0x24, 0x2, time, {Flash4, Flash4, Flash4, Flash4, Flash4, Flash4, Flash4, Flash4}},
	{0x24, 0x0, time, {Flash5, Flash5, Flash5, Flash5, Flash5, Flash5, Flash5, Flash5}},
	{0x24, 0x2, time, {SouthG, WestG, SouthG, SouthG, Walk, WestG, SouthG, SouthG}}
};

int main(void){ 
  DisableInterrupts();
  TExaS_Init(NONE);
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize system
	// Clock
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x32;
	delay = SYSCTL_RCGCGPIO_R;
	
	// Port B
	GPIO_PORTB_DIR_R |= 0x3F;
	GPIO_PORTB_DEN_R |= 0x3F;
	
	// Port E
	GPIO_PORTE_DIR_R &= ~0x7;
	GPIO_PORTE_DEN_R |= 0x7;
	
	// Port F
	GPIO_PORTF_DIR_R |= 0xE;
	GPIO_PORTF_DEN_R |= 0xE;
	
	uint32_t CS = SouthG;
	uint32_t Input;
  EnableInterrupts();  
  while(1){
		GPIO_PORTB_DATA_R = FSM[CS].Out_Lights;
    GPIO_PORTF_DATA_R = FSM[CS].Out_Walk;
		SysTick_Wait10ms(FSM[CS].Wait); 
		Input = GPIO_PORTE_DATA_R;
    CS = FSM[CS].Next[Input];
  }
}


/*
typedef enum StateId {S5, S6, S10, S9} StateId_t;
#define T1sec 10   // Lower this to 10 to run simulation 
                    // and see changes in Logic Analyzer
struct State{
  uint8_t Out;           
  uint32_t Delay; 
  StateId_t Next[4]; 
};
typedef const struct State StateType;

StateType FSM[4]={
  {5, T1sec, S5, S6, S9, S5},
  {6, T1sec, S6,S10, S5, S6},
  {10,T1sec,S10, S9, S6,S10},
  {9, T1sec, S9, S5,S10, S9}
};
StateId_t CS;  // Current State
#define STEPPER  (*((volatile uint32_t *)0x4000703C))
#define INPUT  (*((volatile uint32_t *)0x40006300))
uint32_t Input;
void Initialize(void);


int main(void){
  Initialize();
  CS = S5; // Set initial state;
  while(1){
    STEPPER = FSM[CS].Out;           // Output for Current State
    SysTick_Wait10ms(FSM[CS].Delay); // Dwell in Current State
    Input = (INPUT>>6);              // Read Input: 0,1,2,3
    CS = FSM[CS].Next[Input];        // Next State
  }
}

void Initialize(){
	PLL_Init(Bus80MHz);      // Program 4.6
  SysTick_Init();          // Program 4.7
  SYSCTL_RCGCGPIO_R |= 0x0C;  // 1) port C,D clock enabled              
  GPIO_PORTD_LOCK_R = 0x4C4F434B;  // unlock GPIO Port D
  GPIO_PORTD_CR_R = 0xFF;          // allow changes to PD7-0
  GPIO_PORTC_AMSEL_R &= ~0xC0;      // 3) disable analog function
  GPIO_PORTD_AMSEL_R &= ~0x0F;      // 3) disable analog function
  GPIO_PORTC_PCTL_R &= ~0xFF000000; // 4) GPIO
  GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // 4) GPIO
  GPIO_PORTD_DIR_R |= 0x0F;   // 5) make PD3-0 out
  GPIO_PORTC_DIR_R &= ~0xC0;  //    make PC7-6 input
  GPIO_PORTC_AFSEL_R &= ~0xC0;// 6) disable alt func on PC7-6
  GPIO_PORTD_AFSEL_R &= ~0x0F;// 6) disable alt func on PD3-0
  GPIO_PORTD_DR8R_R |= 0x0F;  // enable 8 mA drive on PD7-6,3-0
  GPIO_PORTC_DEN_R |= 0xC0;   // 7) enable digital I/O on PC7-6
  GPIO_PORTD_DEN_R |= 0x0F;   // 7) enable digital I/O on PD3-0
}
*/



