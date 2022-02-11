.include "m128def.inc"
.def	count	=	r13
.def	dataL	=	r14
.def	dataH	=	r15
.def	temp	=	r16
.def	leddata	=	r17
.def	segen	=	r18
.def	segcnt	=	r19
.def	I2Cdata	=	r20
.def	I2Cack	=	r21
.def	SREG2	=	r22
.def	timecnt	=	r23
.def	escape	=	r24
.def	test	=	r25
.equ	LF		=	$0A				// 줄바꿈
.equ	CR		=	$0D				// 커서 초기화
.equ	EOS		=	$00				// 문자열 끝
.equ	TAP		=	$09				// 탭
.equ	ATS75_CONFIG_REG	=	1
.equ	ATS75_TEMP_REG		=	0

.CSEG
	.ORG 0x0000;
	rjmp RESET;
	.ORG 0x0020;
	jmp TIM0_OVF;
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
	call LED_Test;
	rjmp CMD_Done;

CMD_Segment:					// 7segment Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*seg);
	ldi ZL, low(2*seg);
	call PutStringP;
	call Seg_Test;
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
	call Photo_Test;
	rjmp CMD_Done;

CMD_TempSensor:					// TempSensor Test 중임을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*tempS);
	ldi ZL, low(2*tempS);
	call PutStringP;
	call Temp_Test;
	rjmp CMD_Done;

CMD_Wrong:						// wrong comman가 입력됨을 출력하는 루프
	call NewLine;
	ldi ZH, high(2*wrong);
	ldi ZL, low(2*wrong);
	call PutStringP;
	rjmp CMD_Done;		

LED_Test:					// led test 함수
	ldi temp, $FF;
	out DDRA, temp;				led 8개 모두 출력으로 사용
	ldi leddata, $80;
	
	repeat1:
	out PORTA, leddata;			8번 led 점등
	rcall delay1sec;			1초 지연
	lsr leddata;				오른쪽으로 쉬프팅		
	brne repeat1;				만약 0이 되면 led test 종료, 아니면 반복
	
	out PORTA, leddata;
	ret;	
	
Seg_Test:					// 7 segment test 함수
	push segen;
	push segcnt;

	ldi temp, $FF;
	out DDRC, temp;			7segment 모두 출력으로 사용

	ldi temp, $0F;
	sts DDRG, temp;			4개의 숫자판 모두 출력으로 사용

	ldi segen, $08;			

	loop2:
	sts PORTG, segen;		segen에 해당하는 숫자판 출력으로 사용

	ldi segcnt, 10;			0~9까지 10개의 숫자를 나타내기 위한 카운트

	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);		인덱스 주소를 Z에 저장

	repeat2:
	lpm r0, Z+;					Z값(7seg 숫자값) 증가시키며 r0에 저장				
	out PORTC, r0;				해당하는 숫자값을 출력 
	call delay1sec;				시간 지연
	subi segcnt, 1;				cnt 감소 (0~9 까지 표현하기 위함)
	brne repeat2;				만약 cnt가 남아있으면 반복

	lsr segen;					0~9까지 모두 표현했으면 segen을 변경 (숫자판 변경)
	breq end;					
	rjmp loop2;					마지막 숫자판까지 모두 출력하지 않았다면 loop2로 가서 반복

	end:						// 마지막 숫자까지 모두 표현했으면 함수를 빠져나간다.
	clr r0;
	out PORTC, r0;				마지막 led 숫자를 없앤다
	pop segcnt;
	pop segen;
	ret;	
	
Photo_Test:						// photo sensor test 함수
	push segen;
	push segcnt;
	push timecnt;

	ldi temp, $ff;
	out DDRC, temp;				7segment 출력으로 사용

	ldi temp, $0f;
	sts DDRG, temp;				4개 숫자판 모두 출력으로 사용

	cli;
	call AD_Init;
	call Timer_Init;			초기화
	sei;

	ldi XH, high(digit4);
	ldi XL, low(digit4);		인덱스 배열주소 X에 저장

	ldi segen, $08;
	ldi timecnt, 0;
	ldi escape, 10;				10초가 지나면 함수를 빠져나가도록 설정한 변수
	ldi test, 1;				test 변수가 1이면 타이머 인터럽트에서 photo sensor를 동작시킨다
	
	repeat3:
	ldi XH, high(digit4);
	ldi XL, low(digit4);

	clr r0;
	add XL, segcnt;
	adc XH, r0;

	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);

	ld temp, X;
	clr r0;
	add ZL, temp;
	adc ZH, r0;

	lpm leddata, Z;
	sts PORTG, segen;
	out PORTC, leddata;

	cpi escape, 1;				
	brge repeat3;				만약 escape 변수가 1보다 작아지면 (10초가 지나면) 함수를 빠져나간다.
	
	clr test;					test 변수를 0으로 만들어줘 타이머 인터럽트 내에서 test동작을 멈추도록 한다
	out PORTC, test;			현재 나오고 있는 7 segment값을 없앤다
	pop timecnt;
	pop segcnt;
	pop segen;
	ret;

		

delay1sec:						// 1초 지연함수
	push r20;
	push r21;
	push r22;
	

	ldi r20, 80;
	ldi r21, 255;
	ldi r22, 255;
loop:
	dec r22;
	brne loop;
	dec r21;
	brne loop;
	dec r20;
	brne loop;

	pop r22;
	pop r21;
	pop r20;
	ret;

AD_Init:					// photo sensor 초기화
	clr temp;
	out ADMUX, temp;

	ldi temp, $87;
	out ADCSRA, temp;

Read_ADC:					// photo sensor로 부터 조도값을 읽어오는 함수
	in temp, ADCSRA;
	ori temp, $40;
	out ADCSRA, temp;

	in dataL, ADCL;
	in dataH, ADCH;

Show_ADC:					// photo sensor로부터 읽어온 조도값을 digit4에 저장하는 함수
	ldi YH, high(digit4);
	ldi YL, low(digit4);

	clr temp;
	st Y+, temp;

	ldi temp, $0f;
	and dataH, temp;
	st Y+, dataH;

	mov temp, dataL;
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;
	st Y+, temp;

	mov temp, dataL;
	andi temp, $0f;
	st Y, temp;

Timer_Init:				// 타이머 인터럽트 초기화 함수
	ldi temp, $06;
	out TCCR0, temp;

	ldi temp, $00;
	out TCNT0, temp;

	ldi temp, 01;
	out TIMSK, temp;
	ret;

TIM0_OVF:			// 타이머 인터럽트
	cli;
	in SREG2, SREG;
	push SREG2;

	cpi test, 0;		 만약 test 변수가 0이면 바로 타이머 인터럽트 종료
	breq DONE;

	lsr segen;
	inc segcnt;
	cpi segen, $00;
	brne Skip_resetsegen;
	ldi segen, $08;
	clr segcnt;
	
	Skip_resetsegen:
	inc timecnt;
	cpi timecnt, 244;
	brne DONE;

	second:
	cpi test, 1;			만약 test변수가 1이면 photo sensor test이므로 ADCtest로 이동		
	breq ADCtest;

	cpi test, 2;			만약 test변수가 2이면 Temp test이므로 Temptest로 이동
	breq Temptest;

	rjmp DONE;

	ADCtest:
	dec escape;				1초마다 escape 변수를 감소시킨다
	call Read_ADC;
	call Show_ADC;
	rjmp DONE;

	Temptest:
	dec escape;				1초마다 escape 변수를 감소시킨다
	call TempRead;
	call ModifyFND;
	rjmp DONE;

	DONE:
	pop SREG2;
	out SREG, SREG2;
	sei;
	ret;

Temp_Test:				// Temperature 함수
	push segen;
	push segcnt;
	push timecnt;
	
	ldi temp, $ff;
	out DDRC, temp;

	ldi temp, $0f;
	sts DDRG, temp;

	cli;
	rcall TempInit;
	call Timer_Init;
	sei;

	ldi XH, high(digit4);
	ldi XL, low(digit4);

	ldi segen, $08;
	ldi timecnt, 0;
	ldi escape, 10;				10초간 동작하기 위한 escape 변수 10
	ldi test, 2;				test변수에 2를 넣어 타이머 인터럽트에서 Temptest를 수행하도록함	

	repeat4:
	ldi XH, high(digit4);
	ldi XL, low(digit4);

	clr r0;
	add XL, segcnt;
	adc XH, r0;

	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);

	ld temp, X;
	clr r0;
	add ZL, temp;
	adc ZH, r0;

	lpm leddata, Z;
	sts PORTG, segen;
	cpi segen, 2;
	breq Dot;
	out PORTC, leddata;
	cpi escape, 1;				만약 10초가 지나면 함수를 빠져나간다
	brge repeat4;

	rjmp return;


	Dot:
	ori leddata, $80;
	out PORTC, leddata;
	cpi escape, 1;				만약 10초가 지나면 함수를 빠져나간다		
	brge repeat4;

	return:
	clr test;
	out PORTC, test;
	pop timecnt;
	pop segcnt;
	pop segen;
	ret;

TempInit:				// 열 센서 관련 초기화
	rcall TWI_Init;		I2C 설정 초기화
	
	rcall TWI_Start;	I2C 시작	

	ldi I2Cdata, $98;	열 센서 관련 칩 주소 저장, 쓰기 모드(0)
	rcall TWI_Write;	해당 칩 주소로 접근			(칩을 불러오는 단계)

	ldi I2Cdata, ATS75_CONFIG_REG;		
	rcall TWI_Write;			칩 안에 Configuration register의 주소 저장	(칩 안의 레지스터에 접근하는 단계)

	ldi I2Cdata, 0;		I2Cdata에 0저장 
	rcall TWI_Write;	configuration register에 0을 저장		(칩 안 레지스터에 데이터 0 집어넣기)

	rcall TWI_Stop;		I2C 종료

	ret;

TempRead:				// 열 센서로부터 데이터를 읽는 함수
	rcall TWI_Start;		I2C 시작

	ldi I2Cdata, $98;	열 센서 관련 칩 주소 저장, 쓰기 모드(0)
	rcall TWI_Write;	해당 칩 주소로 접근			(칩 불러오는 단계)

	ldi I2Cdata, ATS75_TEMP_REG;		I2Cdata는 매개변수
	rcall TWI_Write;					칩 안의 Temperature register의 주소 저장 (칩 안의 레지스터를 변경 (config -> temp))
	
	rcall TWI_Start;		I2C 시작
	ldi I2Cdata, $99;	열 센서 관련 칩 주소 저장, 읽기 모드(1)
	rcall TWI_Write;	해당 칩 주소로 접근			(칩 불러오는 단계)

	ldi I2Cack, 1;		1을 저장 -> 계속 데이터 읽어오는 모드
	rcall TWI_Read;		해당 칩 주소 접근하여 데이터 읽기	(temprature register의 값들 읽어옴)
	mov dataH, r0;	우선 읽어온 상위 8비트 데이터를 TWIdataH에 저장

	ldi I2Cack, 0;		0을 저장 -> 이번 데이터만 읽어오고 종료
	rcall TWI_Read;		해당 칩 주소 접근하여 데이터 읽기	(temprature register의 값들 읽어옴)
	mov dataL, r0;	그 다음 읽어온 하위 8비트 데이터를 TWIdataL에 저장
	rcall TWI_Stop;			I2C 종료

	ret;

TWI_Start:				// I2C 시작 함수
	ldi temp, (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);	TWINT - 1로 초기화, TWSTA - I2C 시작, TWEN - TWI enable	
	sts TWCR, temp;		TWCR 레지스터 설정

	TWI_Wait1:			
	lds temp, TWCR;			TWCR레지스터값을 temp에 저장
	sbrs temp, TWINT;		TWINT가 다시 1로 초기화 되어있다면 작업 종료
	rjmp TWI_Wait1;			TWINT가 아직 0이면 작업이 끝나지 않았으므로 기다림

	ret;

TWI_Write:				// I2Cdata로 들어온 데이터, 혹은 주소를 TWDR에 저장하는 함수
	sts TWDR, I2Cdata;		TWDR에 I2Cdata 저장
	ldi temp, (1<<TWINT) | (1<<TWEN);		
	sts TWCR, temp;			TWCR의 TWINT 초기화, TWEN 1로 설정 (TWI enable)

	TWI_Wait2:
	lds temp, TWCR;			temp에 TWCR 저장
	sbrs temp, TWINT;		만약 TWCR의 TWINT가 다시 1로 초기화 되어있지 않았다면 작업 반복
	rjmp TWI_Wait2;

	ret;
	
TWI_Read:				// temperature register로부터 값을 읽어오는 함수
	ldi temp, (1<<TWINT) | (1<<TWEN) | (0<<TWEA);	TWEA는 0이므로 더 이상 데이터를 읽어오는 과정을 반복하지 않는다.
	cpi I2Cack, 0;		만약 I2Cack이 0이면
	breq SecondRead;	두번째 데이터값을 읽어오는 것으로 판단

	ldi temp, (1<<TWINT) | (1<<TWEN) | (1<<TWEA);	TWEA가 1이므로 데이터를 읽어오는 과정을 반복한다.

	SecondRead:
	sts TWCR, temp;		TWCR에 temp값 저장하여 레지스터 설정

	TWI_Wait3:
	lds temp, TWCR;
	sbrs temp, TWINT;		만약 TWINT가 다시 1로 초기화 되지 않았다면 동작반복
	rjmp TWI_Wait3;

	lds r0, TWDR;		r0에 TWDR(열 센서 데이터) 저장
	ret;

TWI_Stop:			// I2C 종료 함수
	ldi temp, (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);		
	sts TWCR, temp;			TWSTO 가 1이므로 I2C 를 종료한다.

	ldi temp, 100;

	TWI_Wait4:
	dec temp;
	brne TWI_Wait4;

	ret;

TWI_Init:			// I2C 관련 설정 초기화 함수
	ldi temp, 3;
	out PORTD, temp;		SCL, SDA enable

	ldi temp, 12;
	sts TWBR, temp;			통신 속도 400 kbps 설정

	ldi temp, (0<<TWPS1) | (0<<TWPS0);
	sts TWSR, temp;							prescaler value 0

	ret;

ModifyFND:
	push YH;
	push YL;				스택에 push

	ldi YH, high(digit4);
	ldi YL, low(digit4);	Y레지스터에 digit4 배열주소 저장

	ldi temp, 17;
	st Y+, temp;			digit4 첫 번째 인덱스는 공백(17)을 의미하도록 설정

	
	clr count;				십의 자리를 나타낼 count 변수 초기화
	mov temp, dataH;		상위 바이트를 temp에 저장

	Plus:	
	subi temp, 10;			상위 바이트 데이터 값에서 10을 뺀다	
	brlt Split;				만약 뺀 값이 음수가 되면 (10보다 작으면) Split 으로 이동
	
	inc count;				한 번 뺄때마다 10의자리수를 늘린다
	rjmp Plus;				더 뺄 수있으면 반복

	Split:
	subi temp, -10;			다시 10을 더한다		
	st Y+, count;			십의 자리수를 digit4 두 번째 인덱스로 사용
	st Y+, temp;			일의 자리수를 digit4 세 번재 인덱스로 사용

	mov temp, dataL;		하위 바이트를 temp에 저장
	andi temp, $80;
	cpi temp, 0;
	brne Half;				만약 하위 바이트의 최상위 비트에 1이 있으면 0.5도, 없으면 0도로 출력
	st Y, temp;
	
	pop YL;
	pop YH;					스택으로부터 pop
	ret;

	Half:
	ldi temp, 5;			0.5 도를 나타내기 위한 설정
	st Y, temp;				digit4 마지막 인덱스에 저장

	pop YL;
	pop YH;					스택으로부터 pop
	ret;




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

segdata:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71, $40, $00

.DSEG
digit4:
	.byte 4

	