#include <avr/io.h>		
#include <avr/interrupt.h>				// 인터럽트를 사용하기 위한 헤더

volatile int temp = 0;					// 타이머 인터럼트가 발생할 때 마다 temp값 증가
int t = 61;								// clk/1024 분주 설정하였으므로 16000000/256/1024 = 61.04 이다.
										// temp값과 t값이 같아지기까지 걸리는 시간이 1초에 해당한다.
ISR(TIMER0_OVF_vect) {					// 타이머 인터럽트 발생
	temp++;						// temp 증가
	if (temp == t) {			// 1초가 지나면
		temp = 0;				// temp 0으로 초기화
		PORTF = (PORTF == (0x80)) ? 0x10 : PORTF << 1;			// F 포트에 출력하고있는 led 값이
	}															// 0x80이면 0x10으로 바꾸고 , 아니라면 왼쪽으로 한칸 비트 쉬프트
}

void timer_setting() {								// 타이머 인터럽트 관련 레지스터 설정 초기화 함수
	TCCR0 = _BV(CS00) | _BV(CS01) | _BV(CS02);		// clk/1024 분주 설정 
	TCNT0 = 0x00;				// 타이머 인터럽트가 발생하면 0으로 초기화
	TIMSK = 0x01;				// 타이머 OVF 인터럽트 설정
	TIFR = 0xFF;				// 타이머 인터럽트 관련 플래그
}

int main(void) {				
	timer_setting();			// 타이머 인터럽트 설정 
	DDRF = 0xFF;				// F포트 모두 출력으로 사용
	PORTF = 0x10;				// 처음 led가 켜지는 위치 0x10으로 설정 (LED 1 점등)
	sei();						// 인터럽트 enable
	while (1) {			// 반복

	}
	return 0;
}