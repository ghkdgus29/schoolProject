.include "m128def.inc"
	rjmp	RESET
.def	temp = r16						// �ӽú���
.def	segdata	= r17					// led data ���� ������ ��������
.def	segen = r18						// ��ƮG�� ���� �Է°��� ������ ��������
.def	segcnt = r19					// ī��Ʈ, 4�� �Է��Ͽ� 4���� LED�� ���ڸ� �����ư��� ����� ����
.equ	FCLK = 16000000					// delay�� ���� Ŭ��ī��Ʈ
	.ORG 0x046;
RESET:
	ldi	temp, low(RAMEND);
	out SPL, temp;
	ldi temp, high(RAMEND);
	out SPH, temp;						// ���������� ����

	ser temp;							
	out DDRC, temp;						// LED�� 7segment�� ��� ������� ����ϱ����� ��ƮC�� 0xFF���� �־���

	ldi temp, $0F;
	sts DDRG, temp;						// 4���� LED �������� �̿��ϱ� ���� ��ƮG�� 0x0F���� �־��ش�.

	ldi XH, High(digit4);				
	ldi XL, Low(digit4);				// ������ �޸��� digit4 �ּҸ� X���������� ����

	ldi temp, 1;
	st X+, temp;
	ldi temp, 2;
	st X+, temp;
	ldi temp, 3;
	st X+, temp;
	ldi temp, 4;
	st X, temp;							// digit4 �� 1, 2, 3, 4 ����, �迭�� ����

forever:
	ldi XH, High(digit4);
	ldi XL, Low(digit4);				// digit4 ù �ּҸ� X�������Ϳ� ����
	ldi segen, $08;						// ù��° �����ǿ��� ���̵������� segen�� ����
	ldi segcnt, 4;						// ī��Ʈ�� ���� 4 ����


loop4:
	ldi ZH, HIGH(2*Seg_Data);	
	ldi ZL, LOW(2*Seg_Data);			// Z�������Ϳ� LED segment data �迭 �ּ� ���� 

	ld temp, X+;						// temp�� X�ּҸ� �����ϸ� ������Ŵ
	clr r0;
	add ZL, temp;						// ZL�� temp������ ���Ѵ�. �� digit4�� 1,2,3,4 �� ��������.
	adc ZH, r0;							// ĳ���� �߻��� �Ͼ�� ���� ���� adc ����, r0 �� 0�̴�.

	lpm segdata, Z;						// �̷��� ������ Z�ּ��� segment data ���� segdata�� ����
	sts PORTG, segen;					// ��ƮG�� segen �� �ش��ϴ� �����Ǹ� �������� ����
	out PORTC, segdata;					// segdata�� ����� led data ���� ��Ʈ C�� ���� ���

	ldi temp, 1;						// temp�� ���ڸ� ����, �� ���ڴ� ������ų ms�� ũ�⸦ �ǹ��Ѵ�.
	rcall Delay_Tms;					// �����Լ� ȣ��
	lsr segen;							// segen �� ���� ����Ʈ ���� ������ �����ǿ� ���� �������� ����

	dec segcnt;							// ī��Ʈ ����
	brne loop4;							// ī��Ʈ�� 0�̵Ǳ� ���� loop4�� ����ؼ� ����.	

	rjmp forever;						// ���� ī��Ʈ�� 0�� �Ǹ� forever�� ���ư� �ݺ��Ѵ�.

Delay_1ms:								// 1ms �� �ð������Լ�
	push YL;
	push YH;
	ldi YL, low(((FCLK/1000)-18)/4);
	ldi YH, high(((FCLK/1000)-18)/4);

delay1ms_01:
	sbiw YH:YL, 1;
	brne delay1ms_01;

	pop YH;
	pop YL;
	ret;

Delay_Tms:								// Tms�� �ð������Լ�, temp�� T�� �޴´�.
	call Delay_1ms;
	dec temp;
	brne Delay_Tms;

	ret;

Seg_Data:								// LED data ���� ����Ǿ� �ִ� ���α׷�  �޸� �迭�̴�.
	.db	$3f, $06, $5b, $4f, $66, $6d, $7d, $07, $7f, $6f

	.DSEG
digit4:									// �ε����� ���ڵ��� ����ִ� ������ �޸� �����̴�.
	.byte 4
		

