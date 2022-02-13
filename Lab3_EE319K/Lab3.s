;****************** Lab3.s ***************
; Program written by: Put your names here
; Date Created: 2/4/2017
; Last Modified: 1/10/2022
; Brief description of the program
;   The LED toggles at 2 Hz and a varying duty-cycle
; Hardware connections (External: Two buttons and one LED)
;  Change is Button input  (1 means pressed, 0 means not pressed)
;  Breathe is Button input  (1 means pressed, 0 means not pressed)
;  LED is an output (1 activates external LED)
; Overall functionality of this system is to operate like this
;   1) Make LED an output and make Change and Breathe inputs.
;   2) The system starts with the the LED toggling at 2Hz,
;      which is 2 times per second with a duty-cycle of 30%.
;      Therefore, the LED is ON for 150ms and off for 350 ms.
;   3) When the Change button is pressed-and-released increase
;      the duty cycle by 20% (modulo 100%). Therefore for each
;      press-and-release the duty cycle changes from 30% to 70% to 70%
;      to 90% to 10% to 30% so on
;   4) Implement a "breathing LED" when Breathe Switch is pressed:
; PortE device registers
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
; Program constants
CLOCK_SPEED EQU 800000

        IMPORT  TExaS_Init
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here
DUTY_CYCLE SPACE 1
COUNT SPACE 4
COUNT_COMPLEMENT SPACE 4
FREQUENCY SPACE 1

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID1
EID1   DCB "ag78233",0  ;replace ABC123 with your EID
       EXPORT EID2
EID2   DCB "thd432",0  ;replace ABC123 with your EID
       ALIGN 4

     EXPORT  Start

Start
; TExaS_Init sets bus clock at 80 MHz, interrupts, ADC1, TIMER3, TIMER5, and UART0
     MOV R0,#2  ;0 for TExaS oscilloscope, 1 for PORTE logic analyzer, 2 for Lab3 grader, 3 for none
     BL  TExaS_Init ;enables interrupts, prints the pin selections based on EID1 EID2
 ; Your Initialization goes here
	 ; Enable Port E Clock
	 LDR R0, =SYSCTL_RCGCGPIO_R
	 LDR R1, [R0]
	 ORR R1, #0x10
	 STR R1, [R0]
	 NOP
	 NOP
	 ; Set PE1 & PE2 as inputs
	 ; Set PE5 as output
	 LDR R0, =GPIO_PORTE_DIR_R
	 LDR R1, [R0]
	 ORR R1, #0x20
	 BIC R1, #0x06
	 STR R1, [R0]
	 ; Digitally enable PE1, PE2, PE5
	 LDR R0, =GPIO_PORTE_DEN_R
	 LDR R1, [R0]
	 ORR R1, #0x26
	 STR R1, [R0]
	 ; Initialize duty cycle to 30%
	 LDR R0, =DUTY_CYCLE
	 MOV R1, #30
	 STRB R1, [R0]
	 ; Initialize frequency to 2
	 LDR R0, =FREQUENCY
	 MOV R1, #2
	 STRB R1, [R0]
	 ; Calculate initial count
	 BL calculate_count
	 LDR R0, =GPIO_PORTE_DATA_R
loop  
; main engine goes here
	 LDR R1, [R0]
	 ; Change Button
	 MOV R2, #0x02
	 ANDS R2, R1
	 BNE change
	 ; Breathe Button
	 MOV R3, #0x04
	 ANDS R3, R1
	 BNE breathe
	 
	 
	 ; Turn on the LED
	 LDR R1, [R0]
	 ORR R1, #0x20
	 STR R1, [R0]
	 ; Delay on
	 LDR R4, =COUNT
	 LDR R4, [R4]
	 BL delay

	 ; Turn off the LED
	 LDR R1, [R0]
	 BIC R1, #0x20
	 STR R1, [R0]
	 ; Delay off
	 LDR R4, =COUNT_COMPLEMENT
	 LDR R4, [R4]
	 BL delay
	 B loop

change
	 ; Turn off LED
	 LDR R1, [R0]
	 BIC R1, #0x20
	 STR R1, [R0]
	 ; Increment DUTY_CYCLE by 20%
	 LDR R4, =DUTY_CYCLE
	 LDRB R5, [R4]
	 ADD R5, #20
	 ; Check if DUTY_CYCLE > 100%
	 SUBS R6, R5, #100
	 BPL rollover
	 STRB R5, [R4]
	 BL calculate_count
	 B c_loop
rollover
	 STRB R6, [R4]
	 BL calculate_count
	 
c_loop
	 ; Looping until the change button is released
	 LDR R1, [R0]
	 MOV R2, #0x02
	 ANDS R2, R1
	 BNE c_loop
	 B loop

calculate_count
	 ; Load duty cycle
	 LDR R4, =DUTY_CYCLE
	 LDRB R5, [R4]; Duty Cycle
	 MOV R6, #100
	 SUBS R7, R6, R5; Duty Cycle Complement
	 
	 ; Calculate duty_cycle * clock_speed
	 LDR R6, =CLOCK_SPEED

	 MUL R5, R6
	 MUL R7, R6
	 
	 ; Divide by time
	 LDR R6, =FREQUENCY
	 LDR R6, [R6]
	 
	 UDIV R5, R6
	 UDIV R7, R6
	 
	 ; Calculate Count
	 MOV R6, #4; # of delay cycles 
	 
	 UDIV R5, R6; 
	 UDIV R7, R6
	 
	 ; Store counts
	 LDR R4, =COUNT
	 STR R5, [R4]
	 LDR R4, =COUNT_COMPLEMENT
	 STR R7, [R4]
	 BX LR
	 
delay
	 ADDS R4, #0
	 BNE delay_loop
	 BX LR
delay_loop
	 SUBS R4, #1
	 BNE delay_loop
	 BX LR

breathe
	 ; Set the frequency to 100
	 LDR R4, =FREQUENCY
	 MOV R5, #100
	 STRB R5, [R4]
	 
	 ; Set the initial duty cycle to 0%
	 LDR R4, =DUTY_CYCLE
	 LDRB R5, [R4]
	 MOV R9, R5; Saving duty cycle
	 AND R5, #0x00
	 STRB R5, [R4]
	
breathe_up
	 BL calculate_count
	 ; Turn on the LED
	 LDR R1, [R0]
	 ORR R1, #0x20
	 STR R1, [R0]
	 ; Delay on
	 LDR R4, =COUNT
	 LDR R4, [R4]
	 BL delay
	 
	 ; Check if breathe button is still pressed
	 LDR R1, [R0]
	 MOV R3, #0x04
	 ANDS R3, R1
	 BEQ breathe_end

	 ; Turn off the LED
	 LDR R1, [R0]
	 BIC R1, #0x20
	 STR R1, [R0]
	 ; Delay off
	 LDR R4, =COUNT_COMPLEMENT
	 LDR R4, [R4]
	 BL delay
	 
	 ; Check if breathe button is still pressed
	 LDR R1, [R0]
	 MOV R3, #0x04
	 ANDS R3, R1
	 BEQ breathe_end
	 
	 ; Increment DUTY_CYCLE
	 LDR R4, =DUTY_CYCLE
	 LDRB R5, [R4]
	 ADD R5, #1
	 CMP R5, #101
	 STRB R5, [R4]
	 BCC breathe_up

	 SUB R5, #1
	 STRB R5, [R4]

	 
breathe_down
	 BL calculate_count
	 ; Turn on the LED
	 LDR R1, [R0]
	 ORR R1, #0x20
	 STR R1, [R0]
	 ; Delay on
	 LDR R4, =COUNT
	 LDR R4, [R4]
	 BL delay
	 
	 ; Check if breathe button is still pressed
	 LDR R1, [R0]
	 MOV R3, #0x04
	 ANDS R3, R1
	 BEQ breathe_end

	 ; Turn off the LED
	 LDR R1, [R0]
	 BIC R1, #0x20
	 STR R1, [R0]
	 ; Delay off
	 LDR R4, =COUNT_COMPLEMENT
	 LDR R4, [R4]
	 BL delay
	 
	 ; Check if breathe button is still pressed
	 LDR R1, [R0]
	 MOV R3, #0x04
	 ANDS R3, R1
	 BEQ breathe_end
	 
	 ; Decrement DUTY_CYCLE
	 LDR R4, =DUTY_CYCLE
	 LDRB R5, [R4]
	 SUB R5, #1
	 CMP R5, #0
	 STRB R5, [R4]
	 BNE breathe_down
	 ADD R5, #1
	 STR R5, [R4]
	 B breathe_end
	 
breathe_end
	 LDR R4, =FREQUENCY
	 MOV R5, #2
	 STRB R5, [R4]
	 
	 ; Restore duty cycle
	 LDR R4, =DUTY_CYCLE
	 STRB R9, [R4]
	 BL calculate_count
	 B loop
	 
	 
	 
	 
	

     ALIGN      ; make sure the end of this section is aligned
     END        ; end of file
