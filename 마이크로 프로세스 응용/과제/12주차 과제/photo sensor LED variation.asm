.include "m128def.inc"
.def	temp		=	r16
.def	ADdataL		=	r17
.def	ADdataH		=	r18
.equ	F_CPU		=	16000000
.equ	F_SCK		=	100000
.equ	Lux10		=	871
.equ	Lux100		=	989
.equ	Lux1		=	93

.CSEG
	.ORG 0x0000;
	rjmp RESET;
	.ORG 0x0046;

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					스택 설정

	ldi temp, $ff;					
	out DDRA, temp;					led 모두 출력으로 사용

	rcall AD_Init;					photo sensor 설정

forever:
	rcall Read_ADC;					photo sensor로 부터 data를 읽어들임
	rcall Show_ADC;					photo sensor로 부터 읽어들인 data를 바탕으로 led를 on off함
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

Show_ADC:				// photo sensor data 를 바탕으로 led를 제어하는 함수
	cpi ADDataL, LOW(Lux1);			
	ldi temp, HIGH(Lux1);			
	cpc ADDataH, temp;				 1Lux의 조도값과 읽어들인 데이터값을 비교	
	brlt Led8;						만약 읽어들인 데이터값이 1Lux보다 작으면 (더 어두우면) led8개 점등
	
	cpi ADDataL, LOW(Lux10);
	ldi temp, HIGH(Lux10);
	cpc ADDataH, temp;				10Lux의 조도값과 읽어들인 데이터값을 비교
	brlt Led4;						만약 읽어들인 데이터값이 1Lux ~ 10Lux 사이의 밝기면 led 4개 점등

	cpi ADDataL, LOW(Lux100);
	ldi temp, HIGH(Lux100);
	cpc ADDataH, temp;				100Lux의 조도값과 읽어들인 데이터값을 비교
	brlt Led2;						만약 읽어들인 데이터값이 10Lux ~ 100Lux 사이의 밝기면 led 2개 점등
	
	rjmp Led0;						만약 읽어들인 데이터값이 100Lux 이상의 밝기면 led 모두 소등
	

Led8:									// led 8개 켜기			
	ser temp;			
	out PORTA, temp;					
	ret;

Led4:									// led 4개 켜기
	ldi temp, $0f;
	out PORTA, temp;
	ret;

Led2:									// led 2개 켜기
	ldi temp, $03;
	out PORTA, temp;
	ret;
Led0:									// led 모두 소등
	clr temp;
	out PORTA, temp;
	ret;
	



	
	


	