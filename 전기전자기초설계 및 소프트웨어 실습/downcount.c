#include <avr/io.h>
#include <avr/interrupt.h>					// 인터럽트를 사용하기 위한 헤더선언
#include <util/delay.h>						// 딜레이 함수를 사용하기 위한 헤더선언

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};	
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};		
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												
int index[4] = {5, 0, 0, 5};				

int temp = 0;
int t=7812;
int stopgo = 0;

void init_port(){
	DDRA = 0xFF;
	DDRE = 0x0E;
	PORTE = 0x04;
	PORTE = 0x0F;
}

void timer_setting(){
	TCCR0 = _BV(CS01);
	TCNT0 = 0x00;
	TIMSK = 0x01;
	TIFR = 0xFF;
}

void init_uart(void){
	UBRR0H = 0;
	UBRR0L = 103;

	UCSR0B = 0x18;

	UCSR0C = 0x06;
}

void display_screen(){
	int i;
	for(i=0; i<4; i++){
		while((UCSR0A & 0x20) == 0x00);
		UDR0 = index[i]+48;
		if(i==3){
			while((UCSR0A & 0x20)== 0x00);
			UDR0 = '\r';

			while((UCSR0A & 0x20)== 0x00);
			UDR0 = '\n';
		}
	}
}

void display(){
	int j;
	for(j=0; j<4; j++){
		PORTE = 0x04;
		PORTA = FND_SEGPOS[j];

		PORTE = 0x08;
		if(j==1) PORTA = FND_SEGWP[index[j]];
		else PORTA = FND_SEGNP[index[j]];
		PORTE = 0x00;
		_delay_ms(1);
	}
}

ISR(TIMER0_OVF_vect){
	if(stopgo==1){
		temp++;
		if(temp == t){
			temp=0;
			index[3]--;
			if((index[3]<0)&(index[2]>0)){		// 만약 1의자리가 음수고 10의자리가 0이상이면 
				index[2]--;						// 10의 자리수를 1 감소시키고 
				index[3] = 9;					// 1의 자리수를 9로 변경
			}
			else if((index[3]<0)&(index[1]>0)&(index[2]==0)){	// 만약 1의자리가 음수, 10의자리가 0, 100의 자리가 0이상이면
				index[1]--;						// 100의 자리수를 1감소시키고
				index[2]=5;						// 10의 자리수는 9로 변경
				index[3]=9;						// 1의 자리수는 9로 변경
			}
			else if((index[3]<0)&(index[0]>0)&(index[1]==0)&(index[2]==0)){		// 만약 1의 자리수가 음수, 10, 100의 자리가 0, 1000의 자리가 0 이상이면
				index[0]--;						// 1000 자리수 1감소
				index[1]=9;						// 100 자리수 9로 변경
				index[2]=5;						// 10 자리수 9로 변경
				index[3]=9;						// 1 자리수 9로 변경
			}
			else if((index[3]<0)&(index[0]==0)&(index[1]==0)&(index[2]==0))	// 만약 1의 자리수가 음수, 나머지 자리수들이 모두 0이면	
				index[3] = 0;						// 1의자리수는 더 이상 감소시키지 않고 0 유지	
		}	
	}
}

int main(void){
	timer_setting();
	init_port();
	init_uart();
	int j;

	sei();
	while(1){
		display();
		switch(PING & 0x0F){
			case 0x0E:					// 5번 스위치 start
				stopgo = 1;
				break;

			case 0x0D:					// 6번 스위치 stop, display
				_delay_ms(150);
				stopgo = 0;
				display_screen();
				break;

			case 0x0B:					// 7번 스위치 reset, display
				_delay_ms(150);
				index[0]=index[2]=5;
				index[1]=index[3]=9;
				display_screen();
				break;
				
		}
	}

}

	
