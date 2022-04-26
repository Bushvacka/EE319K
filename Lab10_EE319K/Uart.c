// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  
// Daniel Valvano
// November 11, 2021

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "Fifo.h"
#include "UART.h"
#include "../inc/tm4c123gh6pm.h"
#define LF  0x0A

// Initialize UART1 on PC4 PC5
// Baud rate is 1000 bits/sec
// Receive interrupts and FIFOs are used on PC4
// Transmit busy-wait is used on PC5.
void UART_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCUART_R |= 0x2; // activate UART1
  SYSCTL_RCGCGPIO_R |= 0x4; // activate port C
  delay = SYSCTL_RCGCGPIO_R; // Wait for clock to stabilize
  UART1_CTL_R &= ~0x0001; // disable UART
  UART1_IBRD_R = 5000; // IBRD=int(80000000/(16*1000)) = 5,000
  UART1_FBRD_R = 0; // FBRD = 0
	UART1_IM_R |= 0x10;
	UART1_IFLS_R |= 0x10;
	UART1_IFLS_R &= ~(0x28);
	NVIC_PRI1_R |= 0xE00000; // Priority 7
	NVIC_EN0_R |= 0x40; // Enable interrupt 6
  UART1_LCRH_R = 0x0070;  // 8-bit length, enable FIFO
  UART1_CTL_R = 0x0301;   // enable RXE, TXE and UART
  GPIO_PORTC_AFSEL_R |= 0x30; // alt funct on PC4-5
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
  GPIO_PORTC_DEN_R |= 0x30;   // digital I/O on PC4-5
  GPIO_PORTC_AMSEL_R &= ~0x30; // No analog on PC4-5
}

//------------UART_InChar------------
// Receive new input, interrupt driven
// Input: none
// Output: return read ASCII code from UART, 
// Reads from software FIFO (not hardware)
// Either return 0 if no data or wait for data (your choice)
unsigned char UART1_InChar(void){
	while((UART1_FR_R&0x10) != 0){} // wait until RXFE is 0
  return((uint8_t)(UART1_DR_R&0xFF));
}
//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until LR is received
//    or until max length of the string is reached.
// Reads from software FIFO (not hardware)
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
void UART1_InMessage(char *bufPt){
	while (*(bufPt - 1) != '>') {
		*bufPt = UART1_InChar();
		bufPt++;
	}
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
// busy-wait; if TXFF is 1, then wait
// Transmit busy-wait is used on PC5.
void UART_OutChar(char data){
	while((UART1_FR_R&0x20) != 0){}  // wait until TXFF is 0
  UART1_DR_R = data;
}

// hardware RX FIFO goes from 7 to 8 or more items
// Receive interrupts and FIFOs are used on PC4
extern void UART1_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x2; // Heartbeat
	UART1_ICR_R = 0x10; // Ack
	for (int i = 0; i < 8; i++) {
		Fifo_Put(UART1_InChar());
	}
}

//------------UART_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART_OutString(char *pt){
  while (*pt != 0) {
		UART_OutChar(*pt);
		pt++;
	}
}

