#include <avr/io.h>
#include <avr/interrupt.h>					// ���ͷ�Ʈ�� ����ϱ� ���� �������
#include <util/delay.h>						// ������ �Լ��� ����ϱ� ���� �������

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};	
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};		
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												
int index[4] = {0, 0, 0, 0};
char buffer[5];				

int temp = 0;
int t=7812;
int stopgo = 0;

void init_port(){
	DDRA = 0xFF;
	DDRE = 0x0E;
	PORTE = 0x04;
	PORTE = 0x0F;
}

void init_interrupt(void){			// ���ͷ�Ʈ ���� ���� �ʱ�ȭ
	EIMSK = 0x00;				// 0���� �ʱ�ȭ
	EICRA = 0x00;				
	EICRB = 0x0A;				// �ܺ� ���ͷ�Ʈ 4�� 5���� falling edge�� ���
	EIMSK = 0x30;				// �ܺ� ���ͷ�Ʈ 4�� 5�� enable
	EIFR = 0xFF;			// ���ͷ�Ʈ �÷��� �ʱ�ȭ
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

void newline(){
	while((UCSR0A & 0x20) == 0x00);		
	UDR0 = '\r';						// Ŀ�� �Ǿ����� ���

	while((UCSR0A & 0x20) == 0x00);
	UDR0 = '\n';						// �ٹٲ�
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
		if(temp==t){
			temp = 0;
			index[3]++;
			if(index[3]>=10){
				index[3]=0;
				index[2]++;
				if(index[2]>=6){
					index[2]=0;
					index[1]++;
					if(index[1]>=10){
						index[1]=0;
						index[0]++;
						if(index[0]>=6) index[0]=0;
					}
				}
			}
		}
	}
	else if(stopgo==2){
		temp++;
		if(temp == t){
			temp=0;
			index[3]--;
			if((index[3]<0)&(index[2]>0)){		// ���� 1���ڸ��� ������ 10���ڸ��� 0�̻��̸� 
				index[2]--;						// 10�� �ڸ����� 1 ���ҽ�Ű�� 
				index[3] = 9;					// 1�� �ڸ����� 9�� ����
			}
			else if((index[3]<0)&(index[1]>0)&(index[2]==0)){	// ���� 1���ڸ��� ����, 10���ڸ��� 0, 100�� �ڸ��� 0�̻��̸�
				index[1]--;						// 100�� �ڸ����� 1���ҽ�Ű��
				index[2]=5;						// 10�� �ڸ����� 9�� ����
				index[3]=9;						// 1�� �ڸ����� 9�� ����
			}
			else if((index[3]<0)&(index[0]>0)&(index[1]==0)&(index[2]==0)){		// ���� 1�� �ڸ����� ����, 10, 100�� �ڸ��� 0, 1000�� �ڸ��� 0 �̻��̸�
				index[0]--;						// 1000 �ڸ��� 1����
				index[1]=9;						// 100 �ڸ��� 9�� ����
				index[2]=5;						// 10 �ڸ��� 9�� ����
				index[3]=9;						// 1 �ڸ��� 9�� ����
			}
			else if((index[3]<0)&(index[0]==0)&(index[1]==0)&(index[2]==0))	// ���� 1�� �ڸ����� ����, ������ �ڸ������� ��� 0�̸�	
				index[3] = 0;						// 1���ڸ����� �� �̻� ���ҽ�Ű�� �ʰ� 0 ����	
		}	
	}
}

int main(void){
	timer_setting();
	init_port();
	init_interrupt();
	init_uart();
	int j;
	char tx, rx;
	int cnt;

	sei();
	while(1){
		display();
		switch(PING & 0x0F){
			case 0x0E:					// 5�� ����ġ start
				stopgo = 1;
				break;

			case 0x0D:					// 6�� ����ġ stop, display
				_delay_ms(150);
				stopgo = 0;
				display_screen();
				break;

			case 0x0B:					// 7�� ����ġ reset, display
				_delay_ms(150);
				index[0]=index[1]=index[2]=index[3]=0;
				display_screen();
				break;
				
			case 0x07:
				_delay_ms(150);
				cnt=0;
				while(1){
					display();
					while((UCSR0A & 0x80) == 0x00){
						display();
					}
					rx = UDR0;
					tx = rx;
					buffer[cnt++]=rx;
					
					while((UCSR0A & 0x20) == 0x00){		
						display();				
					}

					UDR0 = tx;
					if(cnt>=4) break;
				}

				for(j=0; j<4; j++){
					index[j] = buffer[j]-48;
				}
				newline();
				break;
		}
	}

}

ISR(INT4_vect){
	stopgo=1;
}

ISR(INT5_vect){
	stopgo=2;
}

	
