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
.equ	LF		=	$0A				// �ٹٲ�
.equ	CR		=	$0D				// Ŀ�� �ʱ�ȭ
.equ	EOS		=	$00				// ���ڿ� ��
.equ	TAP		=	$09				// ��
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
	out SPH, temp;				 ���ü���

	rcall Initialize;			UART ��� ����
	rcall PrintTitle;			���� ����Ʈ

forever:
	rcall PrintCmdCursor;		Ŀ�� ����Ʈ
	rcall ReadCmd;				Ŀ���� �б�

	rcall CmdInterprete;		Ŀ���� �ؼ��Ͽ� �ش��ϴ� �����ϱ�
	rjmp forever;

Initialize:							// UART ��� ���� �ʱ�ȭ
	ldi temp, $00;		
	sts UBRR0H, temp;			U2X = 0	

	ldi temp, $67;
	out UBRR0L, temp;			UBRR=103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;			TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;			8bit data, 1 stop bit, No parity

	ret;
	
PrintTitle:						// �޴� �μ� �Լ�
	rcall NewLine;					���� ����
	ldi ZH, high(2*Title);
	ldi ZL, low(2*Title);			Title ���ڿ��� �����ϴ� �迭�� �ּҸ� Z�� ����
	rcall PutStringP;				���ڿ� ����Լ� ȣ��
	ret;

PutStringP:
	lpm r0, Z+;					Z�ּҸ� �������Ѱ��� �����͸� r0�� ����
	tst r0;						r0���� EOS���
	breq exit;					������ �Է� ����
	rcall PutChar;				�ٸ��ٸ� ������ ����ؼ� �Է�(����)
	rjmp PutStringP;			�ݺ�

exit:
	ret;

PutChar:						// �����͸� avr�� ���� ������ �͹̳ο� ����ϴ� �Լ�
	sbis UCSR0A, UDRE0;			������ ���� ���̸� putchar���� �ݺ�
	rjmp PutChar;				
	out UDR0, r0;				������ ��� �̷����� r0��(AVR�������Ͱ�)�� ������ �͹̳η� ����
	ret;

NewLine:
	ldi temp, LF;
	mov r0, temp;				r0 �� �ٹٲް� ����
	call PutChar;				�ٹٲ��� avr�� ���� ������ �͹̳ο� ���
	ldi temp, CR;				
	mov r0, temp;				r0�� Ŀ�� ó������ �ű�� ������ ����
	call PutChar;				�̸� ������ �͹̳��� ���� ���
	ret;

Title:																	// Ÿ��Ʋ ���ڿ�, constant��
	.db TAP,"+---------------------------------------+",CR,LF;
	.db TAP,"|This is an AVR Test Board              |",CR,LF; 
	.db TAP,"|By Hwang Hyun , 201711282              |",CR,LF;
	.db TAP,"|Following functions are provided       |",CR,LF;
	.db TAP,"|1) LED Display                         |",CR,LF;
	.db TAP,"|2) 7 Segment Display                   |",CR,LF;
	.db TAP,"|3) Buzzer Test                         |",CR,LF;
	.db TAP,"|4) Photo Sensor Test                   |",CR,LF;
	.db TAP,"|5) Temperature Sensor test             |",CR,LF;
	.db TAP,"+---------------------------------------+",CR,LF,EOS,EOS ;		// padding zero bytes �� ���� ���ϴ� ������ �ɸ��� ���� �� �ִ�. �����Ұ�

cmdcursor:
	.db TAP,"Input Function Number:  ",EOS;						// Ŀ�� ���ڿ�, constant

PrintCmdCursor:												// Ŀ���� ����ϴ� �Լ�
	CALL NewLine;					 ���� ����
	ldi ZH, high(2*cmdcursor);
	ldi ZL, low(2*cmdcursor);		Z�� Ŀ�����ڿ� �ּ� ����
	call PutStringP;				���ڿ� ����ϱ�
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
	sbis UCSR0A, RXC0;			���� �����͸� ���޹޴� ���̸� ����ؼ� Getchar ���� �ݺ�
	rjmp GetChar;
	in r0, UDR0;				�����͸� ��� �Է¹����� UDR0���� AVR r0 �������Ϳ� ����
	ret;

GetString:						//���ڿ� �޴� �Լ�
	rcall GetChar;		
	call PutChar;				Echoing
	st Y+, r0;					Y�ּҿ� �ش��ϴ� �޸� ������ ������ �͹̳ηκ��� �Է¹��� ���� ����
	ldi temp, CR;				temp�� ����Ű�� �ش��ϴ� �ƽ�Ű�� ����
	cp r0, temp;				����Ű�� ������ ���ڿ� �Է¹ޱⰡ ������.
	brne GetString;				���� ����Ű�� ������ �ʾҴٸ� ����ؼ� ���ڿ��� �Է¹޴´�.
	clr r0;						r0�� EOS�� ����
string:
	st Y, r0;					���ڿ� �������� EOS ����
	ret;

CmdInterprete:					// Ŀ���� �ؼ� �Լ�
	ldi XH, high(buffer);
	ldi XL, low(buffer);		�����ּҸ� X�������Ϳ� ����
	ld temp, X;					���ۿ��� �ҷ����� �� temp�� ����

	cpi temp, '1';				���� ���ۿ� �ִ� ���� 1�̸�
	breq CMD_LED;				LED test

	cpi temp, '2';				���ۿ� �ִ� ���� 2�̸�
	breq CMD_Segment;			7 segment test
	
	cpi temp, '3';				���ۿ� �ִ� ���� 3�̸�
	breq CMD_Buzzer;			buzzer test

	cpi temp, '4';				���ۿ� �ִ� ���� 4�̸�
	breq CMD_PhotoSensor;		Photo sensor test

	cpi temp, '5';				���ۿ� �ִ� ���� 5�̸�
	breq CMD_TempSensor;		Temp sensor test

	rjmp CMD_Wrong;				���ۿ� �ִ� ���� 1, 2, 3, 4, 5 �� �ƴϸ� Wrong command ���

CMD_Done:
	ret;

CMD_LED:						// LED Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*led);
	ldi ZL, low(2*led);
	call PutStringP;
	call LED_Test;
	rjmp CMD_Done;

CMD_Segment:					// 7segment Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*seg);
	ldi ZL, low(2*seg);
	call PutStringP;
	call Seg_Test;
	rjmp CMD_Done;

CMD_Buzzer:						// Buzzer Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*buz);
	ldi ZL, low(2*buz);
	call PutStringP;
	rjmp CMD_Done;

CMD_PhotoSensor:				// PhotoSensor Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*pho);
	ldi ZL, low(2*pho);
	call PutStringP;
	call Photo_Test;
	rjmp CMD_Done;

CMD_TempSensor:					// TempSensor Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*tempS);
	ldi ZL, low(2*tempS);
	call PutStringP;
	call Temp_Test;
	rjmp CMD_Done;

CMD_Wrong:						// wrong comman�� �Էµ��� ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*wrong);
	ldi ZL, low(2*wrong);
	call PutStringP;
	rjmp CMD_Done;		

LED_Test:					// led test �Լ�
	ldi temp, $FF;
	out DDRA, temp;				led 8�� ��� ������� ���
	ldi leddata, $80;
	
	repeat1:
	out PORTA, leddata;			8�� led ����
	rcall delay1sec;			1�� ����
	lsr leddata;				���������� ������		
	brne repeat1;				���� 0�� �Ǹ� led test ����, �ƴϸ� �ݺ�
	
	out PORTA, leddata;
	ret;	
	
Seg_Test:					// 7 segment test �Լ�
	push segen;
	push segcnt;

	ldi temp, $FF;
	out DDRC, temp;			7segment ��� ������� ���

	ldi temp, $0F;
	sts DDRG, temp;			4���� ������ ��� ������� ���

	ldi segen, $08;			

	loop2:
	sts PORTG, segen;		segen�� �ش��ϴ� ������ ������� ���

	ldi segcnt, 10;			0~9���� 10���� ���ڸ� ��Ÿ���� ���� ī��Ʈ

	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);		�ε��� �ּҸ� Z�� ����

	repeat2:
	lpm r0, Z+;					Z��(7seg ���ڰ�) ������Ű�� r0�� ����				
	out PORTC, r0;				�ش��ϴ� ���ڰ��� ��� 
	call delay1sec;				�ð� ����
	subi segcnt, 1;				cnt ���� (0~9 ���� ǥ���ϱ� ����)
	brne repeat2;				���� cnt�� ���������� �ݺ�

	lsr segen;					0~9���� ��� ǥ�������� segen�� ���� (������ ����)
	breq end;					
	rjmp loop2;					������ �����Ǳ��� ��� ������� �ʾҴٸ� loop2�� ���� �ݺ�

	end:						// ������ ���ڱ��� ��� ǥ�������� �Լ��� ����������.
	clr r0;
	out PORTC, r0;				������ led ���ڸ� ���ش�
	pop segcnt;
	pop segen;
	ret;	
	
Photo_Test:						// photo sensor test �Լ�
	push segen;
	push segcnt;
	push timecnt;

	ldi temp, $ff;
	out DDRC, temp;				7segment ������� ���

	ldi temp, $0f;
	sts DDRG, temp;				4�� ������ ��� ������� ���

	cli;
	call AD_Init;
	call Timer_Init;			�ʱ�ȭ
	sei;

	ldi XH, high(digit4);
	ldi XL, low(digit4);		�ε��� �迭�ּ� X�� ����

	ldi segen, $08;
	ldi timecnt, 0;
	ldi escape, 10;				10�ʰ� ������ �Լ��� ������������ ������ ����
	ldi test, 1;				test ������ 1�̸� Ÿ�̸� ���ͷ�Ʈ���� photo sensor�� ���۽�Ų��
	
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
	brge repeat3;				���� escape ������ 1���� �۾����� (10�ʰ� ������) �Լ��� ����������.
	
	clr test;					test ������ 0���� ������� Ÿ�̸� ���ͷ�Ʈ ������ test������ ���ߵ��� �Ѵ�
	out PORTC, test;			���� ������ �ִ� 7 segment���� ���ش�
	pop timecnt;
	pop segcnt;
	pop segen;
	ret;

		

delay1sec:						// 1�� �����Լ�
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

AD_Init:					// photo sensor �ʱ�ȭ
	clr temp;
	out ADMUX, temp;

	ldi temp, $87;
	out ADCSRA, temp;

Read_ADC:					// photo sensor�� ���� �������� �о���� �Լ�
	in temp, ADCSRA;
	ori temp, $40;
	out ADCSRA, temp;

	in dataL, ADCL;
	in dataH, ADCH;

Show_ADC:					// photo sensor�κ��� �о�� �������� digit4�� �����ϴ� �Լ�
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

Timer_Init:				// Ÿ�̸� ���ͷ�Ʈ �ʱ�ȭ �Լ�
	ldi temp, $06;
	out TCCR0, temp;

	ldi temp, $00;
	out TCNT0, temp;

	ldi temp, 01;
	out TIMSK, temp;
	ret;

TIM0_OVF:			// Ÿ�̸� ���ͷ�Ʈ
	cli;
	in SREG2, SREG;
	push SREG2;

	cpi test, 0;		 ���� test ������ 0�̸� �ٷ� Ÿ�̸� ���ͷ�Ʈ ����
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
	cpi test, 1;			���� test������ 1�̸� photo sensor test�̹Ƿ� ADCtest�� �̵�		
	breq ADCtest;

	cpi test, 2;			���� test������ 2�̸� Temp test�̹Ƿ� Temptest�� �̵�
	breq Temptest;

	rjmp DONE;

	ADCtest:
	dec escape;				1�ʸ��� escape ������ ���ҽ�Ų��
	call Read_ADC;
	call Show_ADC;
	rjmp DONE;

	Temptest:
	dec escape;				1�ʸ��� escape ������ ���ҽ�Ų��
	call TempRead;
	call ModifyFND;
	rjmp DONE;

	DONE:
	pop SREG2;
	out SREG, SREG2;
	sei;
	ret;

Temp_Test:				// Temperature �Լ�
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
	ldi escape, 10;				10�ʰ� �����ϱ� ���� escape ���� 10
	ldi test, 2;				test������ 2�� �־� Ÿ�̸� ���ͷ�Ʈ���� Temptest�� �����ϵ�����	

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
	cpi escape, 1;				���� 10�ʰ� ������ �Լ��� ����������
	brge repeat4;

	rjmp return;


	Dot:
	ori leddata, $80;
	out PORTC, leddata;
	cpi escape, 1;				���� 10�ʰ� ������ �Լ��� ����������		
	brge repeat4;

	return:
	clr test;
	out PORTC, test;
	pop timecnt;
	pop segcnt;
	pop segen;
	ret;

TempInit:				// �� ���� ���� �ʱ�ȭ
	rcall TWI_Init;		I2C ���� �ʱ�ȭ
	
	rcall TWI_Start;	I2C ����	

	ldi I2Cdata, $98;	�� ���� ���� Ĩ �ּ� ����, ���� ���(0)
	rcall TWI_Write;	�ش� Ĩ �ּҷ� ����			(Ĩ�� �ҷ����� �ܰ�)

	ldi I2Cdata, ATS75_CONFIG_REG;		
	rcall TWI_Write;			Ĩ �ȿ� Configuration register�� �ּ� ����	(Ĩ ���� �������Ϳ� �����ϴ� �ܰ�)

	ldi I2Cdata, 0;		I2Cdata�� 0���� 
	rcall TWI_Write;	configuration register�� 0�� ����		(Ĩ �� �������Ϳ� ������ 0 ����ֱ�)

	rcall TWI_Stop;		I2C ����

	ret;

TempRead:				// �� �����κ��� �����͸� �д� �Լ�
	rcall TWI_Start;		I2C ����

	ldi I2Cdata, $98;	�� ���� ���� Ĩ �ּ� ����, ���� ���(0)
	rcall TWI_Write;	�ش� Ĩ �ּҷ� ����			(Ĩ �ҷ����� �ܰ�)

	ldi I2Cdata, ATS75_TEMP_REG;		I2Cdata�� �Ű�����
	rcall TWI_Write;					Ĩ ���� Temperature register�� �ּ� ���� (Ĩ ���� �������͸� ���� (config -> temp))
	
	rcall TWI_Start;		I2C ����
	ldi I2Cdata, $99;	�� ���� ���� Ĩ �ּ� ����, �б� ���(1)
	rcall TWI_Write;	�ش� Ĩ �ּҷ� ����			(Ĩ �ҷ����� �ܰ�)

	ldi I2Cack, 1;		1�� ���� -> ��� ������ �о���� ���
	rcall TWI_Read;		�ش� Ĩ �ּ� �����Ͽ� ������ �б�	(temprature register�� ���� �о��)
	mov dataH, r0;	�켱 �о�� ���� 8��Ʈ �����͸� TWIdataH�� ����

	ldi I2Cack, 0;		0�� ���� -> �̹� �����͸� �о���� ����
	rcall TWI_Read;		�ش� Ĩ �ּ� �����Ͽ� ������ �б�	(temprature register�� ���� �о��)
	mov dataL, r0;	�� ���� �о�� ���� 8��Ʈ �����͸� TWIdataL�� ����
	rcall TWI_Stop;			I2C ����

	ret;

TWI_Start:				// I2C ���� �Լ�
	ldi temp, (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);	TWINT - 1�� �ʱ�ȭ, TWSTA - I2C ����, TWEN - TWI enable	
	sts TWCR, temp;		TWCR �������� ����

	TWI_Wait1:			
	lds temp, TWCR;			TWCR�������Ͱ��� temp�� ����
	sbrs temp, TWINT;		TWINT�� �ٽ� 1�� �ʱ�ȭ �Ǿ��ִٸ� �۾� ����
	rjmp TWI_Wait1;			TWINT�� ���� 0�̸� �۾��� ������ �ʾ����Ƿ� ��ٸ�

	ret;

TWI_Write:				// I2Cdata�� ���� ������, Ȥ�� �ּҸ� TWDR�� �����ϴ� �Լ�
	sts TWDR, I2Cdata;		TWDR�� I2Cdata ����
	ldi temp, (1<<TWINT) | (1<<TWEN);		
	sts TWCR, temp;			TWCR�� TWINT �ʱ�ȭ, TWEN 1�� ���� (TWI enable)

	TWI_Wait2:
	lds temp, TWCR;			temp�� TWCR ����
	sbrs temp, TWINT;		���� TWCR�� TWINT�� �ٽ� 1�� �ʱ�ȭ �Ǿ����� �ʾҴٸ� �۾� �ݺ�
	rjmp TWI_Wait2;

	ret;
	
TWI_Read:				// temperature register�κ��� ���� �о���� �Լ�
	ldi temp, (1<<TWINT) | (1<<TWEN) | (0<<TWEA);	TWEA�� 0�̹Ƿ� �� �̻� �����͸� �о���� ������ �ݺ����� �ʴ´�.
	cpi I2Cack, 0;		���� I2Cack�� 0�̸�
	breq SecondRead;	�ι�° �����Ͱ��� �о���� ������ �Ǵ�

	ldi temp, (1<<TWINT) | (1<<TWEN) | (1<<TWEA);	TWEA�� 1�̹Ƿ� �����͸� �о���� ������ �ݺ��Ѵ�.

	SecondRead:
	sts TWCR, temp;		TWCR�� temp�� �����Ͽ� �������� ����

	TWI_Wait3:
	lds temp, TWCR;
	sbrs temp, TWINT;		���� TWINT�� �ٽ� 1�� �ʱ�ȭ ���� �ʾҴٸ� ���۹ݺ�
	rjmp TWI_Wait3;

	lds r0, TWDR;		r0�� TWDR(�� ���� ������) ����
	ret;

TWI_Stop:			// I2C ���� �Լ�
	ldi temp, (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);		
	sts TWCR, temp;			TWSTO �� 1�̹Ƿ� I2C �� �����Ѵ�.

	ldi temp, 100;

	TWI_Wait4:
	dec temp;
	brne TWI_Wait4;

	ret;

TWI_Init:			// I2C ���� ���� �ʱ�ȭ �Լ�
	ldi temp, 3;
	out PORTD, temp;		SCL, SDA enable

	ldi temp, 12;
	sts TWBR, temp;			��� �ӵ� 400 kbps ����

	ldi temp, (0<<TWPS1) | (0<<TWPS0);
	sts TWSR, temp;							prescaler value 0

	ret;

ModifyFND:
	push YH;
	push YL;				���ÿ� push

	ldi YH, high(digit4);
	ldi YL, low(digit4);	Y�������Ϳ� digit4 �迭�ּ� ����

	ldi temp, 17;
	st Y+, temp;			digit4 ù ��° �ε����� ����(17)�� �ǹ��ϵ��� ����

	
	clr count;				���� �ڸ��� ��Ÿ�� count ���� �ʱ�ȭ
	mov temp, dataH;		���� ����Ʈ�� temp�� ����

	Plus:	
	subi temp, 10;			���� ����Ʈ ������ ������ 10�� ����	
	brlt Split;				���� �� ���� ������ �Ǹ� (10���� ������) Split ���� �̵�
	
	inc count;				�� �� �������� 10���ڸ����� �ø���
	rjmp Plus;				�� �� �������� �ݺ�

	Split:
	subi temp, -10;			�ٽ� 10�� ���Ѵ�		
	st Y+, count;			���� �ڸ����� digit4 �� ��° �ε����� ���
	st Y+, temp;			���� �ڸ����� digit4 �� ���� �ε����� ���

	mov temp, dataL;		���� ����Ʈ�� temp�� ����
	andi temp, $80;
	cpi temp, 0;
	brne Half;				���� ���� ����Ʈ�� �ֻ��� ��Ʈ�� 1�� ������ 0.5��, ������ 0���� ���
	st Y, temp;
	
	pop YL;
	pop YH;					�������κ��� pop
	ret;

	Half:
	ldi temp, 5;			0.5 ���� ��Ÿ���� ���� ����
	st Y, temp;				digit4 ������ �ε����� ����

	pop YL;
	pop YH;					�������κ��� pop
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

	