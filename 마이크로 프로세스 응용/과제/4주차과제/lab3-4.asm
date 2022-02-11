.CSEG
.def i_LO = r16
.def i_HI = r17
.def sum_LO = r18
.def sum_HI	= r19
.ORG 0x0000;
JMP RESET;

.ORG 0x0046;

RESET:
	ldi i_HI, HIGH(2);
	ldi i_LO, LOW(2);

	ldi sum_HI, 0;
	ldi sum_LO, 0;

summation:
	add sum_LO, i_LO;
	adc sum_HI, i_HI;

	subi i_LO, LOW(-2);
	sbci i_HI, HIGH(-2);

	cpi i_LO, LOW(101);
	ldi r30, HIGH(101);
	cpc i_HI, r30;
	brlt summation;

	sts $0100, sum_LO;
	sts $0101, sum_HI;

forever:
	rjmp forever;

