.include "m128def.inc"
.equ	FCLK				=	16000000
.equ	F_SCK				=	400000
.equ	ATS75_ADDR			=	$98				// 열 전도 관련 칩에 접근하기 위한 주소
.equ	ATS75_CONFIG_REG	=	1				// 열 데이터를 어떤 식으로 저장할지 정하는 레지스터
.equ	ATS75_TEMP_REG		=	0				// 열 데이터를 갖고있는 레지스터

	.org 0x0000;
	jmp RESET;

.def TWIdataH = r14
.def TWIdataL = r15

.def temp = r16

.def FND_En = r17		// led Enable control register
.def FND_Data = r18		// led data register

.def I2Cdata = r19	
.def I2Cack = r20	// ACK 레지스터

	.org 0x0046;
RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;			스택 설정

	rcall FND_Init;
	rcall TempInit;			열 센서 관련 레지스터 초기화

	rcall TempRead;			열 센서에서 온도값을 읽어 TWIdataH, TWIdataL에 저장

	rcall ModifyFND;		열 센서에서 읽어온 데이터를 digit4 배열에 4개의 16진수 형태로 저장

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);
	ldi FND_En, $08;

loop4:
	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);			7segment값을 갖고 있는 배열
	
	ld temp, X+;					digit4배열의 인덱스 값을 temp에 저장

	clr r0;
	add ZL, temp;					
	adc ZH, r0;						7segment값에 인덱스값을 더하여 해당하는 led data값을 가리키는 주소 Z를 만듬

	lpm FND_Data, Z;				*Z 값을 FND_Data에 저장 , 이는 led data임
	sts PORTG, FND_En;				FND_En에 해당하는 부분의 숫자판을 점등
	out PORTC, FND_Data;			led data값을 led로 출력

	rcall delay1mS;					아주 작은 시간 딜레이를 주어 착시현상을 이용, 사용자는 항상 점등된 것 처럼 보임
	lsr FND_En;						led control 레지스터를 오른쪽으로 쉬프트 (그 다음 숫자판 점등)
	brne loop4;						만약 led control 레지스터가 0이 아니면 loop4반복

	rjmp forever;					led control 레지스터가 0이 되면 forever로 이동하여 레지스터를 다시 $08로 초기화

FND_Init:				// led관련 레지스터 초기화
	ser temp;
	out DDRC, temp;		led 모두 출력단으로 사용

	ldi temp, $0F;
	sts DDRG, temp;		숫자판 4개 모두 점등가능으로 설정
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

delay1mS:			// 시간 지연 함수
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

segdata:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71			// 0~F에 해당하는 led data

	.DSEG
digit4:					// 인덱스 배열
	.byte 4
