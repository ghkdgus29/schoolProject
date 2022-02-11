.include "m128def.inc"
	.ORG	0;
	rjmp	RESET;
	.ORG	$0020;
	rjmp	Timer0_OVF;				// ���ͷ�Ʈ �߻��� �̵�

.def	temp	=	r16				// �ӽú���
.def	leddata	=	r17				// led data ����
.def	timecnt	=	r18				// ī��Ʈ
.def	segen	=	r19
.def	segcnt	=	r20

	.ORG	$0046;
RESET:
	ldi	temp, low(RAMEND);			
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					// ���������� ����

	ser temp;
	out DDRC, temp;					// 8���� segment�� ��� ����ϱ� ���� ����

	ldi temp, $0F;
	sts DDRG, temp;					// 4���� �������� ��� ����ϱ� ���� ����

	cli;							// ���ͷ�Ʈ disable
	rcall Timer0_Init;				// Timer ���� �Լ� ȣ��
	sei;							// ���ͷ�Ʈ on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);			// X�������Ϳ� digit4 �ּҸ� ����

	ldi temp, 5;
	st X+, temp;
	ldi temp, 9;
	st X+, temp;
	ldi temp, 5;
	st X+, temp;
	ldi temp, 0;
	st X, temp;						// led data�� �����ϱ� ���� index�� digit4�� �����Ѵ�.

	ldi segen, $08;					// ù��° ������ ����
	ldi timecnt, 0;					// timecnt ���� 0���� �ʱ�ȭ

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);

	clr r0;	
	add XL, segcnt;					// segcnt(�ε��� �ѹ� �ڸ���)���� X�������Ϳ� ���Ѵ�. �� ��, 10��, 1��, 10���� �ǹ��ϴ� �������� �����ϱ� �����̴�.
	adc XH, r0;

	ldi ZH, HIGH(2*SegData);
	ldi	ZL, LOW(2*SegData);			// Z�������Ϳ� led data ���� ����Ǿ��ִ� SegData�� �ּ� ����

	ld temp, X;						// temp�� X�������� �ּҿ� �ִ� �ε��� �ѹ� ���� (digit4)
	clr r0;
	add ZL, temp;					
	adc	ZH, r0;						// Z�������� �ּҿ� digit4�� �ִ� �ε��� �ѹ��� ���Ѵ�.

	lpm leddata, Z;					// �̷��� �ε��� �ѹ��� ���� ���� leddata �������Ϳ� ����
	sts PORTG, segen;				// segen ���� �ش��ϴ� ������ ����
	out	PORTC, leddata;				// leddata���� �ش��ϴ� ���� ����Ѵ�. 

	rjmp forever;					// �ݺ�

Timer0_Init:						// Ÿ�̸� ���� �Լ�
	ldi temp, $06;
	out TCCR0, temp;				// 16MHZ/256���� Timer clock�� �����ϸ�

	ldi temp, $00;
	out TCNT0, temp;				// ���ͷ�Ʈ�� �߻��ϰ��� Ÿ�̸Ӹ� �ٽ� 0���� �ʱ�ȭ�ϴ� ����

	ldi temp, 01;
	out TIMSK, temp;				// �����÷ο� �߻��� ���ͷ�Ʈ�� �߻��ϵ��� ����
	ret;

Timer0_OVF:
	cli;							// ���ͷ�Ʈ disable
	in r1, SREG;
	push r1;						// status register �� ���ÿ� ����
					
	lsr segen;						// segen �� �������� �������Ѵ�. �����Ϸ��� ������ �̵�
	inc segcnt;						// segcnt���� ������Ų��. 
	cpi segen, $00;					
	brne Skip_ResetSegEn;			// segen ���� 0�� �ƴϸ� �̵��Ѵ�.
	ldi segen, $08;					// segen ���� 0�� �Ǹ� �ٽ� 0x08�� �ʱ�ȭ ���ش�.
	clr segcnt;						// segcnt���� �ʱ�ȭ��Ų��.
					
Skip_ResetSegEn:					// segen�� 0���� ū ��Ȳ
	inc timecnt;					
	cpi timecnt, 244;				// timecnt ���ͷ�Ʈ�� �߻��� �� ���� ������Ų��. ���� 1��(=244�� ���ͷ�Ʈ �߻�)�� ������ ����
	brne DONE;						// DONE���� �̵��Ͽ� �ƹ��� ���۵� �������� �ʴ´�.

second:								// 4��° ������ ���� �����Ѵ�. (��)
	clr timecnt;					// ���� 1�ʰ� ������ timecnt�� �ʱ�ȭ�Ѵ�
	ldi YH, High(digit4);			
	ldi YL, Low(digit4);			// Y�������Ϳ� digit4�ּҸ� ����
	ldd temp, Y+3;					// digit4�� 4��° �ε��� �ѹ�, �� 4��° ������(��)�� �����͸� temp�� �����Ѵ�.
	inc temp;						// temp�� ������Ų��.
	cpi temp, 10;					// temp�� 10, �� 9�ʰ� ������ �Ǹ� 
	breq ten_seconds;				// 10�ʷ� �̵��Ѵ�.
	std Y+3, temp;					// temp�� 9�ʸ� ������ �ʾҴٸ� temp���� digit4�� 4��°�� �����Ѵ�.
	jmp DONE;						

ten_seconds:						// 3��° ������ ���� �����Ѵ�. (10��)
	clr temp;						// temp���� 0���� �ʱ�ȭ
	std Y+3, temp;					// (��)�ڸ����� 9�ʰ� ������ �ٽ� 0���� �ٲ۴�. 
	ldd temp, Y+2;					// digit4�� 3��° �ε��� �ѹ�, �� 3��° ������(10��)�� �����͸� temp�� �����Ѵ�.
	inc temp;						// temp�� ������Ų��.
	cpi temp, 6;					// temp�� 6, �� 60�ʸ� ������ �Ǹ� 
	breq minute;					// 1������ �̵��Ѵ�.
	std Y+2, temp;					// temp�� 60�ʸ� ������ �ʾҴٸ� temp���� digit4�� 3��°�� �����Ѵ�.
	jmp DONE;

minute:								// 2��° ������ ���� �����Ѵ�. (1��)
	clr temp;						// temp���� 0���� �ʱ�ȭ
	std Y+2, temp;					// (10��)�ڸ����� 60�ʰ� ������ �ٽ� 0���� �ٲ۴�.
	ldd temp, Y+1;					// digit4�� 2��° �ε��� �ѹ�, �� 2��° ������(1��)�� �����͸� temp�� �����Ѵ�.
	inc temp;						// temp�� ������Ų��.
	cpi temp, 10;					// temp�� 10, �� 9���� ������ �Ǹ� 
	breq ten_minutes;				// 10������ �̵��Ѵ�.
	std Y+1, temp;					// temp�� 9���� ������ �ʾҴٸ� temp���� digit4�� 2��°�� �����Ѵ�.
	jmp DONE;

ten_minutes:						// 1��° ������ ���� �����Ѵ�. (10��)
	clr temp;						// temp���� 0���� �ʱ�ȭ
	std Y+1, temp;					// (1��)�ڸ����� 9���� ������ �ٽ� 0���� �ٲ۴�.
	ld temp, Y;						// digit4�� 1��° �ε��� �ѹ�, �� 1��° ������(10��)�� �����͸� temp�� �����Ѵ�.
	inc temp;						// temp�� ������Ų��.
	cpi temp, 6;					// temp�� 6, �� 60���� ������ �Ǹ� 
	breq  hour;						// 1�ð����� �̵��Ѵ�.
	st Y, temp;						// temp�� 60���� ������ �ʾҴٸ� temp���� digit4�� 1��°�� �����Ѵ�.
	jmp DONE;

hour:
	clr temp;						// temp���� 0���� �ʱ�ȭ
	st Y, temp;						// (10��)�ڸ����� 60���� ������ 0���� �ٲ۴�.

DONE:
	pop r1;
	out SREG, r1;						// status register �� ���ÿ��� ������.
	sei;								// ���ͷ�Ʈ on
	ret;
	

SegData:								// LED data ���� ����Ǿ� �ִ� ���α׷�  �޸� �迭�̴�.
	.db	$3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

	.DSEG
digit4:									// �ε����� ���ڵ��� ����ִ� ������ �޸� �����̴�.
	.byte 4
		
