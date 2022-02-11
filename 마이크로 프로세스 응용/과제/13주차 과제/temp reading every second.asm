.include "m128def.inc"

.def	TWIdataH	=	r14
.def	TWIdataL	=	r15
.def	temp		=	r16
.def	I2Cdata		=	r17	
.def	I2Cack		=	r18	// ACK 레지스터
.def	SREG2		=	r20
.def	segen		=	r21
.def	segcnt		=	r22
.def	timecnt		=	r23
.def	leddata		=	r24
.equ	FCLK		=	16000000
.equ	ATS75_CONFIG_REG	=	1				// 열 데이터를 어떤 식으로 저장할지 정하는 레지스터
.equ	ATS75_TEMP_REG		=	0				// 열 데이터를 갖고있는 레지스터



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
	rcall TempInit;					열 센서 관련 레지스터 초기화
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
	call TempRead;					열 센서에서 온도값을 읽어 TWIdataH, TWIdataL에 저장
	call ModifyFND;					열 센서에서 읽어온 데이터를 digit4 배열에 4개의 16진수 형태로 저장

DONE:
	pop SREG2;
	out SREG, SREG2;			status register 불러오기
	sei;					인터럽트 on
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
	mov TWIdataH, r0;	우선 읽어온 상위 8비트 데이터를 TWIdataH에 저장

	ldi I2Cack, 0;		0을 저장 -> 이번 데이터만 읽어오고 종료
	rcall TWI_Read;		해당 칩 주소 접근하여 데이터 읽기	(temprature register의 값들 읽어옴)
	mov TWIdataL, r0;	그 다음 읽어온 하위 8비트 데이터를 TWIdataL에 저장
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

	mov temp, TWIdataH;
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;				TWIdataH의 상위 4비트 추출
	st Y+, temp;			digit4에 첫 번째 인덱스에 저장

	ldi temp, $0F;
	and temp, TWIdataH;		TWIdataH 의 하위 4비트 추출
	st Y+, temp;			digit4 두 번재 인덱스에 저장

	mov temp, TWIdataL;
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;
	andi temp, $08;			TWIdataL 의 상위 4비트 추출
	st Y+, temp;			digit4 세 번째 인덱스에 저장

	ldi temp, $0F;
	and temp, TWIdataL;		TWIdataL 의 하위 4비트 추출
	st Y, temp;				digit4 네 번째 인덱스에 저장

	pop YL;
	pop YH;					스택으로부터 pop
	ret;


SegData:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71			// 0~F에 해당하는 led data

	.DSEG
digit4:					// 인덱스 배열
	.byte 4


	
	


	