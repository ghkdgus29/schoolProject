.include "m128def.inc"
.def sum_LO = r20;
.def sum_HI = r21;
.CSEG
	.ORG 0x0000;
	JMP RESET;

	.ORG 0x0046;
RESET:
	ldi r16, low(RAMEND);
	out SPL, r16;
	ldi r16, high(RAMEND);
	out SPH, r16;

	ldi ZH, high(2*dataarray);
	ldi ZL, low(2*dataarray);

	ldi r16, 10;
	call ArraySum;

	sts $0100, r16;
	sts $0101, r17;

forever:
	rjmp forever;

DataArray:
	.db $03, $9f, $25, $0d, $19, $49, $41, $1f, $01, $0f


;-------------------------------
;Arraysum ÇÔ¼ö , input r16, Z ,  output r16, r17
;--------------------------------
ArraySum:
	push r20;
	push r21;
	push r22;
	push r23;

loop:
	lpm r0, Z+;

	add sum_LO, r0;
	adc sum_HI, r22;
	dec r16;
	brne loop;

	mov r16, sum_LO;
	mov r17, sum_HI;

	pop r23;
	pop r22;
	pop r21;
	pop r20;
	ret;

