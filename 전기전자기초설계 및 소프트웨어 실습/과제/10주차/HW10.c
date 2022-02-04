#include <avr/io.h>
#include <avr/interrupt.h>						// 인터럽트를 사용하기 위한 헤더
#include <util/delay.h>							// 딜레이함수를 사용하기 위한 헤더

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};			// 7segment 값을 저장하는 배열
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												// 점등시킬 숫자판 값을 저장하는 배열
int index[4] = {0, 0, 0, 0};					// 인덱스를 저장하는 배열,  display함수에서 인덱스값을  FND_SEGNP배열에 더하여 led data를 출력한다.


void init_port(void){					// led 설정 초기화 함수
	DDRA = 0xFF;						// 7segment값 모두 출력하기 위한 레지스터 설정
	DDRE = 0x0C;						// led ctrl , led data 모두 사용하기 위한 레지스터 설정
	PORTE = 0x04;						// led ctrl 먼저 사용 (숫자판 자리)
	PORTA = 0x0F;						// 숫자판 4개 모두 출력으로 사용
}

void init_interrupt(void){				// 인터럽트 설정 초기화 함수
	
	EIMSK = 0x00;						// 인터럽트 입력 초기화
	EICRA = 0x00;						// 0~3 번 외부 인터럽트 설정 레지스터
	EICRB = 0x0A;						// 4~7 번 외부 인터럽트 설정 레지스터로 4, 5 인터럽트를 falling edge 로 사용
	EIMSK = 0x30;						// 4, 5 번 외부 인터럽트를 입력으로 사용
	EIFR = 0xFF;						// 인터럽트 플래그를 초기화
}

void display(){							// avr의 led data display를 위한 함수
	int j;
	for(j=0; j<4; j++){					// for문 4번 반복,, 숫자판 4개를 빠른속도로 번갈아가며 출력 -> 사람눈엔 다른 숫자들이 출력된 것으로 보임
		PORTA = FND_SEGPOS[j];			// 숫자판 위치설정
		PORTE = 0x04;					// led ctrl 사용 (숫자판 위치)
		PORTE = 0x00;					// 초기화 

		PORTA = FND_SEGNP[index[j]];	// 숫자값에 해당하는 led data 출력 , 인덱스를 통해 해당하는 카운트 숫자를 출력한다.
		PORTE = 0x08;					// led data 사용 (7segment 점등)
		PORTE = 0x00;					// 초기화
		_delay_ms(1);					// 짧은 시간 딜레이를 주어 착시현상 이용
	}
}

int main(void){							
	init_port();						// avr의 led관련 레지스터 설정 초기화
	init_interrupt();					// 외부 인터럽트 관련 레지스터 설정 초기화
	
	sei();								// global interrupt enable 레지스터로 외부인터럽트 입력을 받아들이기 위한 설정
	while(1){
		display();						// 계속해서 카운터 숫자들을 나타내기 위한 display 함수 반복
	}
	return 0;
}

ISR(INT4_vect){							// 만약 외부 인터럽트 4번이 발생하였을 때 ( 4번 인터럽트 스위치 누를경우) 1 다운카운트
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 딜레이
	index[3]--;							// 인덱스 배열의 4번째 값을 감소시킨다. (1의 자리 카운트 감소)
	if((index[3]<0)&(index[2]>0)){		// 만약 1의자리가 음수고 10의자리가 0이상이면 
		index[2]--;						// 10의 자리수를 1 감소시키고 
		index[3] = 9;					// 1의 자리수를 9로 변경
	}
	else if((index[3]<0)&(index[1]>0)&(index[2]==0)){	// 만약 1의자리가 음수, 10의자리가 0, 100의 자리가 0이상이면
		index[1]--;						// 100의 자리수를 1감소시키고
		index[2]=9;						// 10의 자리수는 9로 변경
		index[3]=9;						// 1의 자리수는 9로 변경
	}
	else if((index[3]<0)&(index[0]>0)&(index[1]==0)&(index[2]==0)){		// 만약 1의 자리수가 음수, 10, 100의 자리가 0, 1000의 자리가 0 이상이면
		index[0]--;						// 1000 자리수 1감소
		index[1]=9;						// 100 자리수 9로 변경
		index[2]=9;						// 10 자리수 9로 변경
		index[3]=9;						// 1 자리수 9로 변경
	}
	else if((index[3]<0)&(index[0]==0)&(index[1]==0)&(index[2]==0))	// 만약 1의 자리수가 음수, 나머지 자리수들이 모두 0이면	
	index[3] = 0;						// 1의자리수는 더 이상 감소시키지 않고 0 유지	
}

ISR(INT5_vect){							// 만약 외부 인터럽트 5번이 발생하였을 때 ( 5번 인터럽트 스위치 누를경우) 1 업카운트
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 지연 함수
	index[3]++;							// 1의 자리수 증가 
	if((index[3]>9)&(index[2]==9)&(index[1]==9)&(index[0]==9)){	// 만약 9999인 경우 더 이상 1증가 시킬 수 없을 때
		index[0]=9;
		index[1]=9;
		index[2]=9;
		index[3]=9;					// 9999 유지
	}

	else if(index[3]>=10){				// 만약 1의 자리수가 9를 넘어 자리수 증가가 일어나면
		index[3]=0;						// 1의 자리수는 0
		index[2]++;						// 10의 자리수는 1증가
		if(index[2]>=10){				// 만약 10의 자리수가 9를 넘어 자리수 증가가 일어나면
			index[2]=0;					// 10의 자리수는 0
			index[1]++;					// 100의 자리수는 1증가
			if(index[1]>=10){			// 만약 100의 자리수가 9를 넘어 자리수 증가가 일어나면
				index[1]=0;				// 100의 자리수는 0
				index[0]++;				// 1000의 자리수는 1증가
			}
		}
	}	
}

