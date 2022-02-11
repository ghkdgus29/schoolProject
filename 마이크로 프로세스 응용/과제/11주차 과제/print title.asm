.include "m128def.inc"
.def	temp	=	r16
.equ	LF		=	$0A
.equ	CR		=	$0D
.equ	EOS		=	$00
.equ	TAP		=	$09

.CSEG
	.ORG 0x0046;

RESET:
	ldi	temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;				 ���ü���

	rcall Initialize;			UART ��� ����
	rcall PrintTitle;			���� ����Ʈ

forever:
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