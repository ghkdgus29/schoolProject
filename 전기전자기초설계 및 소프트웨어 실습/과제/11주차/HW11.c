#include <avr/io.h>				
#include <avr/interrupt.h>				// 인터럽트를 사용하기 위한 헤더
#include <util/delay.h>					// 딜레이를 사용하기 위한 헤더

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};		// 점을 포함하지 않는 7segment led data 배열
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0xA7, 0xFF, 0xEF};		// 점을 포함하는 7segment led data 배열
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};						// led ctrl 을 위한 포지션 배열(숫자판 위치)				
unsigned char index[4] = {0, 0, 0, 0};										// 시간 값을 나타내는 인덱스 배열

volatile int temp = 0;					// 타이머 인터럽트가 발생하면 temp 값을 증가시킨다.
int t = 78;								// temp가 78이 되면 1/100sec 값이 1 증가한다.
int stopgo = 0;							// 타이머를 동작시킬지 정지시킬지 정하는 flag

void init_port(){						// led 관련 레지스터 설정 초기화 함수
	

	DDRA = 0xFF;						// 포트 A 모두 출력으로 사용 (7 segment)
	DDRE = 0x0C;						// led ctrl , led data 모두 사용하기 위한 레지스터 설정
	PORTE = 0x04;						// led ctrl 먼저 사용 (숫자판 위치)
	PORTA = 0x0F;						// 숫자판 4개 모두 출력으로 사용
}

void timer_setting(){					// 타이머 인터럽트 관련 레지스터 설정 초기화 함수
	TCCR0 = _BV(CS01);					// clk/8 로 분주 설정
	TCNT0 = 0x00;						// 타이머 오버플로우가 발생하면 다시 0으로 초기화
	TIMSK = 0x01;						// 타이머 오버플로우 인터럽트가 발생하도록 설정
	TIFR = 0xFF;						// 타이머 인터럽트 관련 플래그
}

void init_interrupt(){					// 외부 인터럽트 관련 레지스터 설정 초기화 함수
	EIMSK = 0x00;						// 외부 인터럽트 초기화
	EICRA = 0x00;						
	EICRB = 0x0A;						// 외부 인터럽트 4번 5번 falling edge로 사용
	EIMSK = 0x30;						// 외부 인터럽트 4번 5번 enable
	EIFR = 0xFF;						// 외부 인터럽트 관련 플래그
}

void display(){							// 타이머의 display를 위한 함수
	int j;
	for(j=0; j<4; j++){					// 숫자판 4개를 보여준다
		PORTA = FND_SEGPOS[j];
		PORTE = 0x04;					// 숫자판을 번갈아가며 led data 값을 해당 위치에 출력
		PORTE = 0x00;	
		
		if(j==1) PORTA = FND_SEGWP[index[j]];		// 만약 1sec 자리이면 .을 포함한 led data 7segment 출력
		else PORTA = FND_SEGNP[index[j]];			// 이외의 자리에선 .을 포함하지 않는 led data 7segment 출력
		PORTE = 0x08;					// led data 출력
		PORTE = 0x00;					
		_delay_ms(1);					// 짧은 시간 딜레이를 주어 항상 켜져있는것 처럼 보인다
	}
}


ISR(TIMER0_OVF_vect){				// 타이머 인터럽트
	if(stopgo==1){				// 만약 stopgo 플래그가 1이면 타이머가 시작된다.
		temp++;					// temp값 증가
		if(temp == t){			// temp값이 t와 같아지면
			temp = 0;			// temp는 0으로 초기화
			index[3]++;			//  1/100sec 에 해당하는 인덱스 값 증가
			if(index[3]>=10){				// 1/100 sec 가 10이 되면
				index[3]=0;					// 해당 인덱스 값 0 초기화
				index[2]++;					// 1/10 sec 인덱스 1증가
				if(index[2]>=10){					// 1/10 sec가 10이 되면
					index[2]=0;						// 해당 인덱스 값 0 초기화
					index[1]++;						// 1sec 인덱스 1증가
					if(index[1]>=10){						// 1sec가 10이 되면
						index[1]=0;							// 해당 인덱스 값 0 초기화 
						index[0]++;							// 10sec 값 1증가
						if(index[0]>=10) index[0]=0;		// 10sec 가 10이 되면 해당 인덱스 값 0 초기화
					}
				}
			}
			
		}
	}	
}	

ISR(INT4_vect){							// 외부 인터럽트 4번이 발생하면 
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 지연함수
	for(int i=0; i<4; i++){	
		index[i]=0;						// 0000 으로 인덱스값들 초기화 (타이머 0000초기화)
	}

}

ISR(INT5_vect){							// 외부 인터럽트 5번이 발생하면
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 지연함수
	if(stopgo == 0) stopgo=1;			// stopgo 레지스터가 0이면 1로 바꾸고
	else stopgo = 0;					// stopgo 레지스터가 1이면 0으로 바꾼다.
}


int main(void){
	timer_setting();					// 타이머 인터럽트 설정
	init_port();						// led 관련 레지스터 설정
	init_interrupt();					// 외부 인터럽트 설정

	sei();								// 인터럽트 enable
	while(1){			
		display();						// 계속해서 타이머 값 출력
	}
	return 0;
}
