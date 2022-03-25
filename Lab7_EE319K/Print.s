; Print.s
; Student names: Akhil Giridhar & Troy Dutton
; Last modification date: 3/25/2022
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
	PRESERVE8

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"
Count 	EQU 0
Num 	EQU 4
FP		RN 11
LCD_OutDec
	  PUSH {R4-R8, LR}; Preserve registers
	  SUB SP, #8; Allocate space for locals
	  MOV FP, SP; Save frame pointer
	  MOV R4, #0
	  STR R4, [FP, #Count]
	  STR R0, [FP, #Num]
	  ; Store individual digits on stack
	  MOV R5, #10
LCD_Loop  
	  LDR R4, [FP, #Num]
	  CMP R4, #0
	  BEQ LCD_Write
	  UDIV R6, R4, R5; Num / 10
	  MUL R7, R6, R5; Truncated
	  SUB R8, R4, R7; Remainder = Num - Truncated
	  PUSH {R8}
	  STR R6, [FP, #Num]
	  LDR R6, [FP, #Count]
	  ADD R6, #1
	  STR R6, [FP, #Count]
	  B LCD_Loop
	  ; Write digits on stack to LCD
LCD_Write
	  LDR R6, [FP, #Count]
	  CMP R6, #0
	  BNE LCD_Write_Loop
	  ; If Count is 0, the number must be 0
	  MOV R0, #0x30; "0" in ASCII
	  BL ST7735_OutChar; Write to LCD
	  B LCD_OutDec_Done
LCD_Write_Loop
	  POP {R0}; Pop digit
	  ADD R0, #0x30; ASCII offset
	  BL ST7735_OutChar; Write digit to LCD
	  ; Decrement Count
	  LDR R6, [FP, #Count]
	  SUB R6, #1
	  STR R6, [FP, #Count]
	  CMP R6, #0
	  BNE LCD_Write_Loop
LCD_OutDec_Done
	  ADD SP, #8; Remove space used for locals
	  POP {R4-R8, LR}; Restore registers
	  BX LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Asterik EQU 0x2A
Period  EQU 0x2E
Number 	EQU 0
Digits	EQU 4
LCD_OutFix
	 PUSH {R4-R8, LR}; Preserve registers
	 SUB SP, #8; Allocate space for locals
	 
	 
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
