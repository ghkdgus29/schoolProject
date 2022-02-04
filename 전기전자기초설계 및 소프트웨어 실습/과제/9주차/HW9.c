#include <avr/io.h>
#include <util/delay.h>			

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};			//	점을 포함하지 않는 7segment 0~9
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};			// 점을 포함하는 7segment 0~9
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												// 불이 들어올 숫자판의 위치를 설정하는 배열
unsigned char index[4] = {0, 0, 0, 0};																// 인덱스를 저장하는 배열, 인덱스를 통해 타이머를 표현할것임														


void init_port(){								// 포트 초기화 함수

	DDRG = 0x00;								// G핀을 입력으로 사용

	DDRA = 0xFF;								// 7segment 를 모두 출력으로 사용				
	DDRE = 0x0C;								// LED ctrl, LED data 사용
	PORTE = 0x04;								// LED ctrl 먼저 사용
	PORTA = 0x0F;								// 4개의 숫자판 모두 출력
}

void one(){										// 1/100 초를 나타내는 함수
	int i=0, j, cnt;
	while(i<10){								// 1/100 초가 10번 반복되면 반복문 탈출
		cnt = 0;
		while(cnt<10){							// 1ms가 10번 반복하여 cnt가 10이되면 반복문 탈출 (1/100 sec)
			for(j=0; j<4; j++){					// 숫자판의 출력을 위한 for문으로 착시현상을 이용
				PORTA = FND_SEGPOS[j];			// j를 바꿔가면서 숫자판의 다른 숫자들을 display 시킴 
				PORTE = 0x04;					// LED ctrl (숫자판 수정)
				PORTE = 0x00;					// 초기화

				if(j==1) PORTA = FND_SEGWP[index[j]];		// 1초 숫자판일 경우 소수점을 포함하는 segment 표현, index를 통해 현재 시간을 나타낸다.
				else PORTA = FND_SEGNP[index[j]];			// 1초 숫자판이 아닐 경우 소수점을 포함하지 않는 segment 표현, index를 통해 현재 시간을 나타낸다.
				PORTE = 0x08;					// LED data (숫자판의 led data출력)
				PORTE = 0x00;					// 초기화
				_delay_ms(1);					// 1ms 딜레이
				cnt++;							// cnt증가
			}
		}
		if((PING&0x0F)== 0x0B) break;
		if((PING&0x0F)== 0x07) break;			// 만약 SW8, SW7 을 누르면 타이머를 중단한다.
		i++;									// i를 증가시킨다
		index[3]++;								// 1/100 초를 가리키는 index 증가
	}

}

void timer(){								// 타이머함수
	int i=0, j, cnt;
	while(1){
		one();								// 1/100초부터 타이머 동작
		if((PING&0x0F)== 0x0B) break;
		if((PING&0x0F)== 0x07) break;		// 만약 SW8, SW7 을 누르면 타이머 중단
		index[3] = 0;						// 1/100초가 10번 반복되면 다시 0으로 인덱스 초기화	
		index[2]++;							// 1/10초 인덱스 증가
		if(index[2]>=10){					// 1/10초가 10번 반복되면
			index[2]=0;						// 다시 0으로 인덱스 초기화
			index[1]++;						// 1초 인덱스 증가
			if(index[1]>=10){				// 1초가 10번 반복되면
				index[1] = 0;				// 1초 인덱스 0으로 초기화
				index[0]++;					// 10초 인덱스 증가
				if(index[0]>=10){			// 10초가 10번 반복되면
					index[0] = 0;			// 10초 인덱스 0으로 초기화
				}
			}			
		}
	}		
}

void set(){									// 0000으로 초기화 시키는 함수
	int j;
	for(j=0; j<4; j++) index[j] = 0;		// 인덱스값을 모두 0으로 초기화
	PORTE = 0x04;
	PORTA = 0x0F;							// LED ctrl (숫자판 자릿수) 숫자판 4개 모두 사용
				
	PORTE = 0x08;							// led data (7 segment) 
	PORTA = FND_SEGNP[0];					// led data 0값 출력
	PORTE = 0x00;							
}


int main(void){							
	init_port();							// 포트 설정 초기화
	int j;
	while(1){
		switch(PING & 0x0F){				// SW 버튼 눌렀을 때 동작 설정
						
			case 0x0D:						// SW6 누를 경우 timer start
				timer();
				break;

			case 0x0B:						// sw7 누를 경우 timer 일시정지, 다시재생
				_delay_ms(100);				// 스위치 바운싱을 막기 위한 delay
				while(1){
					for(j=0; j<4; j++){					// 타이머 중지 후 착시현상을 이용해 각기 다른 숫자들을 출력
						PORTA = FND_SEGPOS[j];
						PORTE = 0x04;					// led ctrl
						PORTE = 0x00;

						if(j==1) PORTA = FND_SEGWP[index[j]];
						else PORTA = FND_SEGNP[index[j]];			// 타이머를 중지하였기 때문에 인덱스에 변화가 없어 현재 시간값이 증가없이 출력
						PORTE = 0x08;					// led data
						PORTE = 0x00;
						_delay_ms(1);
					}
					
					if((PING&0x0F)== 0x0B) break;		// 만약 SW7을 다시 누르면 반복문 탈출, timer를 다시 재생시킨다.
					if((PING&0x0F)== 0x07) set();		// 만약 SW8을 누르면 set함수로 이동하여 0000초기화
					
				}
				_delay_ms(100);							// 스위치 바운싱을 막기 위한 delay
				
				timer();								// timer 재개
				break;

			case 0x07:									// SW8 을 누르면 timer를 초기화
				for(j=0; j<4; j++) index[j] = 0;		// 인덱스 0으로 모두 초기화
				PORTE = 0x04;
				PORTA = 0x0F;							// led ctrl 모든 숫자판 사용
					
				PORTE = 0x08;							// led data 출력
				PORTA = FND_SEGNP[0];					// 0000출력
				PORTE = 0x00;
				break;
				
				

			default:									// reset 누를 경우 0000 으로 초기화
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
