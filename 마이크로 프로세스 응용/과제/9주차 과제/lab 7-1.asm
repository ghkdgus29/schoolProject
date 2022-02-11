.include "m128def.inc"

.def	temp	=	r16
.def	leddata	=	r17
.equ	FCLK	=	16000000

.CSEG
	.ORG 0x000;
	jmp	RESET;

	.ORG 0x000A;
	jmp EXT_INT4;		�ܺ� ���ͷ�Ʈ 4���� �Էµ� ���
	jmp EXT_INT5;		�ܺ� ���ͷ�Ʈ 5���� �Էµ� ���

	.ORG 0x0046;

; main program

RESET:
	ldi temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;				���������� ����

	cli;				���ͷ�Ʈ disable
	
	ldi temp, $00;
	sts EICRA, temp;			

	ldi temp, $0A;
	sts EICRB, temp;			�ܺ� ���ͷ�Ʈ 4�� 5���� falling edge ���ͷ�Ʈ�� ���

	ldi temp, $30;
	out EIMSK, temp;			�ܺ� ���ͷ�Ʈ 4�� 5���� ��밡���ϵ��� ����	
	
	sei;				���ͷ�Ʈ on
	
	ldi temp, $FF;
	out DDRA, temp;				A��Ʈ ��� ������� ���
	
	ldi leddata, $FF;
	out PORTA, leddata;			A��Ʈ led�� ��� ����
	
forever:
	rjmp forever;			

EXT_INT4:					// �ܺ� ���ͷ�Ʈ 4��, 1�� ����ġ�� ���� ��� ����
	 cli;				���ͷ�Ʈ disable

	 ldi temp, $FF;
	 out PORTA, temp;		  A��Ʈ led ��� ����

	 ldi temp, 200;
	 call delay_Tms;		 debouncing�� ���� �����Լ�

	 sei;				���ͷ�Ʈ on
	 reti;				���������͸� �����Ͽ� ���� ���α׷� ����

EXT_INT5:					// �ܺ� ���ͷ�Ʈ 5��, 2�� ����ġ�� ���� ��� ����
	cli;				���ͷ�Ʈ disable

	ldi temp, $00;
	out	PORTA, temp;		A��Ʈ led�� ��� �ҵ�

	ldi temp, 200;
	call delay_Tms;			debouncing�� ���� �����Լ�

	sei;				���ͷ�Ʈ on
	reti;				���������͸� �����Ͽ� ���� ���α׷� ����
	
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

