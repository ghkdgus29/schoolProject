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

forever:
	rcall GetChar;			// GetChar 함수를 통해 하이퍼터미널에서의 입력값을 AVR r0레지스터에 저장
	rcall PutChar;			// PutChar 함수를 통해 AVR r0레지스터의 저장된 값을 하이퍼터미널에 출력
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

PutChar:
	sbis UCSR0A, UDRE0;		// 만약 데이터를 전달하는 중이면 계속해서 PutChar 루프를 반복
	rjmp PutChar;
	out	UDR0, r0;			//데이터를 모두 전달하면 r0 (AVR)값을 하이퍼터미널로 전달
	ret;

GetChar:					// 문자 입력 함수
	sbis UCSR0A, RXC0;		// 만약 데이터를 전달받는 중이면 계속해서 GetChar 루프 반복
	rjmp GetChar;
	in r0, UDR0;			// 데이터를 모두 입력받으면 UDR0값을 (다른 기기에서의 입력값) AVR r0레지스터에 전달
	ret;
