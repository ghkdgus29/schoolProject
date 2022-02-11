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
	ldi		r29, 7;

	call	FindMost1;

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
;	Output r16 -> $0100 , data with most 1
;************************************************************

FindMost1:
	push r20;
	push r21;
	push r0;
	push r1;
	push r2;
	push r3;
	push r29;

	dec r16;
	lpm r20, Z+;
	mov r3, r20;
	call Find1;
	mov r1, r0;
Loopstart:
	clr r0;
	lpm r20, Z+;
	mov r2, r20;
	call Find1;
	cp r0, r1;
	brlt Skip;
	mov r1, r0;
	mov r3, r2;
Skip:
	dec r16;
	brne Loopstart;
	mov r16, r3;
	pop r29;
	pop r3;
	pop r2;
	pop r1;
	pop r0;
	pop r21;
	pop r20;
	ret;

;*****************************************************
Find1:
	mov r21, r20;
	andi r21, 1;
	cpi r21, 1;
	brne Loop1;
	inc r0;
Loop1:
	lsr r20;
	mov r21, r20;
	andi r21, 1;
	cpi r21, 1;
	brne Loop2;
	inc r0;
Loop2:
	dec r29;
	brne Loop1;
	ldi r29, 7;
	ret;
	
