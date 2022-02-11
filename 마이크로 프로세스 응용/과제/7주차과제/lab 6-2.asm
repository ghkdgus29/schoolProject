.include "m128def.inc"
	.ORG	0;
	rjmp	RESET;
	.ORG	$0020;
	rjmp	Timer0_OVF;				// ���ͷ�Ʈ �߻��� �̵�

.def	temp	=	r16				// �ӽú���
.def	leddata	=	r17				// led data ����
.def	intCnt	=	r18				// ī��Ʈ

	.ORG	$0046;
RESET:
	ldi	temp, low(RAMEND);			
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;					// ���������� ����

	cli;							// ���ͷ�Ʈ disable
	rcall Timer0_Init;				// Timer ���� �Լ� ȣ��
	sei;							// ���ͷ�Ʈ on

	ldi temp, $FF;
	out DDRA, temp;					// led�� 8�� ��� ����ϱ� ���� ����
	ldi leddata, $FF;				// led data�� 0xFF���� ����ִ´�.

forever:
	out PORTA, leddata;
	rjmp forever;					// led data���� �ݺ��ؼ� ����Ѵ�.

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
	inc intCnt;					
	cpi intCnt, 122;				// intCnt�� 244���� ������ �ƹ��ϵ� �Ͼ�� �ʴ´�.
	brne Skip_ResetCnt;
	clr intCnt;						// intCnt�� 0���� �ʱ�ȭ�ϰ�
	com leddata;					// leddata�� �����´�. (0���θ����)

Skip_ResetCnt:						// intCnt�� 244���� ���� ��Ȳ
	sei;							// ���ͷ�Ʈ on
	reti;
