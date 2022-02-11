.CSEG
.def temp = r16
.ORG 0x0000;
JMP RESET;

.ORG 0x0046;

RESET:
	ldi temp, $12;
	sts $0100, temp;

	ldi temp, $34;
	sts $0101, temp;

	ldi temp, $56;
	sts $0102, temp;

	ldi temp, $78;
	sts $0103, temp;
;-----------------------------

	ldi XH, high($0100);
	ldi XL, low($0100);

loop1:
	ld r0, X+;
	ld r1, X+;
	cp r0, r1;
	brge loop2;
	mov r0, r1;

loop2:
	ld r1, X+;
	cp r0, r1;
	brge loop3;
	mov r0, r1;

loop3:
	ld r1, X+;
	cp r0, r1;
	brge loop4;
	mov r0, r1;

loop4:
	st X, r0;

forever:
	rjmp forever;

