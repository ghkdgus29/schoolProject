.include "m128def.inc"

.def	temp	=	r16
.def	leddata	=	r17
.equ	FCLK	=	16000000

.CSEG
	.ORG 0x000;
	jmp	RESET;

	.ORG 0x000A;
	jmp EXT_INT4;		외부 인터럽트 4번이 입력될 경우
	jmp EXT_INT5;		외부 인터럽트 5번이 입력될 경우

	.ORG 0x0046;

; main program

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;				스택포인터 설정

	cli;				인터럽트 disable
	
	ldi temp, $00;
	sts EICRA, temp;			

	ldi temp, $0A;
	sts EICRB, temp;			외부 인터럽트 4번 5번을 falling edge 인터럽트로 사용

	ldi temp, $30;
	out EIMSK, temp;			외부 인터럽트 4번 5번을 사용가능하도록 설정	
	
	sei;				인터럽트 on
	
	ldi temp, $FF;
	out DDRA, temp;				A포트 모두 출력으로 사용
	
	ldi leddata, $FF;
	out PORTA, leddata;			A포트 led를 모두 점등
	
forever:
	rjmp forever;			

EXT_INT4:					// 외부 인터럽트 4번, 1번 스위치를 누를 경우 동작
	 cli;				인터럽트 disable

	 ldi temp, $FF;
	 out PORTA, temp;		  A포트 led 모두 점등

	 ldi temp, 200;
	 call delay_Tms;		 debouncing을 위한 지연함수

	 sei;				인터럽트 on
	 reti;				스택포인터를 참고하여 메인 프로그램 복귀

EXT_INT5:					// 외부 인터럽트 5번, 2번 스위치를 누를 경우 동작
	cli;				인터럽트 disable

	ldi temp, $00;
	out	PORTA, temp;		A포트 led를 모두 소등

	ldi temp, 200;
	call delay_Tms;			debouncing을 위한 지연함수

	sei;				인터럽트 on
	reti;				스택포인터를 참고하여 메인 프로그램 복귀
	
delay_1ms:				// 시간 지연 함수
	push YL;
	push YH;
	ldi YL, low(((FCLK/1000)-18)/4);
	ldi YH, high(((FCLK/1000)-18)/4);

	loop:
	sbiw YH:YL, 1;
	brne loop;

	pop YH;
	pop YL;
	ret;

delay_Tms:				// 시간 지연 함수
	call delay_1ms;
	dec temp;
	brne Delay_Tms;

	ret;

