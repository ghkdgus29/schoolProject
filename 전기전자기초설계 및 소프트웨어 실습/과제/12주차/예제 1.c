#include <avr/io.h>					

void init_uart(void){			// UART ����� ���� ����
	DDRE = 0x02;				// TX ���, RX �Է����� ���
	
	UBRR0H = 0;
	UBRR0L = 103;				// Baud rate = 9600BPS �� ����
	
	UCSR0B = 0x18;				// �ۼ��ź� enable	

	UCSR0C = 0x06;				// 8bit data
}

int main(){
	init_uart();				// UART ��� �ʱ�ȭ
	char rx, tx;				// char �ڷ��� rx, tx ���� ����

	while(1){
		while((UCSR0A & 0x80) == 0x00);			// ���� ���ۿ� ���� ���� ���� ������ �����ִٸ� ��� �о����
		rx = UDR0;							// ���Ź��ۿ��� ������ �� �о���̸� �о���� �����͸� rx�� ����
		tx = rx;							// rx���� tx�� ����
	

		while((UCSR0A & 0x20) == 0x00);			// �۽� ���ۿ� ���� �������� ���� ������ �����ִٸ� ��� ����
		UDR0 = tx;							// ������ �������Ϳ� tx�� ����
	}
	return 0;			
}
