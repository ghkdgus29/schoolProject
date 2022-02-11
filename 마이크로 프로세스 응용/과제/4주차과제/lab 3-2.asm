
;------- register definition
.def count = r1
.def num1_1 = r2
.def num1_2 = r3
.def value = r16
.def temp = r17
.def seven = r18

	.CSEG
	.ORG	0x0000;
	JMP		RESET;

	.ORG	0x0046;

RESET:
	ldi r31, 3;
	sts $0100, r31;

	ldi r31, $84;
	sts $0101, r31;

	ldi r31, $25;
	sts $0102, r31;

	ldi r31, $96;
	sts $0103, r31;

;initialization------------------------

	ldi ZH, HIGH($0100);
	ldi ZL, LOW($0100);
	
	ldi seven, 7;
	ld count, Z+;

loop1:
	ld value, Z+
	mov temp, value;
	andi temp, 1;
	cpi temp, 1; 
	brne loop2;
	inc num1_1;

loop2:
	lsr value;
	mov temp, value;
	andi temp, 1;
	cpi temp, 1;
	brne loop3;
	inc num1_1;

loop3:
	dec seven;
	brne loop2;
	mov r0, value;

between:
	ld value, Z+
	mov temp, value;
	andi temp, 1;
	cpi temp, 1; 
	brne loop4;
	inc num1_2;

loop4:
	ldi seven, 7;
	lsr value;
	mov temp, value;
	andi temp, 1;
	cpi temp, 1;
	brne loop5;
	inc num1_2;

loop5:
	dec seven;
	brne loop4;
	mov r1, value;

	cp r0, r1;
	brge loop6;
	mov r0, r1;

loop6:
	subi count, 1;
	dec count;
	brne between;

	st Z, r0;

	
forever:
	rjmp forever;


