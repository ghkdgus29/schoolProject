.include "m128def.inc"
.def	temp		=	r16
.def	ADdataL		=	r17
.def	ADdataH		=	r18
.equ	F_CPU		=	16000000
.equ	F_SCK		=	100000
.equ	CDS_Value	=	871

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


Show_ADC:				// photo sensor data 를 바탕으로 led를 제어하는 함수
	cpi ADDataL, LOW(CDS_Value);			
	ldi temp, HIGH(CDS_Value);
	cpc ADDataH, temp;					ADC 데이터와 CDS_Value(설정한 조도값)을 비교
	brlt Led_on;						만약 현재 조도값이 설정한 조도값보다 작으면 (어두우면) led를 켠다

Led_off:								// 만약 현재 조도값이 설정한 조도값보다 크면 (밝으면) led를 끈다
	clr temp;			
	out PORTA, temp;					
	ret;

Led_on:									// led를 켠다
	ser temp;
	out PORTA, temp;
	ret;



	
	


	