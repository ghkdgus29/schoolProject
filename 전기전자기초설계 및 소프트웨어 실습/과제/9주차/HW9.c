#include <avr/io.h>
#include <util/delay.h>			

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};			//	���� �������� �ʴ� 7segment 0~9
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};			// ���� �����ϴ� 7segment 0~9
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												// ���� ���� �������� ��ġ�� �����ϴ� �迭
unsigned char index[4] = {0, 0, 0, 0};																// �ε����� �����ϴ� �迭, �ε����� ���� Ÿ�̸Ӹ� ǥ���Ұ���														


void init_port(){								// ��Ʈ �ʱ�ȭ �Լ�

	DDRG = 0x00;								// G���� �Է����� ���

	DDRA = 0xFF;								// 7segment �� ��� ������� ���				
	DDRE = 0x0C;								// LED ctrl, LED data ���
	PORTE = 0x04;								// LED ctrl ���� ���
	PORTA = 0x0F;								// 4���� ������ ��� ���
}

void one(){										// 1/100 �ʸ� ��Ÿ���� �Լ�
	int i=0, j, cnt;
	while(i<10){								// 1/100 �ʰ� 10�� �ݺ��Ǹ� �ݺ��� Ż��
		cnt = 0;
		while(cnt<10){							// 1ms�� 10�� �ݺ��Ͽ� cnt�� 10�̵Ǹ� �ݺ��� Ż�� (1/100 sec)
			for(j=0; j<4; j++){					// �������� ����� ���� for������ ���������� �̿�
				PORTA = FND_SEGPOS[j];			// j�� �ٲ㰡�鼭 �������� �ٸ� ���ڵ��� display ��Ŵ 
				PORTE = 0x04;					// LED ctrl (������ ����)
				PORTE = 0x00;					// �ʱ�ȭ

				if(j==1) PORTA = FND_SEGWP[index[j]];		// 1�� �������� ��� �Ҽ����� �����ϴ� segment ǥ��, index�� ���� ���� �ð��� ��Ÿ����.
				else PORTA = FND_SEGNP[index[j]];			// 1�� �������� �ƴ� ��� �Ҽ����� �������� �ʴ� segment ǥ��, index�� ���� ���� �ð��� ��Ÿ����.
				PORTE = 0x08;					// LED data (�������� led data���)
				PORTE = 0x00;					// �ʱ�ȭ
				_delay_ms(1);					// 1ms ������
				cnt++;							// cnt����
			}
		}
		if((PING&0x0F)== 0x0B) break;
		if((PING&0x0F)== 0x07) break;			// ���� SW8, SW7 �� ������ Ÿ�̸Ӹ� �ߴ��Ѵ�.
		i++;									// i�� ������Ų��
		index[3]++;								// 1/100 �ʸ� ����Ű�� index ����
	}

}

void timer(){								// Ÿ�̸��Լ�
	int i=0, j, cnt;
	while(1){
		one();								// 1/100�ʺ��� Ÿ�̸� ����
		if((PING&0x0F)== 0x0B) break;
		if((PING&0x0F)== 0x07) break;		// ���� SW8, SW7 �� ������ Ÿ�̸� �ߴ�
		index[3] = 0;						// 1/100�ʰ� 10�� �ݺ��Ǹ� �ٽ� 0���� �ε��� �ʱ�ȭ	
		index[2]++;							// 1/10�� �ε��� ����
		if(index[2]>=10){					// 1/10�ʰ� 10�� �ݺ��Ǹ�
			index[2]=0;						// �ٽ� 0���� �ε��� �ʱ�ȭ
			index[1]++;						// 1�� �ε��� ����
			if(index[1]>=10){				// 1�ʰ� 10�� �ݺ��Ǹ�
				index[1] = 0;				// 1�� �ε��� 0���� �ʱ�ȭ
				index[0]++;					// 10�� �ε��� ����
				if(index[0]>=10){			// 10�ʰ� 10�� �ݺ��Ǹ�
					index[0] = 0;			// 10�� �ε��� 0���� �ʱ�ȭ
				}
			}			
		}
	}		
}

void set(){									// 0000���� �ʱ�ȭ ��Ű�� �Լ�
	int j;
	for(j=0; j<4; j++) index[j] = 0;		// �ε������� ��� 0���� �ʱ�ȭ
	PORTE = 0x04;
	PORTA = 0x0F;							// LED ctrl (������ �ڸ���) ������ 4�� ��� ���
				
	PORTE = 0x08;							// led data (7 segment) 
	PORTA = FND_SEGNP[0];					// led data 0�� ���
	PORTE = 0x00;							
}


int main(void){							
	init_port();							// ��Ʈ ���� �ʱ�ȭ
	int j;
	while(1){
		switch(PING & 0x0F){				// SW ��ư ������ �� ���� ����
						
			case 0x0D:						// SW6 ���� ��� timer start
				timer();
				break;

			case 0x0B:						// sw7 ���� ��� timer �Ͻ�����, �ٽ����
				_delay_ms(100);				// ����ġ �ٿ���� ���� ���� delay
				while(1){
					for(j=0; j<4; j++){					// Ÿ�̸� ���� �� ���������� �̿��� ���� �ٸ� ���ڵ��� ���
						PORTA = FND_SEGPOS[j];
						PORTE = 0x04;					// led ctrl
						PORTE = 0x00;

						if(j==1) PORTA = FND_SEGWP[index[j]];
						else PORTA = FND_SEGNP[index[j]];			// Ÿ�̸Ӹ� �����Ͽ��� ������ �ε����� ��ȭ�� ���� ���� �ð����� �������� ���
						PORTE = 0x08;					// led data
						PORTE = 0x00;
						_delay_ms(1);
					}
					
					if((PING&0x0F)== 0x0B) break;		// ���� SW7�� �ٽ� ������ �ݺ��� Ż��, timer�� �ٽ� �����Ų��.
					if((PING&0x0F)== 0x07) set();		// ���� SW8�� ������ set�Լ��� �̵��Ͽ� 0000�ʱ�ȭ
					
				}
				_delay_ms(100);							// ����ġ �ٿ���� ���� ���� delay
				
				timer();								// timer �簳
				break;

			case 0x07:									// SW8 �� ������ timer�� �ʱ�ȭ
				for(j=0; j<4; j++) index[j] = 0;		// �ε��� 0���� ��� �ʱ�ȭ
				PORTE = 0x04;
				PORTA = 0x0F;							// led ctrl ��� ������ ���
					
				PORTE = 0x08;							// led data ���
				PORTA = FND_SEGNP[0];					// 0000���
				PORTE = 0x00;
				break;
				
				

			default:									// reset ���� ��� 0000 ���� �ʱ�ȭ
				for(j=0; j<4; j++) index[j] = 0;
				PORTE = 0x04;
				PORTA = 0x0F;
					
				PORTE = 0x08;
				PORTA = FND_SEGNP[0];
				PORTE = 0x00;
				break;
			
		}
	}	
	return 0;

}
