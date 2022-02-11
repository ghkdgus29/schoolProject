.include "m128def.inc"
.def temp	=	r20
.equ	LF	=	$0A				// 줄바꿈
.equ	CR	=	$0D				// 커서 처음으로 옮기기
.equ	EOS	=	$00				// 문자열 마지막 표시

.CSEG
		.ORG 0x0000;
		JMP RESET;

		.ORG 0x0046;
RESET:
	ldi r16, low(RAMEND);
	out	SPL, r16;
	ldi	r16, high(RAMEND);
	out SPH, r16;
	rcall Uart_Init;			// 직렬통신 설정 초기화

	ldi ZH, high(2*message);
	ldi ZL, low(2*message);		// Z레지스터에 message (string) 주소 할당
	rcall PutStringP;			// 문자열 출력 함수 

forever:
	rjmp	forever;

message:
	.db		"This is a AVR Test Board",CR, LF, EOS;		// 프로그램 메모리에 저장되어 있는 문자열

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
	out	UDR0, r0;			// 데이터를 모두 전달하면 r0 (AVR)값을 하이퍼터미널로 전달
	ret;

PutStringP:
	lpm	r0, Z+;				// Z주소를 증가시켜가며 데이터를 r0에 저장
	tst r0;					// r0가 EOS 와 같다면 
	breq exit;				// 데이터 입력 종료
	rcall PutChar;			// 다르다면 데이터 입력(전달)
	rjmp PutStringP;		// 반복

exit:
	ret;