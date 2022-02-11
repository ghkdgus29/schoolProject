.include "m128def.inc"
.def	temp	=	r16
.def	leddata	=	r17
.equ	LF		=	$0A				// 줄바꿈
.equ	CR		=	$0D				// 커서 초기화
.equ	EOS		=	$00				// 문자열 끝
.equ	TAP		=	$09				// 탭

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
	rcall PrintCmdCursor;		커서 프린트
	rcall ReadCmd;				커서값 읽기

	rcall CmdInterprete;		커서값 해석하여 해당하는 동작하기
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

cmdcursor:
	.db TAP,"Input Function Number:  ",EOS;						// 커서 문자열, constant

PrintCmdCursor:												// 커서를 출력하는 함수
	CALL NewLine;					 한줄 띄어쓰기
	ldi ZH, high(2*cmdcursor);
	ldi ZL, low(2*cmdcursor);		Z에 커서문자열 주소 저장
	call PutStringP;				문자열 출력하기
	ret;

.DSEG

buffer:	
	.byte 10

.CSEG
ReadCmd:
	ldi YH, high(buffer);
	ldi YL, low(buffer);
	rcall GetString;
	ret;

GetChar:
	sbis UCSR0A, RXC0;			만약 데이터를 전달받는 중이면 계속해서 Getchar 루프 반복
	rjmp GetChar;
	in r0, UDR0;				데이터를 모두 입력받으면 UDR0값을 AVR r0 레지스터에 전달
	ret;

GetString:						//문자열 받는 함수
	rcall GetChar;		
	call PutChar;				Echoing
	st Y+, r0;					Y주소에 해당하는 메모리 공간에 하이퍼 터미널로부터 입력받은 값들 저장
	ldi temp, CR;				temp에 엔터키에 해당하는 아스키값 저장
	cp r0, temp;				엔터키를 누르면 문자열 입력받기가 끝난다.
	brne GetString;				만약 엔터키가 눌리지 않았다면 계속해서 문자열을 입력받는다.
	clr r0;						r0에 EOS값 저장
string:
	st Y, r0;					문자열 마지막에 EOS 저장
	ret;

CmdInterprete:					// 커서값 해석 함수
	ldi XH, high(buffer);
	ldi XL, low(buffer);		버퍼주소를 X레지스터에 저장
	ld temp, X;					버퍼에서 불러들인 값 temp에 저장

	cpi temp, '1';				만약 버퍼에 있던 값이 1이면
	breq CMD_LED;				LED test

	cpi temp, '2';				버퍼에 있던 값이 2이면
	breq CMD_Segment;			7 segment test
	
	cpi temp, '3';				버퍼에 있던 값이 3이면
	breq CMD_Buzzer;			buzzer test

	cpi temp, '4';				버퍼에 있던 값이 4이면
	breq CMD_PhotoSensor;		Photo sensor test

	cpi temp, '5';				버퍼에 있던 값이 5이면
	breq CMD_TempSensor;		Temp sensor test

	rjmp CMD_Wrong;				버퍼에 있던 값이 1, 2, 3, 4, 5 가 아니면 Wrong command 출력

CMD_Done:
	ret;

CMD_LED:						// LED Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*led);
	ldi ZL, low(2*led);
	call PutStringP;
	rjmp CMD_Done;

CMD_Segment:					// 7segment Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*seg);
	ldi ZL, low(2*seg);
	call PutStringP;
	rjmp CMD_Done;

CMD_Buzzer:						// Buzzer Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*buz);
	ldi ZL, low(2*buz);
	call PutStringP;
	rjmp CMD_Done;

CMD_PhotoSensor:				// PhotoSensor Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*pho);
	ldi ZL, low(2*pho);
	call PutStringP;
	rjmp CMD_Done;

CMD_TempSensor:					// TempSensor Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*tempS);
	ldi ZL, low(2*tempS);
	call PutStringP;
	rjmp CMD_Done;

CMD_Wrong:						// wrong comman가 입력됨을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*wrong);
	ldi ZL, low(2*wrong);
	call PutStringP;
	rjmp CMD_Done;		

led:
	.db TAP,"---> Here is LED Test Routine ",LF,CR,EOS;

seg:
	.db TAP,"---> Here is 7segment Test Routine",LF,CR,EOS;

buz:
	.db TAP,"---> Here is Buzzer Test Routine",LF,CR,EOS;

tempS:
	.db TAP,"---> Here is TempSensor Test Routine",LF,CR,EOS;

pho:
	.db TAP,"---> Here is PhotoSensor Test Routine ",LF,CR,EOS;

wrong:
	.db TAP,"---> Wrong command... ",LF,CR,EOS;
	