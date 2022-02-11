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

	ldi		r16, 10;
	call	FindNegative;

	sts		$0100, r16;

forever:
	rjmp	forever;
;*******************************************************
;	Constant Data:
;*******************************************************
DataArray:
	.db $03, $9f, $25, $0d, $19, $49, $41, $1f, $01, $0f

;***********************************************************
;	Function FindNegative:
;	Input r16 => size of Array , Z => start address of Array
;	Output r16 -> $0100 , number of negative
;************************************************************

FindNegative:
	push r20;
	push r0;
Loop1:
	lpm r20, Z+;
	andi r20, $80;
	cpi r20, $80;
	brne Loop2;
	inc r0;
Loop2:
	dec r16;
	brne Loop1;
	mov r16, r0;
	ret;
