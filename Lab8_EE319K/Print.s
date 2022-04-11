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

  
Count 	EQU 0
Num 	EQU 4
FP		RN 11
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
LCD_OutDec
	  PUSH {R4-R8, LR}; Preserve registers
	  SUB SP, #8; Allocate space for locals
	  MOV FP, SP; Save frame pointer
	  MOV R4, #0
	  STR R4, [FP, #Count]
	  STR R0, [FP, #Num]
	  MOV R5, #10
	  ; Push individual digits on stack
LCD_Dec_Loop  
	  LDR R4, [FP, #Num]
	  CMP R4, #0
	  BEQ LCD_Dec_Write
	  UDIV R6, R4, R5; Num / 10
	  MUL R7, R6, R5; Truncated Num
	  SUB R8, R4, R7; Remainder = Num - Truncated Num
	  PUSH {R8}
	  STR R6, [FP, #Num]
	  LDR R6, [FP, #Count]
	  ADD R6, #1
	  STR R6, [FP, #Count]
	  B LCD_Dec_Loop
	  ; Write digits to LCD
LCD_Dec_Write
	  LDR R6, [FP, #Count]
	  CMP R6, #0
	  BNE LCD_Dec_Write_Loop
	  ; If Count is 0, the number must be 0
	  MOV R0, #0x30; "0" in ASCII
	  BL ST7735_OutChar; Write to LCD
	  B LCD_Dec_Done
LCD_Dec_Write_Loop
	  POP {R0}; Pop digit
	  ADD R0, #0x30; ASCII offset
	  BL ST7735_OutChar; Write digit to LCD
	  ; Decrement Count
	  LDR R6, [FP, #Count]
	  SUB R6, #1
	  STR R6, [FP, #Count]
	  CMP R6, #0
	  BNE LCD_Dec_Write_Loop
LCD_Dec_Done
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
Zero	EQU 0x30
Asterik EQU 0x2A
Period  EQU 0x2E
; Already delared
;FP		RN 11
;Count 	EQU 0
;Num 	EQU 4
Index	EQU 8
LCD_OutFix
	 PUSH {R4-R8, LR}; Preserve registers
	 SUB SP, #12; Allocate space for locals
	 MOV FP, SP; Save frame pointer
	 MOV R4, #0
	 STR R4, [FP, #Index]
	 STR R4, [FP, #Count]
	 STR R0, [FP, #Num]
	 
	 ; Check for edge cases
	 MOV R4, #Zero
	 CMP R0, #0
	 BEQ LCD_Edge; Num is 0
	 MOV R4, #Asterik
	 MOV R5, #9999
	 CMP R0, R5
	 BHI LCD_Edge; Num is >9999
	 MOV R5, #10
	 
	 ; Push digits onto the stack
LCD_Fix_Loop
	 LDR R4, [FP, #Num]
	 CMP R4, #0
	 BEQ LCD_Fix_Write
	 UDIV R6, R4, R5; Num / 10
	 MUL R7, R6, R5; Truncated Num
	 SUB R8, R4, R7; Remainder = Num - Truncated Num
	 STR R6, [FP, #Num]; Update Num
	 PUSH {R8}; Push digit
	 LDR R4, [FP, #Count]
	 ADD R4, #1
	 STR R4, [FP, #Count]
	 B LCD_Fix_Loop
	 ; 8.965
	 ; 01234
	 ; Write digits in fixed-point format
LCD_Fix_Write
	 LDR R4, [FP, #Index]
	 CMP R4, #1
	 BNE LCD_Write_Skip
	 ; Index 1 -> Write "."
	 MOV R0, #Period
	 BL ST7735_OutChar
	 ADD R4, #1
	 STR R4, [FP, #Index]
	 B LCD_Fix_Write
LCD_Write_Skip
	 ; If (5 - Count) - Index > 0 write "0", otherwise write a digit from the stack
	 ; If Count == 4, always write a digit
	 MOV R6, #5
	 LDR R5, [FP, #Count]
	 CMP R5, #4
	 BEQ LCD_Digit
	 SUB R6, R5
	 SUB R6, R4
	 CMP R6, #0
	 BGT LCD_Empty
LCD_Digit
	 POP {R0}
	 ADD R0, #0x30
	 BL ST7735_OutChar; Write digit
	 SUB R5, #1; Decrement count
	 STR R5, [FP, #Count]
	 ADD R4, #1; Increment index
	 STR R4, [FP, #Index]
	 CMP R4, #5
	 BLO LCD_Fix_Write
	 B LCD_Fix_Done
LCD_Empty
	 MOV R0, #Zero
	 BL ST7735_OutChar; Write "0"
	 ADD R4, #1; Increment index
	 STR R4, [FP, #Index]
	 B LCD_Fix_Write; No need to check index range because edge case covers "0.000"
	 ; Write "0.000" or "*.***"
LCD_Edge
	 MOV R0, R4
	 BL ST7735_OutChar
	 MOV R0, #Period
	 BL ST7735_OutChar
	 MOV R0, R4
	 BL ST7735_OutChar
	 MOV R0, R4
	 BL ST7735_OutChar
	 MOV R0, R4
	 BL ST7735_OutChar	 
LCD_Fix_Done
	 ADD SP, #12; Deallocate local space
	 POP {R4-R8, LR}; Restore registers
     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
