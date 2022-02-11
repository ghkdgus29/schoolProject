.include "m128def.inc"
.def	temp	=	r16
.def	leddata	=	r17
.def	segen	=	r18
.def	timecnt	=	r19
.def	SREG2	=	r20			// status 레지스터를 저장할 레지스터
.def	segcnt	=	r21
.def	cnt		=	r23
.equ	FCLK	=	16000000
.equ	LF		=	$0A
.equ	CR		=	$0D
.equ	EOS		=	$00

.CSEG

		.ORG 0;
		rjmp RESET;
		.ORG 0x000A;
		jmp EXT_INT4;		외부 인터럽트 4번, 1번 스위치를 누를 경우 동작
		jmp EXT_INT5;		외부 인터럽트 5번 , 2번 스위치를 누를 경우 동작
		.ORG 0x0020;
		rjmp TIM0_OVF;      타이머 인터럽트
		.ORG 0x0046;

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					스택포인터 설정

	ser temp;
	out DDRC, temp;			7segment 모두 사용하기 위한 설정

	ldi temp, $0F;
	sts DDRG, temp;			4개 숫자판을 모두 출력하기 위한 설정

	call Uart_Init;			UART 통신 설정 초기화
	cli;			인터럽트 disable
	rcall Timer0_Init;		타이머 인터럽트 초기화 설정
	rcall EXINT_Init;		외부 인터럽트 초기화 설정
	sei;			인터럽트 on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);	x 레지스터에 digit4 주소를 저장 (인덱싱)

	ldi temp, 5;
	st X+, temp;
	ldi temp, 9;
	st X+, temp;
	ldi temp, 5;
	st X+, temp;
	ldi temp, 0;
	st X, temp;				인덱싱할 숫자를 차례로 저장한다 (digit4에 저장)

	ldi segen, $08;
	ldi timecnt, 0;

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);

	clr r0;
	add XL, segcnt;
	adc XH, r0;				 segcnt는 인덱싱넘버 자리수를 의미하며 이를 x레지스터에 더하여 1초, 10초, 1분, 10분 인덱싱 정보에 접근한다.

	ldi ZH, HIGH(2*SegData);
	ldi ZL, LOW(2*SegData);			z레지스터에 7segment를 표시하기 위한 값들이 저장되어 있는 SegData주소를 저장

	ld temp, X;
	clr r0;
	add ZL, temp;
	adc ZH, r0;						z레지스터에 X에 저장되어 있는 인덱싱 넘버를 더함

	lpm leddata, Z;					이렇게 인덱싱 넘버까지 던한 값을 leddata에 저장
	sts PORTG, segen;				segen값에 해당하는 숫자판 점등
	out PORTC, leddata;				leddata값에 해당하는 7segment 값을 해당 숫자판에 출력

	rjmp forever;
		
Timer0_Init:				// 타이머 인터럽트 초기화 설정
	ldi temp, $06;
	out TCCR0, temp;		16MHZ/256으로 timer clock 스케일링

	ldi temp, $00;
	out TCNT0, temp;		인터럽트가 발생하고 나서 타이머를 다시 0으로 초기화

	ldi temp, 01;
	out TIMSK, temp;		오버플로우 발생시 인터럽트 발생
	ret;

EXINT_Init:					// 외부 인터럽트 초기화 설정
	ldi temp, $00;
	sts EICRA, temp;
	
	ldi temp, $0A;
	sts EICRB, temp;        외부 인터럽트 4번 5번을 falling edge 인터럽트로 사용

	ldi temp, $30;
	out EIMSK, temp;		외부 인터럽트 4번 5번을 사용가능하도록 설정
	ret;

Uart_Init:					// UART 통신 설정 초기화
	ldi temp, $00;
	sts UBRR0H, temp;		// U2X = 0

	ldi temp, $67;
	out UBRR0L, temp;		// UBRR = 103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;		// TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;		// 8bit data, 1 stop bit, No parity

	ret;

TIM0_OVF:					// 타이머 인터럽트 
	cli;			인터럽트 disable
	in SREG2, SREG;
	push SREG2;				status register 를 스택에 저장

	lsr segen;				segen 우측 쉬프팅, 숫자판 이동
	inc segcnt;				segcnt(숫자판 자리수) 증가
	cpi segen, $00;			
	brne Skip_resetsegen;		segen값이 0이 아니면 이동
	ldi segen, $08;			segen값이 0이 되면 다시 0x08로 초기화
	clr segcnt;				숫자판 자리수도 초기화

Skip_resetsegen:			//segen이 0보다 크면
	inc timecnt;			
	cpi timecnt, 244;		// 타이머 인터럽트가 244번 지나지 않으면 (1초가 지나지 않으면) 
	brne DONE;				//  DONE으로 이동 , (아무일이 일어나지 않는다.)

second:						// 1초가 지나면 (244번 인터럽트 발생) 4번째 숫자판을 수정
	clr timecnt;
	ldi YH, High(digit4);
	ldi YL, Low(digit4);
	ldd temp, Y+3;
	inc temp;				 초에 해당하는 인덱싱 넘버 증가
	cpi temp, 10;				
	breq ten_seconds;		자리수 증가가 일어나면 ten_seconds로 이동
	std Y+3, temp;			자리수 증가가 일어나지 않았다면 temp값을 인덱싱 넘버에 저장
	jmp DONE;

ten_seconds:				// 10초가 지나면 3번째 숫자판 수정
	clr temp;
	std Y+3, temp;			이전 자리수의 인덱싱 넘버는 0으로 초기화
	ldd temp, Y+2;			자리수를 증가시키고, 그에 해당하는 인덱싱 넘버값을 temp에 저장
	inc temp;
	cpi temp, 6;			만약 자리수 증가가 일어나면 minute으로 이동
	breq minute;
	std Y+2, temp;			자리수 증가가 일어나지 않았다면 temp값을 인덱싱 넘버에 저장
	jmp DONE;

minute:						//60초가 지나면 2번째 숫자판 수정
	clr temp;				minute, ten_minute 함수는 위의 second, ten_seconds함수와 동작이 동일하다.
	std Y+2, temp;
	ldd temp, Y+1;
	inc temp;
	cpi temp, 10;
	breq ten_minutes;
	std Y+1, temp;
	jmp DONE;

ten_minutes:				// 10분이 지나면 1번째 숫자판 수정
	clr temp;
	std Y+1, temp;
	ld temp, Y;
	inc temp;
	cpi temp, 6;
	breq hour;
	st Y, temp;
	jmp DONE;

hour:					// 60분이 지나면 모두 0이 된다
	clr temp;			
	st Y, temp;			첫번째 숫자판에 해당하는 인덱싱 넘버 초기화

DONE:					// 자리수 증가가 일어나지 않으면 수행
	pop SREG2;
	out SREG, SREG2;		status register 를 스택에서 꺼낸다.
	sei;			인터럽트 on
	ret;

EXT_INT4:				// 외부 인터럽트 4 , 1번 스위치를 누를 경우 수행
	cli;			인터럽트 disable

	ldi YH, High(digit4);
	ldi YL, Low(digit4);		y레지스터에 인덱싱 넘버 주소값 저장

	ldi temp, 200;
	call delay_Tms;				debouncing 을 고려한 지연함수

	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y, temp;					인덱싱 넘버값들을 모두 0으로 초기화 -> 타이머 0000초기화됨

	sei;			인터럽트 on
	reti;			스택포인터를 참고하여 메인프로그램 복귀

EXT_INT5:			// 외부 인터럽트 5, 2번 스위치를 누를 경우 수행
	cli;				인터럽트 disable
	in temp, SREG;		status register 저장
	push temp;
	push YH;
	push YL;
	push ZH;
	push ZL;			레지스터가 섞이는 걸 막기 위한 레지스터들 push


	ldi ZH, High(2*message);
	ldi ZL, Low(2*message);				message 배열의 주소 Z에 저장
	ldi YH, High(buffer);
	ldi YL, Low(buffer);				buffer 메모리 공간 주소 Y에 저장

	rcall NewLine;					줄 바꿈
	call PutStringP;				message 배열에 담겨있는 문자열 출력
	call GetString;					하이퍼 터미널로 부터 시간을 입력받아 buffer에 저장

	ldi cnt, 4;						카운트 4, 4번 루프를 돌기 위함이다

	ldi YH, HIGH(buffer);
	ldi YL, LOW(buffer);			buffer 메모리 공간 주소 Y 저장
	ldi ZH, HIGH(digit4);
	ldi ZL, LOW(digit4);			digit4 메모리 공간 주소 Z저장

again:
	ld temp, Y+;					temp에 buffer값 저장
	subi temp, $30;					temp에 들어있는 값은 아스키값이므로 '0'에 해당하는 아스키코드값을 빼준다
	st Z+, temp;					temp에 들어있는 값을 digit4에 저장
	dec cnt;						모든 digit4에 다음동작 반복 수행
	brne again;

	pop ZL;
	pop ZH;
	pop YL;
	pop YH;
	pop temp;
	out SREG, temp;					status register, push 한 register들을 모두 꺼낸다
	sei;					인터럽트 on
	reti;


PutChar:					// 문자 전달, 출력 함수
	sbis UCSR0A, UDRE0;
	rjmp PutChar;
	out UDR0, r0;
	ret;

GetChar:					// 문자를 받는, 입력 함수
	sbis UCSR0A, RXC0;
	rjmp GetChar;
	in r0, UDR0;
	ret;

PutStringP:					// 프로그램 메모리 공간에 저장되어 있는 값을 하이퍼 터미널로 출력한다.
	lpm r0, Z+;
	tst r0;
	breq exit1;
	rcall PutChar;
	rjmp PutStringP;
exit1:
	ret;

PutStringD:					// 메모리공간에 저장되어 있는 값을 하이퍼터미널로 출력한다
	ld r0, Z+;
	tst r0;
	breq exit2;
	rcall PutChar;
	rjmp PutStringD;
exit2:
	ret;

GetString:					//하이퍼 터미널로부터 문자열을 입력받아 Y레지스터에 해당하는 주소에 저장한다.
	rcall GetChar;
	rcall PutChar;
	st Y+, r0;
	ldi temp, CR;
	cp r0, temp;
	brne GetString;
	clr r0;
string:
	st Y, r0;
	ret;

NewLine:					// 줄바꿈
	ldi temp, LF;
	mov r0, temp;
	call PutChar;
	ldi temp, CR;
	mov r0, temp;
	call PutChar;
	ret;

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

SegData:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

message:
	.db "Write setting time in MMSS format:", EOS;

	.DSEG
digit4:
	.byte 4

buffer:
	.byte 5

