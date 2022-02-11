.include "m128def.inc"
.def temp	=	r20;

		.ORG 0x0000;
		JMP RESET;

		.ORG 0x0046;
RESET:
	ldi r16, low(RAMEND);
	out	SPL, r16;
	ldi	r16, high(RAMEND);
	out SPH, r16;
	rcall Uart_Init;

	ldi r16, 'A';
	mov r0, r16;
	rcall PutChar;

forever:
	rjmp forever;

Uart_Init:
	ldi temp, $00;
	sts	UBRR0H, temp;		// U2X=0

	ldi temp, $67;
	out UBRR0L, temp;		// UBRR=103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;		// TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;		//	8bit data, 1 stop bit, No partiy

	ret;

PutChar:					// 문자 출력 함수
	sbis UCSR0A, UDRE0;		// 만약 데이터를 전달하는 중이면 계속해서 PutChar 루프를 반복
	rjmp PutChar;
	out	UDR0, r0;			// 데이터를 모두 입력하면 r0를 (AVR에 저장된 값) 다른 기기에 전달
	ret;

