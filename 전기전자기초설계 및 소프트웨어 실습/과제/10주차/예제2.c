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
		while(((EIFR&0x10)|(EIFR&0x20))!=0x30){}		// 인터럽트 4번 5번 flag들어오기 전까지 대기

		if(((EIFR&0x10)|(EIFR&0x20))==0x30){			// 인터럽트 4번 5번 flag 들어올경우
			PORTF=0xC0;									
			_delay_ms(1000);
		}
		
		sei();				// 인터럽트 enable
		_delay_ms(100);
		if(EIFR==0){		// 인터럽트 플래그가 0이되면 (인터럽트 동작을 모두 수행하면)
			PORTF=0x00;		// 0으로 초기화
		}
		cli();				// 인터럽트 disable
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
