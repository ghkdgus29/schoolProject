#include <avr/io.h>
#include <util/delay.h>

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};		// 점 없는거
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};		// 점 있는거
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};		// 숫자판위치

void init_port(){
	DDRA = 0xFF;		// 7segment 모두 출력으로 사용
	DDRE = 0x0C;		// led ctrl, led data 모두 enable
	PORTE = 0x04;		// led ctrl 사용
	PORTA = 0x0F;		// 숫자판 4개모두 사용
}

int main(){
	init_port();		// 초기화
	int i;
	while(1){
		for(i=0; i<10; i++){		// 숫자 0~9 
			PORTE = 0x08;				// led data 사용
			PORTA = FND_SEGWP[i];		// 0~9 표시 
			_delay_ms(500);				// 지연시간
		}
	}
	return 0;
}
