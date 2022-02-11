
;***************************************************************************
;* Simple Test program for ATmega 128
;* 
;* File Name            :"SimpleCode.asm"
;* Date                 :15.09.08
;* Version              :1.0
;* Target MCU           :ATmega128
;*
;* DESCRIPTION
;*
;* This program helps understanding the function of several instructions
;*
;***************************************************************************

.include "m128def.inc"

    .CSEG
	.ORG	0x000 ;
	
	JMP	 RESET		


	.ORG	0x0046;
;****************************************************************************
;* Main Program
;****************************************************************************
 	
.def	temp	=	r16

RESET:

;	Initialize R0 ~ R11 with some values for simulation
	ldi 	temp,1				; 
	mov		r0, temp			;	r0 = 1
	ldi		temp,2				;
	mov 	r1, temp			;	r1 = 2
	ldi		temp, 3				;	
	mov		r2, temp			;	r2 = 3
	ldi		temp, 4				;
	mov		r3, temp			;	r3 = 4
	ldi		temp, 5				;
	mov		r4, temp			;	r4 = 5
	ldi 	temp, 6				;
	mov 	r5, temp			;	r5 = 6
	ldi		temp, 7				;
	mov		r6, temp 			;	r6 = 7
	ldi		temp, 8				;
	mov		r7, temp 			;	r7 = 8
	ldi		temp, 9				;
	mov		r8, temp			;	r8 = 9
	ldi		temp, 10			;
	mov		r9, temp			;	r9 = 10
	ldi		temp, 11			;
	mov		r10, temp			;	r10 = 11
	ldi		temp, 12			;
	mov		r11, temp			;	r11 = 12

; 	Single Register Direct Addressing Mode Test	
	com		r0					;
	tst		r1					;
	neg		r1					;
	tst		r1					;	
	inc		r2					;
	dec		r3					;
	tst		r4					;
	clr		r4					;
	tst		r4					;
	neg		r6					;
	lsr		r6					;
	asr		r6					;
	ror		r7					;
	rol		r8					;

; 	Two Register Direct Addressing Mode Test	
	add		r0, r1				;
	sub		r2, r3				;
	and		r4, r5				;
	or		r6, r7				;
	eor		r8, r9				;
	cp		r10, r11			;
	mov		r10, r11			;

;	Data Direct Addressing Mode Test	
	ldi		temp, $ff			;
	mov		r11, temp			;
	sts		$0100, r11			;
	lds		r12, $0100			;
	
;	Data Indirect Addressing Mode Test
	ldi		temp, $fe			;
	mov		r13, temp 			;
	ldi		XH, high($0101)		;
	ldi		XL, low($0101)		;
	st		X, r13				;
	ld		r14, X				;
	
	
forever:
	rjmp	forever				;	