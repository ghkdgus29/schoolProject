.include "m128def.inc"
.def	temp	=	r16
.def	leddata	=	r17
.equ	LF		=	$0A				// �ٹٲ�
.equ	CR		=	$0D				// Ŀ�� �ʱ�ȭ
.equ	EOS		=	$00				// ���ڿ� ��
.equ	TAP		=	$09				// ��

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
	rcall PrintCmdCursor;		Ŀ�� ����Ʈ
	rcall ReadCmd;				Ŀ���� �б�

	rcall CmdInterprete;		Ŀ���� �ؼ��Ͽ� �ش��ϴ� �����ϱ�
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

cmdcursor:
	.db TAP,"Input Function Number:  ",EOS;						// Ŀ�� ���ڿ�, constant

PrintCmdCursor:												// Ŀ���� ����ϴ� �Լ�
	CALL NewLine;					 ���� ����
	ldi ZH, high(2*cmdcursor);
	ldi ZL, low(2*cmdcursor);		Z�� Ŀ�����ڿ� �ּ� ����
	call PutStringP;				���ڿ� ����ϱ�
	ret;

.DSEG

buffer:	
	.byte 10

.CSEG
ReadCmd:
	ldi YH, high(buffer);
	ldi YL, low(buffer);
	rcall GetString;
	ret;

GetChar:
	sbis UCSR0A, RXC0;			���� �����͸� ���޹޴� ���̸� ����ؼ� Getchar ���� �ݺ�
	rjmp GetChar;
	in r0, UDR0;				�����͸� ��� �Է¹����� UDR0���� AVR r0 �������Ϳ� ����
	ret;

GetString:						//���ڿ� �޴� �Լ�
	rcall GetChar;		
	call PutChar;				Echoing
	st Y+, r0;					Y�ּҿ� �ش��ϴ� �޸� ������ ������ �͹̳ηκ��� �Է¹��� ���� ����
	ldi temp, CR;				temp�� ����Ű�� �ش��ϴ� �ƽ�Ű�� ����
	cp r0, temp;				����Ű�� ������ ���ڿ� �Է¹ޱⰡ ������.
	brne GetString;				���� ����Ű�� ������ �ʾҴٸ� ����ؼ� ���ڿ��� �Է¹޴´�.
	clr r0;						r0�� EOS�� ����
string:
	st Y, r0;					���ڿ� �������� EOS ����
	ret;

CmdInterprete:					// Ŀ���� �ؼ� �Լ�
	ldi XH, high(buffer);
	ldi XL, low(buffer);		�����ּҸ� X�������Ϳ� ����
	ld temp, X;					���ۿ��� �ҷ����� �� temp�� ����

	cpi temp, '1';				���� ���ۿ� �ִ� ���� 1�̸�
	breq CMD_LED;				LED test

	cpi temp, '2';				���ۿ� �ִ� ���� 2�̸�
	breq CMD_Segment;			7 segment test
	
	cpi temp, '3';				���ۿ� �ִ� ���� 3�̸�
	breq CMD_Buzzer;			buzzer test

	cpi temp, '4';				���ۿ� �ִ� ���� 4�̸�
	breq CMD_PhotoSensor;		Photo sensor test

	cpi temp, '5';				���ۿ� �ִ� ���� 5�̸�
	breq CMD_TempSensor;		Temp sensor test

	rjmp CMD_Wrong;				���ۿ� �ִ� ���� 1, 2, 3, 4, 5 �� �ƴϸ� Wrong command ���

CMD_Done:
	ret;

CMD_LED:						// LED Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*led);
	ldi ZL, low(2*led);
	call PutStringP;
	rjmp CMD_Done;

CMD_Segment:					// 7segment Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*seg);
	ldi ZL, low(2*seg);
	call PutStringP;
	rjmp CMD_Done;

CMD_Buzzer:						// Buzzer Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*buz);
	ldi ZL, low(2*buz);
	call PutStringP;
	rjmp CMD_Done;

CMD_PhotoSensor:				// PhotoSensor Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*pho);
	ldi ZL, low(2*pho);
	call PutStringP;
	rjmp CMD_Done;

CMD_TempSensor:					// TempSensor Test ������ ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*tempS);
	ldi ZL, low(2*tempS);
	call PutStringP;
	rjmp CMD_Done;

CMD_Wrong:						// wrong comman�� �Էµ��� ����ϴ� ����
	call NewLine;
	ldi ZH, high(2*wrong);
	ldi ZL, low(2*wrong);
	call PutStringP;
	rjmp CMD_Done;		

led:
	.db TAP,"---> Here is LED Test Routine ",LF,CR,EOS;

seg:
	.db TAP,"---> Here is 7segment Test Routine",LF,CR,EOS;

buz:
	.db TAP,"---> Here is Buzzer Test Routine",LF,CR,EOS;

tempS:
	.db TAP,"---> Here is TempSensor Test Routine",LF,CR,EOS;

pho:
	.db TAP,"---> Here is PhotoSensor Test Routine ",LF,CR,EOS;

wrong:
	.db TAP,"---> Wrong command... ",LF,CR,EOS;
	