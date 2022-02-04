#include <avr/io.h>

void init_uart(void){			// UART 통신을 위한 설정
	DDRE = 0x02;				// TX 출력, RX 입력으로 사용
	
	UBRR0H = 0;
	UBRR0L = 103;				// Baud rate = 9600BPS 로 설정
	
	UCSR0B = 0x18;				// 송수신부 enable	

	UCSR0C = 0x06;				// 8bit data
}

int main(){
	init_uart();				// UART 통신 초기화
	char rx, tx;				// char 자료형 rx, tx 변수 선언

	while(1){
		while((UCSR0A & 0x80) == 0x00);					// 수신 버퍼에 아직 읽지 못한 정보가 남아있다면 계속 읽어들임
		rx = UDR0;									// 수신버퍼에서 정보를 다 읽어들이면 읽어들인 데이터를 rx에 저장
		if((rx>=65) & (rx<=95)) rx+=32;				// 만약 rx값이 (읽어들인 데이터값) 알파벳 대문자이면 32를 더해 알파벳 소문자로 만든다.
		else if((rx>=97) & (rx <=127)) rx-=32;		// 만약 rx값이 (읽어들인 데이터값) 알파벳 소문자이면 32를 빼 알파벳 대문자로 만든다.
		tx = rx;							// rx값을 tx에 저장한다.
	

		while((UCSR0A & 0x20) == 0x00);				// 송신 버퍼에 아직 전달하지 못한 정보가 남아있다면 계속 전달
		UDR0 = tx;							// 데이터 레지스터에 tx값 전달
	}
	return 0;			
}
