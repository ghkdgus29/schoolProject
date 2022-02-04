#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void init_port(void){
	DDRF = 0xF0;
	PORTF = 0x00;
	DDRE = 0x00;
	PORTE = 0xFF;
}

void init_interrupt(void){
	
	EIMSK = 0x00;
	EICRA = 0x00;
	EICRB = 0x0A;
	EIMSK = 0x30;
	EIFR = 0xFF;
}

int main(void){
	init_port();
	init_interrupt();
	
	while(1){
		while(((EIFR&0x10)|(EIFR&0x20))!=0x30){}		// ���ͷ�Ʈ 4�� 5�� flag������ ������ ���

		if(((EIFR&0x10)|(EIFR&0x20))==0x30){			// ���ͷ�Ʈ 4�� 5�� flag ���ð��
			PORTF=0xC0;									
			_delay_ms(1000);
		}
		
		sei();				// ���ͷ�Ʈ enable
		_delay_ms(100);
		if(EIFR==0){		// ���ͷ�Ʈ �÷��װ� 0�̵Ǹ� (���ͷ�Ʈ ������ ��� �����ϸ�)
			PORTF=0x00;		// 0���� �ʱ�ȭ
		}
		cli();				// ���ͷ�Ʈ disable
	}
	return 0;
}

ISR(INT4_vect){
	PORTF = ~PORTF;
	_delay_ms(1000);
}

ISR(INT5_vect){
	PORTF = 0x90;
	_delay_ms(1000);
}
