// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/18/2022
// Student names: Troy Dutton & Akhil Giridhar
#include <stdint.h>

#define SIZE 16

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

char buffer[SIZE];
uint8_t getI, putI, count;
// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init() {
	getI = putI = count = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
	if (count == SIZE) {
		return 0; // Failure - buffer full
	} else {
		buffer[putI] = data;
		putI = (putI + 1) % SIZE;
		DisableInterrupts();
		count++;
		EnableInterrupts();
		return 1; // Success
	}
}

// *********** Fifo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){
	if (count == 0) {
		return 0; // Failure - buffer empty
	} else {
		*datapt = buffer[getI];
		getI = (getI + 1) % SIZE;
		DisableInterrupts();
		count--;
		EnableInterrupts();
		return 1; // Success
	}
}

	



