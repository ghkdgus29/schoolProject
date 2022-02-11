.include "m128def.inc"
.def	temp	=	r16
.equ	LF		=	$0A
.equ	CR		=	$0D
.equ	EOS		=	$00
.equ	TAP		=	$09

.CSEG
	.ORG 0x0046;

RESET:
	ldi	temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;				 스택설정

	rcall Initialize;			UART 통신 설정
	rcall PrintTitle;			제목 프린트

forever:
	rjmp forever;

Initialize:							// UART 통신 설정 초기화
	ldi temp, $00;		
	sts UBRR0H, temp;			U2X = 0	

	ldi temp, $67;
	out UBRR0L, temp;			UBRR=103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;			TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;			8bit data, 1 stop bit, No parity

	ret;
	
PrintTitle:						// 메뉴 인쇄 함수
	rcall NewLine;					한줄 띄어쓰기
	ldi ZH, high(2*Title);
	ldi ZL, low(2*Title);			Title 문자열이 존재하는 배열의 주소를 Z에 저장
	rcall PutStringP;				문자열 출력함수 호출
	ret;

PutStringP:
	lpm r0, Z+;					Z주소를 증가시켜가며 데이터를 r0에 저장
	tst r0;						r0값이 EOS라면
	breq exit;					데이터 입력 종료
	rcall PutChar;				다르다면 데이터 계속해서 입력(전달)
	rjmp PutStringP;			반복

exit:
	ret;

PutChar:						// 데이터를 avr을 통해 하이퍼 터미널에 출력하는 함수
	sbis UCSR0A, UDRE0;			데이터 전달 중이면 putchar루프 반복
	rjmp PutChar;				
	out UDR0, r0;				전달이 모두 이뤄지면 r0값(AVR레지스터값)을 하이퍼 터미널로 전달
	ret;

NewLine:
	ldi temp, LF;
	mov r0, temp;				r0 에 줄바꿈값 저장
	call PutChar;				줄바꿈을 avr을 통해 하이퍼 터미널에 출력
	ldi temp, CR;				
	mov r0, temp;				r0에 커서 처음으로 옮기는 동작을 저장
	call PutChar;				이를 하이퍼 터미널을 통해 출력
	ret;

Title:																	// 타이틀 문자열, constant임
	.db TAP,"+---------------------------------------+",CR,LF;
	.db TAP,"|This is an AVR Test Board              |",CR,LF; 
	.db TAP,"|By Hwang Hyun , 201711282              |",CR,LF;
	.db TAP,"|Following functions are provided       |",CR,LF;
	.db TAP,"|1) LED Display                         |",CR,LF;
	.db TAP,"|2) 7 Segment Display                   |",CR,LF;
	.db TAP,"|3) Buzzer Test                         |",CR,LF;
	.db TAP,"|4) Photo Sensor Test                   |",CR,LF;
	.db TAP,"|5) Temperature Sensor test             |",CR,LF;
	.db TAP,"+---------------------------------------+",CR,LF,EOS,EOS ;		// padding zero bytes 에 의해 원하는 동작이 걸리지 않을 수 있다. 주의할것