.include "m128def.inc"
	rjmp	RESET
.def	temp = r16						// 임시변수
.def	segdata	= r17					// led data 값을 저장할 레지스터
.def	segen = r18						// 포트G에 넣을 입력값을 저장할 레지스터
.def	segcnt = r19					// 카운트, 4를 입력하여 4개의 LED에 숫자를 번갈아가며 사용할 것임
.equ	FCLK = 16000000					// delay를 위한 클락카운트
	.ORG 0x046;
RESET:
	ldi	temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;						// 스택포인터 설정

	ser temp;							
	out DDRC, temp;						// LED의 7segment를 모두 출력으로 사용하기위해 포트C에 0xFF값을 넣어줌

	ldi temp, $0F;
	sts DDRG, temp;						// 4개의 LED 숫자판을 이용하기 위해 포트G에 0x0F값을 넣어준다.

	ldi XH, High(digit4);				
	ldi XL, Low(digit4);				// 데이터 메모리의 digit4 주소를 X레지스터의 저장

	ldi temp, 1;
	st X+, temp;
	ldi temp, 2;
	st X+, temp;
	ldi temp, 3;
	st X+, temp;
	ldi temp, 4;
	st X, temp;							// digit4 에 1, 2, 3, 4 저장, 배열과 유사

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);				// digit4 첫 주소를 X레지스터에 저장
	ldi segen, $08;						// 첫번째 숫자판에만 불이들어오도록 segen에 저장
	ldi segcnt, 4;						// 카운트를 위한 4 저장


loop4:
	ldi ZH, HIGH(2*Seg_Data);	
	ldi ZL, LOW(2*Seg_Data);			// Z레지스터에 LED segment data 배열 주소 저장 

	ld temp, X+;						// temp에 X주소를 저장하며 증가시킴
	clr r0;
	add ZL, temp;						// ZL에 temp변수를 더한다. 즉 digit4의 1,2,3,4 가 더해진다.
	adc ZH, r0;							// 캐리의 발생이 일어났을 때를 위해 adc 수행, r0 은 0이다.

	lpm segdata, Z;						// 이렇게 더해진 Z주소의 segment data 값을 segdata에 저장
	sts PORTG, segen;					// 포트G에 segen 에 해당하는 숫자판만 켜지도록 설정
	out PORTC, segdata;					// segdata에 저장된 led data 값을 포트 C를 통해 출력

	ldi temp, 1;						// temp에 숫자를 저장, 이 숫자는 지연시킬 ms의 크기를 의미한다.
	rcall Delay_Tms;					// 지연함수 호출
	lsr segen;							// segen 을 우측 쉬프트 시켜 다음번 숫자판에 불이 들어오도록 설정

	dec segcnt;							// 카운트 감소
	brne loop4;							// 카운트가 0이되기 전엔 loop4를 계속해서 돈다.	

	rjmp forever;						// 만약 카운트가 0이 되면 forever로 돌아가 반복한다.

Delay_1ms:								// 1ms 의 시간지연함수
	push YL;
	push YH;
	ldi YL, low(((FCLK/1000)-18)/4);
	ldi YH, high(((FCLK/1000)-18)/4);

delay1ms_01:
	sbiw YH:YL, 1;
	brne delay1ms_01;

	pop YH;
	pop YL;
	ret;

Delay_Tms:								// Tms의 시간지연함수, temp로 T를 받는다.
	call Delay_1ms;
	dec temp;
	brne Delay_Tms;

	ret;

Seg_Data:								// LED data 값이 저장되어 있는 프로그램  메모리 배열이다.
	.db	$3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

	.DSEG
digit4:									// 인덱싱할 숫자들이 들어있는 데이터 메모리 공간이다.
	.byte 4
		

