.include "m128def.inc"
	.ORG	0;
	rjmp	RESET;
	.ORG	$0020;
	rjmp	Timer0_OVF;				// 인터럽트 발생시 이동

.def	temp	=	r16				// 임시변수
.def	leddata	=	r17				// led data 저장
.def	timecnt	=	r18				// 카운트
.def	segen	=	r19
.def	segcnt	=	r20

	.ORG	$0046;
RESET:
	ldi	temp, low(RAMEND);			
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					// 스택포인터 설정

	ser temp;
	out DDRC, temp;					// 8개의 segment를 모두 사용하기 위한 설정

	ldi temp, $0F;
	sts DDRG, temp;					// 4개의 숫자판을 모두 사용하기 위한 설정

	cli;							// 인터럽트 disable
	rcall Timer0_Init;				// Timer 설정 함수 호출
	sei;							// 입터럽트 on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);			// X레지스터에 digit4 주소를 저장

	ldi temp, 5;
	st X+, temp;
	ldi temp, 9;
	st X+, temp;
	ldi temp, 5;
	st X+, temp;
	ldi temp, 0;
	st X, temp;						// led data에 접근하기 위한 index를 digit4에 저장한다.

	ldi segen, $08;					// 첫번째 숫자판 점등
	ldi timecnt, 0;					// timecnt 변수 0으로 초기화

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);

	clr r0;	
	add XL, segcnt;					// segcnt(인덱싱 넘버 자리수)값을 X레지스터에 더한다. 즉 초, 10초, 1분, 10분을 의미하는 숫자판을 접근하기 위함이다.
	adc XH, r0;

	ldi ZH, HIGH(2*SegData);
	ldi	ZL, LOW(2*SegData);			// Z레지스터에 led data 값이 저장되어있는 SegData의 주소 저장

	ld temp, X;						// temp에 X레지스터 주소에 있는 인덱싱 넘버 저장 (digit4)
	clr r0;
	add ZL, temp;					
	adc	ZH, r0;						// Z레지스터 주소에 digit4에 있는 인덱싱 넘버를 더한다.

	lpm leddata, Z;					// 이렇게 인덱싱 넘버를 더한 값을 leddata 레지스터에 저장
	sts PORTG, segen;				// segen 값에 해당하는 숫자판 점등
	out	PORTC, leddata;				// leddata값에 해당하는 값을 출력한다. 

	rjmp forever;					// 반복

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
	in r1, SREG;
	push r1;						// status register 를 스택에 저장
					
	lsr segen;						// segen 을 우측으로 쉬프팅한다. 점등하려는 숫자판 이동
	inc segcnt;						// segcnt값을 증가시킨다. 
	cpi segen, $00;					
	brne Skip_ResetSegEn;			// segen 값이 0이 아니면 이동한다.
	ldi segen, $08;					// segen 값이 0이 되면 다시 0x08로 초기화 해준다.
	clr segcnt;						// segcnt값을 초기화시킨다.
					
Skip_ResetSegEn:					// segen이 0보다 큰 상황
	inc timecnt;					
	cpi timecnt, 244;				// timecnt 인터럽트가 발생할 때 마다 증가시킨다. 만약 1초(=244번 인터럽트 발생)가 지나기 전엔
	brne DONE;						// DONE으로 이동하여 아무런 동작도 수행하지 않는다.

second:								// 4번째 전광판 값을 수정한다. (초)
	clr timecnt;					// 만약 1초가 지나면 timecnt를 초기화한다
	ldi YH, High(digit4);			
	ldi YL, Low(digit4);			// Y레지스터에 digit4주소를 저장
	ldd temp, Y+3;					// digit4의 4번째 인덱싱 넘버, 즉 4번째 숫자판(초)에 데이터를 temp에 저장한다.
	inc temp;						// temp를 증가시킨다.
	cpi temp, 10;					// temp가 10, 즉 9초가 지나게 되면 
	breq ten_seconds;				// 10초로 이동한다.
	std Y+3, temp;					// temp가 9초를 지나지 않았다면 temp값을 digit4의 4번째에 저장한다.
	jmp DONE;						

ten_seconds:						// 3번째 전광판 값을 수정한다. (10초)
	clr temp;						// temp값을 0으로 초기화
	std Y+3, temp;					// (초)자리에서 9초가 지나면 다시 0으로 바꾼다. 
	ldd temp, Y+2;					// digit4의 3번째 인덱싱 넘버, 즉 3번째 숫자판(10초)에 데이터를 temp에 저장한다.
	inc temp;						// temp를 증가시킨다.
	cpi temp, 6;					// temp가 6, 즉 60초를 지나게 되면 
	breq minute;					// 1분으로 이동한다.
	std Y+2, temp;					// temp가 60초를 지나지 않았다면 temp값을 digit4의 3번째에 저장한다.
	jmp DONE;

minute:								// 2번째 전광판 값을 수정한다. (1분)
	clr temp;						// temp값을 0으로 초기화
	std Y+2, temp;					// (10초)자리에서 60초가 지나면 다시 0으로 바꾼다.
	ldd temp, Y+1;					// digit4의 2번째 인덱싱 넘버, 즉 2번째 숫자판(1분)에 데이터를 temp에 저장한다.
	inc temp;						// temp를 증가시킨다.
	cpi temp, 10;					// temp가 10, 즉 9분을 지나게 되면 
	breq ten_minutes;				// 10분으로 이동한다.
	std Y+1, temp;					// temp가 9분을 지나지 않았다면 temp값을 digit4의 2번째에 저장한다.
	jmp DONE;

ten_minutes:						// 1번째 전광판 값을 수정한다. (10분)
	clr temp;						// temp값을 0으로 초기화
	std Y+1, temp;					// (1분)자리에서 9분이 지나면 다시 0으로 바꾼다.
	ld temp, Y;						// digit4의 1번째 인덱싱 넘버, 즉 1번째 숫자판(10분)에 데이터를 temp에 저장한다.
	inc temp;						// temp를 증가시킨다.
	cpi temp, 6;					// temp가 6, 즉 60분을 지나게 되면 
	breq  hour;						// 1시간으로 이동한다.
	st Y, temp;						// temp가 60분을 지나지 않았다면 temp값을 digit4의 1번째에 저장한다.
	jmp DONE;

hour:
	clr temp;						// temp값을 0으로 초기화
	st Y, temp;						// (10분)자리에서 60분이 지나면 0으로 바꾼다.

DONE:
	pop r1;
	out SREG, r1;						// status register 를 스택에서 꺼낸다.
	sei;								// 인터럽트 on
	ret;
	

SegData:								// LED data 값이 저장되어 있는 프로그램  메모리 배열이다.
	.db	$3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

	.DSEG
digit4:									// 인덱싱할 숫자들이 들어있는 데이터 메모리 공간이다.
	.byte 4
		
