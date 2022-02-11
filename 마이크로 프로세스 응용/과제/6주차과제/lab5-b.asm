.include "m128def.inc"
.CSEG
	.ORG	0x0000;
	JMP		RESET
;*******************************************************
; Main Program
;*******************************************************
.def	temp	=r16

.def	segen	=r18
.def	segcnt	=r19
	.ORG	0x0046;
RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;

	ldi temp, $FF;
	out DDRC, temp;

	ldi temp, $0F;
	sts DDRG, temp;

	ldi	segen, $08;
	

forever:
	sts PORTG, segen;
	
	ldi ZH, HIGH(2*segdata);
	ldi ZL, LOW(2*segdata);

	ldi segcnt, 10;

loop10:
	lpm r0, Z+;
	out PORTC, r0;
	rcall delay1sec;
	subi segcnt, 1;
	brne loop10;

	lsr segen;
	breq init;
	rjmp forever;
init:
	ldi segen, $08;
	rjmp forever;

segdata:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

;--- 시간 지연 함수
delay1sec:
	ldi r20, 80;
	ldi r21, 255;
	ldi r22, 255;
loop:
	dec r22;
	brne loop;
	dec r21;
	brne loop;
	dec r20;
	brne loop;
	ret;	
	
	




