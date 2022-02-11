.CSEG
.def count = r16
.def temp = r17
.ORG 0x0000;
JMP RESET;

.ORG 0x0046;

RESET:
	ldi temp, 3;
	sts $0100, temp;

	ldi temp, $84;
	sts $0101, temp;

	ldi temp, $25;
	sts $0102, temp;

	ldi temp, $96;
	sts $0103, temp;

;-------------------------------

	ldi ZH, high($0100);
	ldi ZL, low($0100);
	ld count, Z+;
	mov r1, count;

loop1:
	ld temp, Z+;
	andi temp, $80;
	cpi temp, $80;
	breq loop2;

	dec r1;

loop2:
	dec count;
	brne loop1;
end:
	st Z, r1;

forever:
	rjmp forever;

