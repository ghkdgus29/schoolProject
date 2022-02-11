.include "m128def.inc"
	.ORG	0;
	rjmp	RESET;
	.ORG	$0020;
	rjmp	Timer0_OVF;				// 인터럽트 발생시 이동

.def	temp	=	r16				// 임시변수
.def	leddata	=	r17				// led data 저장
.def	intCnt	=	r18				// 카운트

	.ORG	$0046;
RESET:
	ldi	temp, low(RAMEND);			
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					// 스택포인터 설정

	cli;							// 인터럽트 disable
	rcall Timer0_Init;				// Timer 설정 함수 호출
	sei;							// 입터럽트 on

	ldi temp, $FF;
	out DDRA, temp;					// led를 8개 모두 사용하기 위한 설정
	ldi leddata, $FF;				// led data에 0xFF값을 집어넣는다.

forever:
	out PORTA, leddata;
	rjmp forever;					// led data값을 반복해서 출력한다.

Timer0_Init:						// 타이머 설정 함수
	ldi temp, $06;
	out TCCR0, temp;				// 16MHZ/256으로 Timer clock을 스케일링

	ldi temp, $00;
	out TCNT0, temp;				// 인터럽트가 발생하고나서 타이머를 다시 0으로 초기화하는 설정

	ldi temp, 01;
	out TIMSK, temp;				// 오버플로우 발생시 인터럽트가 발생하도록 설정
	ret;

Timer0_OVF:
	cli;							// 인터럽트 disable
	inc intCnt;					
	cpi intCnt, 122;				// intCnt가 244보다 작으면 아무일도 일어나지 않는다.
	brne Skip_ResetCnt;
	clr intCnt;						// intCnt를 0으로 초기화하고
	com leddata;					// leddata를 뒤집는다. (0으로만든다)

Skip_ResetCnt:						// intCnt가 244보다 작은 상황
	sei;							// 인터럽트 on
	reti;
