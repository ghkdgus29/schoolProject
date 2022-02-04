#include <avr/io.h>				
#include <avr/interrupt.h>				// ���ͷ�Ʈ�� ����ϱ� ���� ���
#include <util/delay.h>					// �����̸� ����ϱ� ���� ���

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};		// ���� �������� �ʴ� 7segment led data �迭
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};		// ���� �����ϴ� 7segment led data �迭
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};						// led ctrl �� ���� ������ �迭(������ ��ġ)				
unsigned char index[4] = {0, 0, 0, 0};										// �ð� ���� ��Ÿ���� �ε��� �迭

volatile int temp = 0;					// Ÿ�̸� ���ͷ�Ʈ�� �߻��ϸ� temp ���� ������Ų��.
int t = 78;								// temp�� 78�� �Ǹ� 1/100sec ���� 1 �����Ѵ�.
int stopgo = 0;							// Ÿ�̸Ӹ� ���۽�ų�� ������ų�� ���ϴ� flag

void init_port(){						// led ���� �������� ���� �ʱ�ȭ �Լ�
	

	DDRA = 0xFF;						// ��Ʈ A ��� ������� ��� (7 segment)
	DDRE = 0x0C;						// led ctrl , led data ��� ����ϱ� ���� �������� ����
	PORTE = 0x04;						// led ctrl ���� ��� (������ ��ġ)
	PORTA = 0x0F;						// ������ 4�� ��� ������� ���
}

void timer_setting(){					// Ÿ�̸� ���ͷ�Ʈ ���� �������� ���� �ʱ�ȭ �Լ�
	TCCR0 = _BV(CS01);					// clk/8 �� ���� ����
	TCNT0 = 0x00;						// Ÿ�̸� �����÷ο찡 �߻��ϸ� �ٽ� 0���� �ʱ�ȭ
	TIMSK = 0x01;						// Ÿ�̸� �����÷ο� ���ͷ�Ʈ�� �߻��ϵ��� ����
	TIFR = 0xFF;						// Ÿ�̸� ���ͷ�Ʈ ���� �÷���
}

void init_interrupt(){					// �ܺ� ���ͷ�Ʈ ���� �������� ���� �ʱ�ȭ �Լ�
	EIMSK = 0x00;						// �ܺ� ���ͷ�Ʈ �ʱ�ȭ
	EICRA = 0x00;						
	EICRB = 0x0A;						// �ܺ� ���ͷ�Ʈ 4�� 5�� falling edge�� ���
	EIMSK = 0x30;						// �ܺ� ���ͷ�Ʈ 4�� 5�� enable
	EIFR = 0xFF;						// �ܺ� ���ͷ�Ʈ ���� �÷���
}

void display(){							// Ÿ�̸��� display�� ���� �Լ�
	int j;
	for(j=0; j<4; j++){					// ������ 4���� �����ش�
		PORTA = FND_SEGPOS[j];
		PORTE = 0x04;					// �������� �����ư��� led data ���� �ش� ��ġ�� ���
		PORTE = 0x00;	
		
		if(j==1) PORTA = FND_SEGWP[index[j]];		// ���� 1sec �ڸ��̸� .�� ������ led data 7segment ���
		else PORTA = FND_SEGNP[index[j]];			// �̿��� �ڸ����� .�� �������� �ʴ� led data 7segment ���
		PORTE = 0x08;					// led data ���
		PORTE = 0x00;					
		_delay_ms(1);					// ª�� �ð� �����̸� �־� �׻� �����ִ°� ó�� ���δ�
	}
}


ISR(TIMER0_OVF_vect){				// Ÿ�̸� ���ͷ�Ʈ
	if(stopgo==1){				// ���� stopgo �÷��װ� 1�̸� Ÿ�̸Ӱ� ���۵ȴ�.
		temp++;					// temp�� ����
		if(temp == t){			// temp���� t�� ��������
			temp = 0;			// temp�� 0���� �ʱ�ȭ
			index[3]++;			//  1/100sec �� �ش��ϴ� �ε��� �� ����
			if(index[3]>=10){				// 1/100 sec �� 10�� �Ǹ�
				index[3]=0;					// �ش� �ε��� �� 0 �ʱ�ȭ
				index[2]++;					// 1/10 sec �ε��� 1����
				if(index[2]>=10){					// 1/10 sec�� 10�� �Ǹ�
					index[2]=0;						// �ش� �ε��� �� 0 �ʱ�ȭ
					index[1]++;						// 1sec �ε��� 1����
					if(index[1]>=10){						// 1sec�� 10�� �Ǹ�
						index[1]=0;							// �ش� �ε��� �� 0 �ʱ�ȭ 
						index[0]++;							// 10sec �� 1����
						if(index[0]>=10) index[0]=0;		// 10sec �� 10�� �Ǹ� �ش� �ε��� �� 0 �ʱ�ȭ
					}
				}
			}
			
		}
	}	
}	

ISR(INT4_vect){							// �ܺ� ���ͷ�Ʈ 4���� �߻��ϸ� 
	_delay_ms(100);						// debouncing���� ���� ������ ���̱� ���� �����Լ�
	for(int i=0; i<4; i++){	
		index[i]=0;						// 0000 ���� �ε������� �ʱ�ȭ (Ÿ�̸� 0000�ʱ�ȭ)
	}

}

ISR(INT5_vect){							// �ܺ� ���ͷ�Ʈ 5���� �߻��ϸ�
	_delay_ms(100);						// debouncing���� ���� ������ ���̱� ���� �����Լ�
	if(stopgo == 0) stopgo=1;			// stopgo �������Ͱ� 0�̸� 1�� �ٲٰ�
	else stopgo = 0;					// stopgo �������Ͱ� 1�̸� 0���� �ٲ۴�.
}


int main(void){
	timer_setting();					// Ÿ�̸� ���ͷ�Ʈ ����
	init_port();						// led ���� �������� ����
	init_interrupt();					// �ܺ� ���ͷ�Ʈ ����

	sei();								// ���ͷ�Ʈ enable
	while(1){			
		display();						// ����ؼ� Ÿ�̸� �� ���
	}
	return 0;
}
