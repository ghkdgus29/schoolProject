.include "m128def.inc"
.def	temp	=	r16
.def	leddata	=	r17
.def	segen	=	r18
.def	timecnt	=	r19
.def	SREG2	=	r20			// status �������͸� ������ ��������
.def	segcnt	=	r21
.def	stopgo	=	r22			// Ÿ�̸Ӹ� �Ͻ��������� , �簳���� �����ϴ� ��������, 0�̸� ��������, 1�̸� ���ۻ����̴�.
.equ	FCLK	=	16000000

		.ORG 0;
		rjmp RESET;
		.ORG 0x000A;
		jmp EXT_INT4;		�ܺ� ���ͷ�Ʈ 4��, 1�� ����ġ�� ���� ��� ����
		jmp EXT_INT5;		�ܺ� ���ͷ�Ʈ 5��, 2�� ����ġ�� ���� ��� ����
		.ORG 0x0020;
		rjmp TIM0_OVF;      Ÿ�̸� ���ͷ�Ʈ
		.ORG 0x0046;

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					���������� ����

	ser temp;
	out DDRC, temp;			7segment ��� ����ϱ� ���� ����

	ldi temp, $0F;
	sts DDRG, temp;			4�� �������� ��� ����ϱ� ���� ����

	cli;			���ͷ�Ʈ disable
	rcall Timer0_Init;		Ÿ�̸� ���ͷ�Ʈ �ʱ�ȭ ����
	rcall EXINT_Init;		�ܺ� ���ͷ�Ʈ �ʱ�ȭ ����
	sei;			���ͷ�Ʈ on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);	x �������Ϳ� digit4 �ּҸ� ���� (�ε���)

	ldi temp, 0;
	st X+, temp;
	ldi temp, 0;
	st X+, temp;
	ldi temp, 0;
	st X+, temp;
	ldi temp, 0;
	st X, temp;				�ε����� ���ڸ� ���ʷ� �����Ѵ� (digit4�� ����)

	ldi segen, $08;
	ldi timecnt, 0;
	ldi stopgo,	0;			ó���� Ÿ�̸� ��������

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);

	clr r0;
	add XL, segcnt;
	adc XH, r0;				 segcnt�� �ε��̳ѹ� �ڸ����� �ǹ��ϸ� �̸� x�������Ϳ� ���Ͽ� 1��, 10��, 1��, 10�� �ε��� ������ �����Ѵ�.

	ldi ZH, HIGH(2*SegData);
	ldi ZL, LOW(2*SegData);			z�������Ϳ� 7segment�� ǥ���ϱ� ���� ������ ����Ǿ� �ִ� SegData�ּҸ� ����

	ld temp, X;
	clr r0;
	add ZL, temp;
	adc ZH, r0;						z�������Ϳ� X�� ����Ǿ� �ִ� �ε��� �ѹ��� ����

	lpm leddata, Z;					�̷��� �ε��� �ѹ����� ���� ���� leddata�� ����
	sts PORTG, segen;				segen���� �ش��ϴ� ������ ����
	out PORTC, leddata;				leddata���� �ش��ϴ� 7segment ���� �ش� �����ǿ� ���

	rjmp forever;
		
Timer0_Init:				// Ÿ�̸� ���ͷ�Ʈ �ʱ�ȭ ����
	ldi temp, $02;
	out TCCR0, temp;		16MHZ/8�� timer clock �����ϸ�

	ldi temp, $00;
	out TCNT0, temp;		���ͷ�Ʈ�� �߻��ϰ� ���� Ÿ�̸Ӹ� �ٽ� 0���� �ʱ�ȭ

	ldi temp, 01;
	out TIMSK, temp;		�����÷ο� �߻��� ���ͷ�Ʈ �߻�
	ret;

EXINT_Init:					// �ܺ� ���ͷ�Ʈ �ʱ�ȭ ����
	ldi temp, $00;
	sts EICRA, temp;
	
	ldi temp, $0A;
	sts EICRB, temp;        �ܺ� ���ͷ�Ʈ 4��, 5���� falling edge ���ͷ�Ʈ�� ���

	ldi temp, $30;
	out EIMSK, temp;		�ܺ� ���ͷ�Ʈ 4�� 5���� ��밡���ϵ��� ����
	ret;

TIM0_OVF:					// Ÿ�̸� ���ͷ�Ʈ 
	cli;			���ͷ�Ʈ disable
	in SREG2, SREG;
	push SREG2;				status register �� ���ÿ� ����

	lsr segen;				segen ���� ������, ������ �̵�
	inc segcnt;				segcnt(������ �ڸ���) ����
	cpi segen, $00;			
	brne Skip_resetsegen;		segen���� 0�� �ƴϸ� �̵�
	ldi segen, $08;			segen���� 0�� �Ǹ� �ٽ� 0x08�� �ʱ�ȭ
	clr segcnt;				������ �ڸ����� �ʱ�ȭ

Skip_resetsegen:			//segen�� 0���� ũ��	
	cpi stopgo, 0;			���� ���������̸�
	breq DONE;				DONE���� �̵�
	inc timecnt;			
	cpi timecnt, 78;		// Ÿ�̸� ���ͷ�Ʈ�� 78�� ������ ������ (1/100�ʰ� ������ ������) 
	brne DONE;				//  DONE���� �̵� , (�ƹ����� �Ͼ�� �ʴ´�.)

one:						// 1/100�ʰ� ������ (78�� ���ͷ�Ʈ �߻�) 4��° �������� ����
	clr timecnt;
	ldi YH, High(digit4);
	ldi YL, Low(digit4);
	ldd temp, Y+3;
	inc temp;				 1/100�ʿ� �ش��ϴ� �ε��� �ѹ� ����
	cpi temp, 10;				
	breq ten;				�ڸ��� ������ �Ͼ�� ten���� �̵�
	std Y+3, temp;			�ڸ��� ������ �Ͼ�� �ʾҴٸ� temp���� �ε��� �ѹ��� ����
	jmp DONE;

ten:						// 1/10�ʰ� ������ 3��° ������ ����
	clr temp;
	std Y+3, temp;			���� �ڸ����� �ε��� �ѹ��� 0���� �ʱ�ȭ
	ldd temp, Y+2;			�ڸ����� ������Ű��, �׿� �ش��ϴ� �ε��� �ѹ����� temp�� ����
	inc temp;
	cpi temp, 10;			���� �ڸ��� ������ �Ͼ�� hundred�� �̵�
	breq hundred;
	std Y+2, temp;			�ڸ��� ������ �Ͼ�� �ʾҴٸ� temp���� �ε��� �ѹ��� ����
	jmp DONE;

hundred:					//	1�ʰ� ������ 2��° ������ ����
	clr temp;				hundred, thousand �Լ��� ���� one, ten �Լ��� ������ �����ϴ�.
	std Y+2, temp;
	ldd temp, Y+1;
	inc temp;
	cpi temp, 10;
	breq thousand;
	std Y+1, temp;
	jmp DONE;

thousand:				// 10�ʰ� ������ 1��° ������ ����
	clr temp;
	std Y+1, temp;
	ld temp, Y;
	inc temp;
	cpi temp, 6;
	breq minute;
	st Y, temp;
	jmp DONE;

minute:					// 60�ʰ� ������ ��� 0�� �ȴ�
	clr temp;			
	st Y, temp;			ù��° �����ǿ� �ش��ϴ� �ε��� �ѹ� �ʱ�ȭ

DONE:					// �ڸ��� ������ �Ͼ�� ������ ����
	pop SREG2;
	out SREG, SREG2;		status register �� ���ÿ��� ������.
	sei;			���ͷ�Ʈ on
	ret;

EXT_INT4:				// �ܺ� ���ͷ�Ʈ 4 , 1�� ����ġ�� ���� ��� ����
	cli;			���ͷ�Ʈ disable

	ldi YH, High(digit4);
	ldi YL, Low(digit4);		y�������Ϳ� �ε��� �ѹ� �ּҰ� ����

	ldi temp, 200;
	call delay_Tms;				debouncing �� ����� �����Լ�

	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y+, temp;
	ldi temp, 0;
	st Y, temp;					�ε��� �ѹ������� ��� 0���� �ʱ�ȭ -> Ÿ�̸� 0000�ʱ�ȭ��

	ldi stopgo, 0;				Ÿ�̸� ����

	sei;			���ͷ�Ʈ on
	reti;			���������͸� �����Ͽ� �������α׷� ����

EXT_INT5:			// �ܺ� ���ͷ�Ʈ 5, 2�� ����ġ�� ���� ��� ����
	cli;			���ͷ�Ʈ disable

	ldi temp, 200;
	call delay_Tms;				debouncing �� ����� �����Լ�

	inc stopgo;			 stopgo ����
	cpi stopgo, 1;		 ����  stopgo�� 1�̸� Ÿ�̸� �簳
	breq Go;
	
	clr stopgo;			stopgo�� 1�� �ƴϸ� stopgo�� 0���� �ʱ�ȭ -> Ÿ�̸� ����
	sei;
	reti;
Go:
	sei;			���ͷ�Ʈ on
	reti;	

delay_1ms:				// �ð� ���� �Լ�
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

delay_Tms:				// �ð� ���� �Լ�
	call delay_1ms;
	dec temp;
	brne Delay_Tms;

	ret;

SegData:
	.db $3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

	.DSEG
digit4:
	.byte 4
