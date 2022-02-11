.include "m128def.inc"
.CSEG
	.ORG	0x0000;
	JMP		RESET
;*******************************************************
; Main Program
;*******************************************************
	.ORG	0x0046;
.def	temp	=r16
.def	leddata	=r17

RESET:
	ldi	temp, low(RAMEND);
	out	SPL, temp;
	ldi	temp, high(RAMEND);
	out	SPH, temp;

	ldi	temp, $FF;
	out DDRA, temp;
	ldi leddata, $80;

forever:
	out PORTA, leddata;
	rcall delay1sec;
	ror	leddata;
	rjmp forever;

;---- 시간 지연 함수
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