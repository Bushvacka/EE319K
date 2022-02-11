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
GPIO_PORTE_PDR_R   EQU 0x40024514

        IMPORT  TExaS_Init
        THUMB
        AREA    DATA, ALIGN=2
;global variables go here

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
     MOV R0,#3  ;0 for TExaS oscilloscope, 1 for PORTE logic analyzer, 2 for Lab3 grader, 3 for none
     BL  TExaS_Init ;enables interrupts, prints the pin selections based on EID1 EID2
 ; Your Initialization goes here
	 LDR R0, =SYSCTL_RCGCGPIO_R
	 LDR R1, [R0]
	 ORR R1, #0x10
	 STR R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_DIR_R
	 LDR R1, [R0]
	 ORR R1, #0x20
	 BIC R1, #0x06
	 STR R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_DEN_R
	 LDR R1, [R0]
	 ORR R1, #0x26
	 STR R1, [R0]
	 
	 LDR R0, =GPIO_PORTE_PDR_R
	 LDR R1, [R0]
	 ORR R1, #0x06
	 STR R1, [R0]
	 
	 



loop  
; main engine goes here

	 B    loop
   
  
      
     ALIGN      ; make sure the end of this section is aligned
     END        ; end of file

