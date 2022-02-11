.include "m128def.inc"
.def temp	=	r20
.equ	LF	=	$0A				// �ٹٲ�
.equ	CR	=	$0D				// Ŀ�� ó������ �ű��
.equ	EOS	=	$00				// ���ڿ� ������ ǥ��

.CSEG
		.ORG 0x0000;
		JMP RESET;

		.ORG 0x0046;
RESET:
	ldi r16, low(RAMEND);
	out	SPL, r16;
	ldi	r16, high(RAMEND);
	out SPH, r16;
	rcall Uart_Init;			// UART ��� ���� �ʱ�ȭ

forever:
	ldi YH, HIGH(rdata);
	ldi YL, LOW(rdata);			// rdata �޸� ������ �ּҸ� Y�� ����
	rcall GetString;			// ������ �͹̳��� �Է°��� AVR�� ����
	rcall NewLine;				// �ٹٲ�

	ldi ZH, HIGH(rdata);
	ldi ZL, LOW(rdata);			// rdata �޸� ������ �ּҸ� Z�� ����
	rcall PutStringD;			// AVR�� ����� ������ ������ �͹̳ο� ���
	rcall NewLine;				// �ٹٲ�

	rjmp forever;

Uart_Init:
	ldi temp, $00;
	sts	UBRR0H, temp;		// U2X=0

	ldi temp, $67;
	out UBRR0L, temp;		// UBRR=103, Baud rate(bps) = 9600

	ldi temp, $18;
	out UCSR0B, temp;		// TX, RX enable without interrupt

	ldi temp, $06;
	sts UCSR0C, temp;		//	8bit data, 1 stop bit, No partiy

	ret;

PutChar:
	sbis UCSR0A, UDRE0;		// ���� �����͸� �Է¹޴� ���̸� ����ؼ� PutChar ������ �ݺ�
	rjmp PutChar;
	out	UDR0, r0;			// �����͸� ��� �Է¹����� r0�� AVR�� ����
	ret;

GetChar:					// ���� �Է� �Լ�
	sbis UCSR0A, RXC0;		// ���� �����͸� ���޹޴� ���̸� ����ؼ� GetChar ���� �ݺ�
	rjmp GetChar;
	in r0, UDR0;			// �����͸� ��� �Է¹����� UDR0���� (�ٸ� ��⿡���� �Է°�) AVR r0�������Ϳ� ����
	ret;

PutStringD:
	ld	r0, Z+;				// Z�ּҸ� �������Ѱ��� �����͸� r0�� ����
	tst r0;					// r0�� EOS �� ���ٸ� 
	breq exit;				// ������ �Է� ����
	rcall PutChar;			// �ٸ��ٸ� ������ �Է�(����)
	rjmp PutStringD;		// �ݺ�

exit:
	ret;

GetString:
	rcall GetChar;
	rcall PutChar;			// EHCOing
	st Y+, r0;				// Y�ּҿ� �ش��ϴ� �޸𸮰����� �������͹̳η� ���� �Է¹��� ���� ����
	ldi temp, CR;			// temp�� ����Ű�� �ش��ϴ� �ƽ�Ű������	
	cp r0, temp;			// ����Ű�� ������ ���ڿ� �Է¹ޱⰡ ������
	brne GetString;			// ���� ����Ű�� ������ �ʾҴٸ� ����ؼ� ���ڿ��� �Է¹޴´�
	clr r0;					// r0�� EOS���� ����

string:
	st Y, r0;				// ���ڿ� �������� EOS����
	ret;

NewLine:
	ldi temp, LF;
	mov r0, temp;
	call PutChar;			// �ٹٲ��� avr�� ���� �������͹̳ο� ���
	ldi temp, CR;
	mov r0, temp;
	call PutChar;			// Ŀ���� ó������ �ű� (avr�� ���� �������͹̳ο� ���)
	ret;

.DSEG

rdata:
	.byte	64				// ������ ���� ���� (���ڿ�)