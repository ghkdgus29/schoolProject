	.CSEG

.def	temp = r16

start:

	ldi ZH, HIGH(2*ConstantData) ;
	ldi ZL, LOW(2*ConstantData) ;

	ldi XH, HIGH(MemoryBank1) ;
	ldi XL, LOW(MemoryBank1) ;

	ldi temp, 10;

Loop1:
	lpm r0, Z+ ;
	st X+, r0 ;
	dec temp ;
	brne Loop1;


	ldi XH, HIGH(MemoryBank1);
	ldi XL, LOW(MemoryBank1);

	ldi YH, HIGH(MemoryBank2);
	ldi YL, LOW(MemoryBank2);
	
	ldi temp, 10; 

Loop2:
	ld r0, X+;
	st Y+, r0;
	dec temp;
	brne Loop2;

	ldi XH, HIGH(MemoryBank3);
	ldi XL, LOW(MemoryBank3);

	ldi temp, 10;

Loop3:
	ld r0, -Y;
	st X+, r0;
	dec temp;
	brne Loop3;

	ldi XH, HIGH(MemoryBank4);
	ldi XL, LOW(MemoryBank4);

	ldi ZH, HIGH(2*Order);
	ldi ZL, LOW(2*Order);

	ldi temp, 10;

Loop4:
	ldi YH, HIGH(MemoryBank3);
	ldi YL, LOW(MemoryBank3);
	lpm r1, Z+;
	add r28, r1;
	ld r0, Y;
	st X+, r0;
	dec temp;
	brne Loop4;


forever:
	rjmp forever;

ConstantData:
	.db $03, $9f, $25, $0d, $99, $49, $41, $1f, $01, $09

Order:
	.db 9, 0, 8, 1, 7, 2, 6, 3, 5, 4

;--------------------------------------------------------------------------
	.DSEG
MemoryBank1:
	.byte 16
MemoryBank2:
	.byte 16
MemoryBank3:
	.byte 16
MemoryBank4:
	.byte 16

	.CSEG