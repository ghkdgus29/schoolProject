.CSEG
.ORG 0x0000;
JMP RESET;

.ORG 0x0046;

RESET:
	ldi r16, $73;
	sts $0100, r16;

	ldi r17, $74;
	sts $0101, r17;
;-----------------------------

	ldi XH, high(value1);
	ldi XL, low(value1);

	ldi YH, high(value2);
	ldi YL, low(value2);

	ld r0, X;
	ld r1, Y;

	st X, r1;
	st Y, r0;

forever:
	rjmp forever;

.DSEG
value1:
	.byte 1
value2:
	.byte 1