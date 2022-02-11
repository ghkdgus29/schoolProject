.CSEG
.def i = r16
.def sum = r17
.ORG 0x0000;
JMP RESET;

.ORG 0x0046;

RESET:
	ldi i, 1;
	ldi sum, 0;

summation:
	add sum, i;
	subi i, -2;
	cpi i, 20;
	brlt summation;

	sts $0100, sum;

forever:
	rjmp forever;

