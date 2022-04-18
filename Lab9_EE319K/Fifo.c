// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 11/11/2021 
// Student names: Troy Dutton
#include <stdint.h>

#define SIZE 17

static char buffer[SIZE];
static char get, put;
// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void Fifo_Init() {
	get = 0;
	put = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t Fifo_Put(char data){
	if ((put + 1) % SIZE != get) {
		buffer[put] = data;
		put = (put + 1) % SIZE;
		return 1; // Success
	} else {
		return 0; // Failure - buffer full
	}
}

// *********** Fifo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t Fifo_Get(char *datapt){
	if (get != put) {
		*datapt = buffer[get];
		get = (get + 1) % SIZE;
		return 1; // Success
	} else {
		return 0; // Failure - buffer empty
	}
}

	




