#include <avr/io.h>
#include <avr/interrupt.h>					// 인터럽트를 사용하기 위한 헤더선언
#include <util/delay.h>						// 딜레이 함수를 사용하기 위한 헤더선언

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};			// 7segment leddata값을 저장하고 있는 배열
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};												// 7segment led의 숫자판 위치를 저장하고 있는 배열
int index[4] = {0, 0, 0, 0};				// count값을 저장할 index배열

void init_port(void){				// led port 초기화
	DDRA = 0xFF;					// 7segment 모두 출력으로 사용
	DDRE = 0x0E;					// RX 입력, TX 출력, led ctrl, led data 사용
	PORTE = 0x04;					// led ctrl (숫자판 위치) 먼저 설정
	PORTA = 0x0F;					// 숫자판 4개 모두 사용
}

void init_interrupt(void){			// 인터럽트 관련 설정 초기화
	EIMSK = 0x00;				// 0으로 초기화
	EICRA = 0x00;				
	EICRB = 0x0A;				// 외부 인터럽트 4번 5번을 falling edge로 사용
	EIMSK = 0x30;				// 외부 인터럽트 4번 5번 enable
	EIFR = 0xFF;			// 인터럽트 플래그 초기화
}

void display_timer(){						// count를 표시하는 함수
	int j;
	for(j=0; j<4; j++){						// for문을 돌리면서 led 숫자판 4개 컨트롤
		PORTA = FND_SEGPOS[j];				
		PORTE = 0x04;						// 숫자판 위치를 바꿔가며 점등
		PORTE = 0x00;

		PORTA = FND_SEGNP[index[j]];
		PORTE = 0x08;						// 해당 숫자판의 led data를 인덱스를 간접참조하여 7segment값 출력
		PORTE = 0x00;
		_delay_ms(1);						// 이를 빠른 시간 반복하여 숫자판이 항상 켜져있는 것 처럼 보인다.
	}
}

void display_screen(){						// putty 에 AVR 의 count값을 출력하도록 하는 함수
	int i;
	for(i=0; i<4; i++){							// for문을 돌려가면서 4개의 index배열 값들을 char타입으로 출력
		while((UCSR0A & 0x20) == 0x00);			// 송신 버퍼에 아직 전달하지 못한 데이터값이 있다면 기다린다.
		UDR0 = index[i]+48;					// 인덱스 배열의 값에 48을 더하여 아스키값으로 숫자를 나타낸다.
		if(i==3) {				// 만약 마지막자리 숫자까지 입력이 끝나면
			while((UCSR0A & 0x20) == 0x00);				
			UDR0 = '\r';						// 커서를 맨앞으로 당긴다.

			while((UCSR0A & 0x20) == 0x00);	
			UDR0 = '\n';						// 줄바꿈을 수행한다.
		}
	}

}

void init_uart(void){				// UART 통신 관련 설정 초기화
	
	UBRR0H = 0;	
	UBRR0L = 103;				// BAUD rate = 9600BPS 로 설정

	UCSR0C = 0x06;				// start bit 1, data bit 8, stop bit 1, no parity bit 로 설정
	
	UCSR0B = 0x18;				// 송수신부 enable		
}

int main(){
	init_port();					// led port관련 초기화
	init_interrupt();				// 외부 인터럽트 관련 초기화
	init_uart();					// 통신 관련 설정 초기화

	char rx, tx;			// char 타입 변수 선언

	sei();			// 인터럽트 enable

	display_screen();		// putty에 숫자값 display
	while(1){
		display_timer();		// AVR에 숫자값 display

		while((UCSR0A & 0x80) == 0x00) {		// 수신 버퍼로부터 데이터를 읽는 중이면 기다리면서
			display_timer();				// AVR 숫자값 display
		}
		rx = UDR0;				// rx에 읽어들인 데이터값 저장   ( putty -> avr)
		tx = rx;				// rx 를 tx에 저장 

		while((UCSR0A & 0x20) == 0x00){			// 송신 버퍼로부터 데이터를 전달하는 중이면 기다리면서 
			display_timer();				// AVR 숫자값 display
		}

		UDR0 = tx;				// tx를 데이터 레지스터에 저장 ( avr -> putty)
		
		if((tx == 'u')|(tx == 'U')) Upcount();		// 읽어들인 값이 u, U이면 counter 증가
		else if((tx == 'd')|(tx == 'D')) Downcount();		// 읽어들인 값이 d, D이면 counter 감소		
	}
	return 0;			
}

void Upcount(){				// counter 증가 함수
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 지연 함수
	while((UCSR0A & 0x20) == 0x00);		
	UDR0 = '\r';						// 커서 맨앞으로 당김

	while((UCSR0A & 0x20) == 0x00);
	UDR0 = '\n';						// 줄바꿈

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
	display_screen();			// putty에 숫자값 display
}

void Downcount(){
	_delay_ms(100);						// debouncing으로 인한 오차를 줄이기 위한 딜레이

	while((UCSR0A & 0x20) == 0x00);
	UDR0 = '\r';						// 커서 맨앞으로 당김

	while((UCSR0A & 0x20) == 0x00);
	UDR0 = '\n';						// 줄바꿈
		
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
	display_screen();
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
	display_screen();		// putty에 숫자값 display
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
	display_screen();		// putty에 숫자값 display
}




