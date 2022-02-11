	.CSEG
	.ORG	0x0000;
	JMP		RESET;

	.ORG	0x0046;

RESET:
	ldi r31, $12;
	sts $0100, r31;

	ldi r31, $34;
	sts $0101, r31;

	ldi r31, $56;
	sts $0102, r31;

	ldi r31, $78;
	sts $0103, r31;

;initialization------------------------

	ldi XH, HIGH($0100);
	ldi XL, LOW($0100); 
	
phase1:
	ld r0, X+;
	ld r1, X+;
	cp r0, r1;
	brge phase2;
	mov r0, r1;

phase2:
	ld r1, X+;
	cp r0, r1;
	brge phase3;
	mov r0, r1;

phase3:
	ld r1, X+;
	cp r0, r1;
	brge final;
	mov r0, r1;

final:
	st X, r0;

forever:
	rjmp forever;




