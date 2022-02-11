.include "m128def.inc"
.def	temp	=	r16
.def	leddata	=	r17
.def	segen	=	r18
.def	timecnt	=	r19
.def	SREG2	=	r20			// status �������͸� ������ ��������
.def	segcnt	=	r21
.def	cnt		=	r23
.equ	FCLK	=	16000000
.equ	LF		=	$0A
.equ	CR		=	$0D
.equ	EOS		=	$00

.CSEG

		.ORG 0;
		rjmp RESET;
		.ORG 0x000A;
		jmp EXT_INT4;		�ܺ� ���ͷ�Ʈ 4��, 1�� ����ġ�� ���� ��� ����
		jmp EXT_INT5;		�ܺ� ���ͷ�Ʈ 5�� , 2�� ����ġ�� ���� ��� ����
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

	call Uart_Init;			UART ��� ���� �ʱ�ȭ
	cli;			���ͷ�Ʈ disable
	rcall Timer0_Init;		Ÿ�̸� ���ͷ�Ʈ �ʱ�ȭ ����
	rcall EXINT_Init;		�ܺ� ���ͷ�Ʈ �ʱ�ȭ ����
	sei;			���ͷ�Ʈ on

	ldi XH, High(digit4);
	ldi XL, Low(digit4);	x �������Ϳ� digit4 �ּҸ� ���� (�ε���)

	ldi temp, 5;
	st X+, temp;
	ldi temp, 9;
	st X+, temp;
	ldi temp, 5;
	st X+, temp;
	ldi temp, 0;
	st X, temp;				�ε����� ���ڸ� ���ʷ� �����Ѵ� (digit4�� ����)

	ldi segen, $08;
	ldi timecnt, 0;

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
	ldi temp, $06;
	out TCCR0, temp;		16MHZ/256���� timer clock �����ϸ�

	ldi temp, $00;
	out TCNT0, temp;		���ͷ�Ʈ�� �߻��ϰ� ���� Ÿ�̸Ӹ� �ٽ� 0���� �ʱ�ȭ

	ldi temp, 01;
	out TIMSK, temp;		�����÷ο� �߻��� ���ͷ�Ʈ �߻�
	ret;

EXINT_Init:					// �ܺ� ���ͷ�Ʈ �ʱ�ȭ ����
	ldi temp, $00;
	sts EICRA, temp;
	
	ldi temp, $0A;
	sts EICRB, temp;        �ܺ� ���ͷ�Ʈ 4�� 5���� falling edge ���ͷ�Ʈ�� ���

	ldi temp, $30;
	out EIMSK, temp;		�ܺ� ���ͷ�Ʈ 4�� 5���� ��밡���ϵ��� ����
	ret;

Uart_Init:					// UART ��� ���� �ʱ�ȭ
	ldi temp, $00;
	sts UBRR0H, temp;		// U2X = 0

	ldi temp, $67;
	out UBRR0L, temp;		// UBRR = 103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;		// TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;		// 8bit data, 1 stop bit, No parity

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
	inc timecnt;			
	cpi timecnt, 244;		// Ÿ�̸� ���ͷ�Ʈ�� 244�� ������ ������ (1�ʰ� ������ ������) 
	brne DONE;				//  DONE���� �̵� , (�ƹ����� �Ͼ�� �ʴ´�.)

second:						// 1�ʰ� ������ (244�� ���ͷ�Ʈ �߻�) 4��° �������� ����
	clr timecnt;
	ldi YH, High(digit4);
	ldi YL, Low(digit4);
	ldd temp, Y+3;
	inc temp;				 �ʿ� �ش��ϴ� �ε��� �ѹ� ����
	cpi temp, 10;				
	breq ten_seconds;		�ڸ��� ������ �Ͼ�� ten_seconds�� �̵�
	std Y+3, temp;			�ڸ��� ������ �Ͼ�� �ʾҴٸ� temp���� �ε��� �ѹ��� ����
	jmp DONE;

ten_seconds:				// 10�ʰ� ������ 3��° ������ ����
	clr temp;
	std Y+3, temp;			���� �ڸ����� �ε��� �ѹ��� 0���� �ʱ�ȭ
	ldd temp, Y+2;			�ڸ����� ������Ű��, �׿� �ش��ϴ� �ε��� �ѹ����� temp�� ����
	inc temp;
	cpi temp, 6;			���� �ڸ��� ������ �Ͼ�� minute���� �̵�
	breq minute;
	std Y+2, temp;			�ڸ��� ������ �Ͼ�� �ʾҴٸ� temp���� �ε��� �ѹ��� ����
	jmp DONE;

minute:						//60�ʰ� ������ 2��° ������ ����
	clr temp;				minute, ten_minute �Լ��� ���� second, ten_seconds�Լ��� ������ �����ϴ�.
	std Y+2, temp;
	ldd temp, Y+1;
	inc temp;
	cpi temp, 10;
	breq ten_minutes;
	std Y+1, temp;
	jmp DONE;

ten_minutes:				// 10���� ������ 1��° ������ ����
	clr temp;
	std Y+1, temp;
	ld temp, Y;
	inc temp;
	cpi temp, 6;
	breq hour;
	st Y, temp;
	jmp DONE;

hour:					// 60���� ������ ��� 0�� �ȴ�
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

	sei;			���ͷ�Ʈ on
	reti;			���������͸� �����Ͽ� �������α׷� ����

EXT_INT5:			// �ܺ� ���ͷ�Ʈ 5, 2�� ����ġ�� ���� ��� ����
	cli;				���ͷ�Ʈ disable
	in temp, SREG;		status register ����
	push temp;
	push YH;
	push YL;
	push ZH;
	push ZL;			�������Ͱ� ���̴� �� ���� ���� �������͵� push


	ldi ZH, High(2*message);
	ldi ZL, Low(2*message);				message �迭�� �ּ� Z�� ����
	ldi YH, High(buffer);
	ldi YL, Low(buffer);				buffer �޸� ���� �ּ� Y�� ����

	rcall NewLine;					�� �ٲ�
	call PutStringP;				message �迭�� ����ִ� ���ڿ� ���
	call GetString;					������ �͹̳η� ���� �ð��� �Է¹޾� buffer�� ����

	ldi cnt, 4;						ī��Ʈ 4, 4�� ������ ���� �����̴�

	ldi YH, HIGH(buffer);
	ldi YL, LOW(buffer);			buffer �޸� ���� �ּ� Y ����
	ldi ZH, HIGH(digit4);
	ldi ZL, LOW(digit4);			digit4 �޸� ���� �ּ� Z����

again:
	ld temp, Y+;					temp�� buffer�� ����
	subi temp, $30;					temp�� ����ִ� ���� �ƽ�Ű���̹Ƿ� '0'�� �ش��ϴ� �ƽ�Ű�ڵ尪�� ���ش�
	st Z+, temp;					temp�� ����ִ� ���� digit4�� ����
	dec cnt;						��� digit4�� �������� �ݺ� ����
	brne again;

	pop ZL;
	pop ZH;
	pop YL;
	pop YH;
	pop temp;
	out SREG, temp;					status register, push �� register���� ��� ������
	sei;					���ͷ�Ʈ on
	reti;


PutChar:					// ���� ����, ��� �Լ�
	sbis UCSR0A, UDRE0;
	rjmp PutChar;
	out UDR0, r0;
	ret;

GetChar:					// ���ڸ� �޴�, �Է� �Լ�
	sbis UCSR0A, RXC0;
	rjmp GetChar;
	in r0, UDR0;
	ret;

PutStringP:					// ���α׷� �޸� ������ ����Ǿ� �ִ� ���� ������ �͹̳η� ����Ѵ�.
	lpm r0, Z+;
	tst r0;
	breq exit1;
	rcall PutChar;
	rjmp PutStringP;
exit1:
	ret;

PutStringD:					// �޸𸮰����� ����Ǿ� �ִ� ���� �������͹̳η� ����Ѵ�
	ld r0, Z+;
	tst r0;
	breq exit2;
	rcall PutChar;
	rjmp PutStringD;
exit2:
	ret;

GetString:					//������ �͹̳ηκ��� ���ڿ��� �Է¹޾� Y�������Ϳ� �ش��ϴ� �ּҿ� �����Ѵ�.
	rcall GetChar;
	rcall PutChar;
	st Y+, r0;
	ldi temp, CR;
	cp r0, temp;
	brne GetString;
	clr r0;
string:
	st Y, r0;
	ret;

NewLine:					// �ٹٲ�
	ldi temp, LF;
	mov r0, temp;
	call PutChar;
	ldi temp, CR;
	mov r0, temp;
	call PutChar;
	ret;

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

message:
	.db "Write setting time in MMSS format:", EOS;

	.DSEG
digit4:
	.byte 4

buffer:
	.byte 5

