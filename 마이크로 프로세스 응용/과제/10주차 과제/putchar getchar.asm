.include "m128def.inc"
.def temp	=	r20;

		.ORG 0x0000;
		JMP RESET;

		.ORG 0x0046;
RESET:
	ldi r16, low(RAMEND);
	out	SPL, r16;
	ldi	r16, high(RAMEND);
	out SPH, r16;
	rcall Uart_Init;

forever:
	rcall GetChar;			// GetChar �Լ��� ���� �������͹̳ο����� �Է°��� AVR r0�������Ϳ� ����
	rcall PutChar;			// PutChar �Լ��� ���� AVR r0���������� ����� ���� �������͹̳ο� ���
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
	sbis UCSR0A, UDRE0;		// ���� �����͸� �����ϴ� ���̸� ����ؼ� PutChar ������ �ݺ�
	rjmp PutChar;
	out	UDR0, r0;			//�����͸� ��� �����ϸ� r0 (AVR)���� �������͹̳η� ����
	ret;

GetChar:					// ���� �Է� �Լ�
	sbis UCSR0A, RXC0;		// ���� �����͸� ���޹޴� ���̸� ����ؼ� GetChar ���� �ݺ�
	rjmp GetChar;
	in r0, UDR0;			// �����͸� ��� �Է¹����� UDR0���� (�ٸ� ��⿡���� �Է°�) AVR r0�������Ϳ� ����
	ret;
