#include <avr/io.h>
#include <util/delay.h>

unsigned char FND_SEGNP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};
unsigned char FND_SEGWP[10] = {0xBF, 0x86, 0xDB, 0xCF, 0XE6, 0XED, 0xFD, 0xA7, 0xFF, 0xEF};
unsigned char FND_SEGPOS[4] = {0x01, 0x02, 0x04, 0x08};

void init_port(){
	DDRA = 0xFF;
	DDRE = 0x0C;
	PORTE = 0x04;
	PORTA = 0x0F;
}

int main(void){
	init_port();
	int j;
	int k=70;
	for(j=0; j<4; j++){
		PORTA = FND_SEGPOS[j];
		PORTE = 0x04;
		PORTE = 0x00;
		PORTA = FND_SEGNP[j];
		PORTE = 0x08;
		PORTE = 0x00;
		_delay_ms(1000);
	}

	while(1){
		for(j=0; j<4; j++){
			PORTA = FND_SEGPOS[j];
			PORTE = 0x04;
			PORTE = 0x00;
			PORTA = FND_SEGNP[j];
			PORTE = 0x08;
			PORTE = 0x00;
			_delay_ms(k);
		}
		if(k>=2) 
			k=k-1;

	}
	return 0;
}
