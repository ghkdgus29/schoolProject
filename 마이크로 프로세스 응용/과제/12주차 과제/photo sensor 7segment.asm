.include "m128def.inc"
.def	temp		=	r16
.def	ADdataL		=	r17
.def	ADdataH		=	r18
.def	SREG2		=	r20
.def	segen		=	r21
.def	segcnt		=	r22
.def	timecnt		=	r23
.def	leddata		=	r24
.equ	F_CPU		=	16000000
.equ	F_SCK		=	100000


.CSEG
	.ORG 0x0000;
	rjmp RESET;
	.ORG 0x0020;
	jmp	TIM0_OVF;
	.ORG 0x0046;

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					스택 설정

	ldi temp, $ff;					
	out DDRC, temp;					7segment 모두 출력으로 사용

	ldi temp, $0f;
	sts DDRG, temp;					4개 숫자판 모두 출력으로 사용
	
	cli;				인터럽트 off
	rcall AD_Init;					photo sensor 설정
	call Timer_Init;
	sei;				인터럽트 on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);			x 레지스터에 digit4 주소를 저장 (인덱싱)

	ldi segen, $08;			점등할 숫자판 자리수 변경을 위한 변수
	ldi timecnt, 0;			타이머 인터럽트가 발생한 횟수를 저장하기 위한 변수

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);			인덱스 배열주소를 X에 저장

	clr r0;
	add XL, segcnt;
	adc XH, r0;						숫자판 자리수에 해당하는만큼 더한 인덱스 배열의 주소를 X에 저장

	ldi ZH, High(2*SegData);
	ldi ZL, Low(2*SegData);			led data (7segment) 를 저장한 배열주소를 Z에 저장

	ld temp, X;					
	clr r0;
	add ZL, temp;
	adc ZH, r0;						인덱스 만큼 Z주소에 더한다

	lpm leddata, Z;					Z주소에 해당하는 led data를 레지스터 leddata에 저장		
	sts PORTG, segen;				segen에 저장되어 있는 자리수의 숫자판을 사용
	out PORTC, leddata;				leddata에 저장되어있는 led data를 출력
	
	rjmp forever;		

AD_Init:					// Photo sensor를 사용하기 위한 초기화
	clr temp;
	out ADMUX, temp;	AREF모드, photo sensor 사용, ADLAR off

	ldi temp, $87;
	out ADCSRA, temp;	enable ADC 모드, single conversion mode, 16M/128 Hz 로 스케일링
	
Read_ADC:					// photo sensor data를 읽어들이는 함수
	in temp, ADCSRA;			ADCSRA 레지스터값을 temp 에 저장
	ori temp, $40;				temp에 $40 을 더한다 (ADSC on)
	out ADCSRA, temp;			ADSCRA 레지스터에 ADSC비트를 set하여 ADC 동작을 수행

	in ADdataL, ADCL;			
	in ADdataH, ADCH;			읽어들인 ADC 데이터를 ADdataL, ADdataH 레지스터에 저장

	ret;


Show_ADC:				// photo sensor data 를 바탕으로 7segment를 control 하는 함수

	ldi YH, high(digit4);
	ldi YL, low(digit4);		인덱스 배열의 주소를 Y에 저장

	clr temp;
	st Y+, temp;				0을 가장 상위의 인덱스 배열에 저장

	andi ADdataH, $0f;			ADdataH의 하위 4개의 비트를 and masking
	st Y+, ADdataH;				하위 4개의 비트를 그 다음 인덱스 배열에 저장

	mov temp, ADdataL;			temp에 ADdataL 저장
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;					lsr 4번을 통해 상위 4개의 비트를 하위 4비트로 옮김
	st Y+, temp;				해당하는 데이터를 그 다음 인덱스 배열에 저장

	mov temp, ADdataL;			temp에 ADdataL 저장
	andi temp, $0f;				하위 4개의 비트를 and masking
	st Y, temp;					해당하는 데이터를 마지막 인덱스 배열에 저장

	ret;
		
Timer_Init:					// 타이머 인터럽트 초기화 설정
	ldi temp, $06;
	out TCCR0, temp;		16MHZ/256으로 timer clock 스케일링

	ldi temp, $00;
	out TCNT0, temp;		인터럽트가 발생하고 나서 타이머를 다시 0으로 초기화

	ldi temp, 01;
	out TIMSK, temp;		오버플로우 발생시 인터럽트 발생
	ret;

TIM0_OVF:				// 타이머 인터럽트
	cli;					인터럽트 off
	in SREG2, SREG;				status register 저장
	push SREG2;

	lsr segen;					숫자판 자리수 한칸 변경
	inc segcnt;					인덱스 배열에 접근할 주소에 더할 segcnt 증가
	cpi segen, $00;				만약 숫자판 자리수가 
	brne Skip_resetsegen;		0이 아니면 skip
	ldi segen, $08;				0이면 다시 0000 1000 으로 초기화
	clr segcnt;					segcnt도 0으로 초기화

Skip_resetsegen:
	inc timecnt;
	cpi timecnt, 244;			타이머 인터럽트가 244번 발생하면 1초에 해당한다
	brne DONE;					1초가 지나지 않으면 아무일도 일어나지 않는다.

second:							// 1초가 지나면
	call Read_ADC;					photo sensor로 부터 data 읽기
	call Show_ADC;					photo sensor로 부터 읽은 data를 7segment로 표시

DONE:
	pop SREG2;
	out SREG, SREG2;			status register 불러오기
	sei;					인터럽트 on
	ret;

SegData:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71			// 0~F에 해당하는 led data

	.DSEG
digit4:					// 인덱스 배열
	.byte 4


	
	


	