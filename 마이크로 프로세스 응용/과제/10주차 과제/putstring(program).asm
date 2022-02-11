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
	rcall Uart_Init;			// ������� ���� �ʱ�ȭ

	ldi ZH, high(2*message);
	ldi ZL, low(2*message);		// Z�������Ϳ� message (string) �ּ� �Ҵ�
	rcall PutStringP;			// ���ڿ� ��� �Լ� 

forever:
	rjmp	forever;

message:
	.db		"This is a AVR Test Board",CR, LF, EOS;		// ���α׷� �޸𸮿� ����Ǿ� �ִ� ���ڿ�

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
	out	UDR0, r0;			// �����͸� ��� �����ϸ� r0 (AVR)���� �������͹̳η� ����
	ret;

PutStringP:
	lpm	r0, Z+;				// Z�ּҸ� �������Ѱ��� �����͸� r0�� ����
	tst r0;					// r0�� EOS �� ���ٸ� 
	breq exit;				// ������ �Է� ����
	rcall PutChar;			// �ٸ��ٸ� ������ �Է�(����)
	rjmp PutStringP;		// �ݺ�

exit:
	ret;