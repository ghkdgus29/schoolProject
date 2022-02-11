.include "m128def.inc"
.CSEG
	.ORG	0x0000;
	JMP		RESET
;*******************************************************
; Main Program
;*******************************************************
	.ORG	0x0046;
RESET:
	ldi		r16,low(RAMEND);
	out		SPL,r16;
	ldi		r16,high(RAMEND);
	out		SPH,r16;

	ldi		ZH, HIGH(2*DataArray);
	ldi		ZL, LOW(2*DataArray);

	ldi		XH, HIGH($0100);
	ldi		XL, LOW($0100);
	ldi		r18, 10;

	call	ConvI2A_Array;

forever:
	rjmp	forever;
;*******************************************************
;	Constant Data:
;*******************************************************
DataArray:
	.db $03, $9f, $25, $0d, $19, $49, $41, $1f, $01, $0f


;**********************************************************
; Function ConvI2A_Array:
; Input r18 => Size of Number Array, Z => Start address of Number Array, 
;		X => start address of ASCII String
; Output None
;***********************************************************

ConvI2A_Array:
Loop:
	lpm r16, Z+;
	call Int2Ascii;
	st X+, r0;
	st X+, r1;
	dec r18;
	brne Loop;
	ret;



;***********************************************************
;	Function Int2Ascii:
;	Input r16 => Integer Number 
;	Output r0 -> $0100 , First Ascii, r1 -> $0101, Second Ascii
;************************************************************

;register definition
.def HI = r20;
.def LO = r21;

Int2Ascii:
	push r20;
	push r21;
	
	mov HI, r16;
	mov LO, r16;

	andi HI, $f0;
	lsr HI;
	lsr HI;
	lsr HI;
	lsr HI;
	subi HI, -$30; 

	andi LO, $0f;
	subi LO, -$30;
	
	mov r0, HI;
	mov r1, LO;

	pop r21;
	pop r20;
	ret;
