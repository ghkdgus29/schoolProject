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
	rcall Uart_Init;			// UART 통신 설정 초기화

forever:
	ldi YH, HIGH(rdata);
	ldi YL, LOW(rdata);			// rdata 메모리 공간의 주소를 Y에 저장
	rcall GetString;			// 하이퍼 터미널의 입력값을 AVR에 저장
	rcall NewLine;				// 줄바꿈

	ldi ZH, HIGH(rdata);
	ldi ZL, LOW(rdata);			// rdata 메모리 공간의 주소를 Z에 저장
	rcall PutStringD;			// AVR에 저장된 값들을 하이퍼 터미널에 출력
	rcall NewLine;				// 줄바꿈

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
	sbis UCSR0A, UDRE0;		// 만약 데이터를 입력받는 중이면 계속해서 PutChar 루프를 반복
	rjmp PutChar;
	out	UDR0, r0;			// 데이터를 모두 입력받으면 r0를 AVR에 저장
	ret;

GetChar:					// 문자 입력 함수
	sbis UCSR0A, RXC0;		// 만약 데이터를 전달받는 중이면 계속해서 GetChar 루프 반복
	rjmp GetChar;
	in r0, UDR0;			// 데이터를 모두 입력받으면 UDR0값을 (다른 기기에서의 입력값) AVR r0레지스터에 전달
	ret;

PutStringD:
	ld	r0, Z+;				// Z주소를 증가시켜가며 데이터를 r0에 저장
	tst r0;					// r0가 EOS 와 같다면 
	breq exit;				// 데이터 입력 종료
	rcall PutChar;			// 다르다면 데이터 입력(전달)
	rjmp PutStringD;		// 반복

exit:
	ret;

GetString:
	rcall GetChar;
	rcall PutChar;			// EHCOing
	st Y+, r0;				// Y주소에 해당하는 메모리공간에 하이퍼터미널로 부터 입력받은 값들 저장
	ldi temp, CR;			// temp에 엔터키에 해당하는 아스키값저장	
	cp r0, temp;			// 엔터키를 누르면 문자열 입력받기가 끝난다
	brne GetString;			// 만약 엔터키를 누르지 않았다면 계속해서 문자열을 입력받는다
	clr r0;					// r0에 EOS값을 저장

string:
	st Y, r0;				// 문자열 마지막에 EOS저장
	ret;

NewLine:
	ldi temp, LF;
	mov r0, temp;
	call PutChar;			// 줄바꿈을 avr을 통해 하이퍼터미널에 출력
	ldi temp, CR;
	mov r0, temp;
	call PutChar;			// 커서를 처음으로 옮김 (avr을 통해 하이퍼터미널에 출력)
	ret;

.DSEG

rdata:
	.byte	64				// 데이터 저장 공간 (문자열)