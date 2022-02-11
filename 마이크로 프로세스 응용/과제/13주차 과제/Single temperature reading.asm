.include "m128def.inc"
.equ	FCLK				=	16000000
.equ	F_SCK				=	400000
.equ	ATS75_ADDR			=	$98				// �� ���� ���� Ĩ�� �����ϱ� ���� �ּ�
.equ	ATS75_CONFIG_REG	=	1				// �� �����͸� � ������ �������� ���ϴ� ��������
.equ	ATS75_TEMP_REG		=	0				// �� �����͸� �����ִ� ��������

	.org 0x0000;
	jmp RESET;

.def TWIdataH = r14
.def TWIdataL = r15

.def temp = r16

.def FND_En = r17		// led Enable control register
.def FND_Data = r18		// led data register

.def I2Cdata = r19	
.def I2Cack = r20	// ACK ��������

	.org 0x0046;
RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;			���� ����

	rcall FND_Init;
	rcall TempInit;			�� ���� ���� �������� �ʱ�ȭ

	rcall TempRead;			�� �������� �µ����� �о� TWIdataH, TWIdataL�� ����

	rcall ModifyFND;		�� �������� �о�� �����͸� digit4 �迭�� 4���� 16���� ���·� ����

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);
	ldi FND_En, $08;

loop4:
	ldi ZH, high(2*segdata);
	ldi ZL, low(2*segdata);			7segment���� ���� �ִ� �迭
	
	ld temp, X+;					digit4�迭�� �ε��� ���� temp�� ����

	clr r0;
	add ZL, temp;					
	adc ZH, r0;						7segment���� �ε������� ���Ͽ� �ش��ϴ� led data���� ����Ű�� �ּ� Z�� ����

	lpm FND_Data, Z;				*Z ���� FND_Data�� ���� , �̴� led data��
	sts PORTG, FND_En;				FND_En�� �ش��ϴ� �κ��� �������� ����
	out PORTC, FND_Data;			led data���� led�� ���

	rcall delay1mS;					���� ���� �ð� �����̸� �־� ���������� �̿�, ����ڴ� �׻� ����� �� ó�� ����
	lsr FND_En;						led control �������͸� ���������� ����Ʈ (�� ���� ������ ����)
	brne loop4;						���� led control �������Ͱ� 0�� �ƴϸ� loop4�ݺ�

	rjmp forever;					led control �������Ͱ� 0�� �Ǹ� forever�� �̵��Ͽ� �������͸� �ٽ� $08�� �ʱ�ȭ

FND_Init:				// led���� �������� �ʱ�ȭ
	ser temp;
	out DDRC, temp;		led ��� ��´����� ���

	ldi temp, $0F;
	sts DDRG, temp;		������ 4�� ��� ��������� ����
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

delay1mS:			// �ð� ���� �Լ�
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

segdata:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f, $77, $7c, $39, $5e, $79, $71			// 0~F�� �ش��ϴ� led data

	.DSEG
digit4:					// �ε��� �迭
	.byte 4
