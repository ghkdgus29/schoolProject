.include "m128def.inc"

.def	TWIdataH	=	r14
.def	TWIdataL	=	r15
.def	temp		=	r16
.def	I2Cdata		=	r17	
.def	I2Cack		=	r18	// ACK ��������
.def	SREG2		=	r20
.def	segen		=	r21
.def	segcnt		=	r22
.def	timecnt		=	r23
.def	leddata		=	r24
.equ	FCLK		=	16000000
.equ	ATS75_CONFIG_REG	=	1				// �� �����͸� � ������ �������� ���ϴ� ��������
.equ	ATS75_TEMP_REG		=	0				// �� �����͸� �����ִ� ��������



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
	out SPH, temp;					���� ����

	ldi temp, $ff;					
	out DDRC, temp;					7segment ��� ������� ���

	ldi temp, $0f;
	sts DDRG, temp;					4�� ������ ��� ������� ���
	
	cli;				���ͷ�Ʈ off
	rcall TempInit;					�� ���� ���� �������� �ʱ�ȭ
	call Timer_Init;
	sei;				���ͷ�Ʈ on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);			x �������Ϳ� digit4 �ּҸ� ���� (�ε���)

	ldi segen, $08;			������ ������ �ڸ��� ������ ���� ����
	ldi timecnt, 0;			Ÿ�̸� ���ͷ�Ʈ�� �߻��� Ƚ���� �����ϱ� ���� ����

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);			�ε��� �迭�ּҸ� X�� ����

	clr r0;
	add XL, segcnt;
	adc XH, r0;						������ �ڸ����� �ش��ϴ¸�ŭ ���� �ε��� �迭�� �ּҸ� X�� ����

	ldi ZH, High(2*SegData);
	ldi ZL, Low(2*SegData);			led data (7segment) �� ������ �迭�ּҸ� Z�� ����

	ld temp, X;					
	clr r0;
	add ZL, temp;
	adc ZH, r0;						�ε��� ��ŭ Z�ּҿ� ���Ѵ�

	lpm leddata, Z;					Z�ּҿ� �ش��ϴ� led data�� �������� leddata�� ����		
	sts PORTG, segen;				segen�� ����Ǿ� �ִ� �ڸ����� �������� ���
	out PORTC, leddata;				leddata�� ����Ǿ��ִ� led data�� ���
	
	rjmp forever;		

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
			
Timer_Init:					// Ÿ�̸� ���ͷ�Ʈ �ʱ�ȭ ����
	ldi temp, $06;
	out TCCR0, temp;		16MHZ/256���� timer clock �����ϸ�

	ldi temp, $00;
	out TCNT0, temp;		���ͷ�Ʈ�� �߻��ϰ� ���� Ÿ�̸Ӹ� �ٽ� 0���� �ʱ�ȭ

	ldi temp, 01;
	out TIMSK, temp;		�����÷ο� �߻��� ���ͷ�Ʈ �߻�
	ret;

TIM0_OVF:				// Ÿ�̸� ���ͷ�Ʈ
	cli;					���ͷ�Ʈ off
	in SREG2, SREG;				status register ����
	push SREG2;

	lsr segen;					������ �ڸ��� ��ĭ ����
	inc segcnt;					�ε��� �迭�� ������ �ּҿ� ���� segcnt ����
	cpi segen, $00;				���� ������ �ڸ����� 
	brne Skip_resetsegen;		0�� �ƴϸ� skip
	ldi segen, $08;				0�̸� �ٽ� 0000 1000 ���� �ʱ�ȭ
	clr segcnt;					segcnt�� 0���� �ʱ�ȭ

Skip_resetsegen:
	inc timecnt;
	cpi timecnt, 244;			Ÿ�̸� ���ͷ�Ʈ�� 244�� �߻��ϸ� 1�ʿ� �ش��Ѵ�
	brne DONE;					1�ʰ� ������ ������ �ƹ��ϵ� �Ͼ�� �ʴ´�.

second:							// 1�ʰ� ������
	call TempRead;					�� �������� �µ����� �о� TWIdataH, TWIdataL�� ����
	call ModifyFND;					�� �������� �о�� �����͸� digit4 �迭�� 4���� 16���� ���·� ����

DONE:
	pop SREG2;
	out SREG, SREG2;			status register �ҷ�����
	sei;					���ͷ�Ʈ on
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
	mov TWIdataH, r0;	�켱 �о�� ���� 8��Ʈ �����͸� TWIdataH�� ����

	ldi I2Cack, 0;		0�� ���� -> �̹� �����͸� �о���� ����
	rcall TWI_Read;		�ش� Ĩ �ּ� �����Ͽ� ������ �б�	(temprature register�� ���� �о��)
	mov TWIdataL, r0;	�� ���� �о�� ���� 8��Ʈ �����͸� TWIdataL�� ����
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

	mov temp, TWIdataH;
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;				TWIdataH�� ���� 4��Ʈ ����
	st Y+, temp;			digit4�� ù ��° �ε����� ����

	ldi temp, $0F;
	and temp, TWIdataH;		TWIdataH �� ���� 4��Ʈ ����
	st Y+, temp;			digit4 �� ���� �ε����� ����

	mov temp, TWIdataL;
	lsr temp;
	lsr temp;
	lsr temp;
	lsr temp;
	andi temp, $08;			TWIdataL �� ���� 4��Ʈ ����
	st Y+, temp;			digit4 �� ��° �ε����� ����

	ldi temp, $0F;
	and temp, TWIdataL;		TWIdataL �� ���� 4��Ʈ ����
	st Y, temp;				digit4 �� ��° �ε����� ����

	pop YL;
	pop YH;					�������κ��� pop
	ret;


SegData:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71			// 0~F�� �ش��ϴ� led data

	.DSEG
digit4:					// �ε��� �迭
	.byte 4


	
	


	