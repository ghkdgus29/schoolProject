#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

unsigned char FND_SEGPOS[4] = { 0X01,0X02,0X04,0X08 };                      // led 점등 위치배열
unsigned char FND_NHU[4] = { 0X54, 0X5C,0X76,0X3E };                       // 사람SKIN이 아님을 확인하기 위한 'noHU' 세그먼트 배열
unsigned char FND_HU[2] = { 0X76,0X3E };                                    // 사람SKIN RGB 경우 사람을 뜻하는 'HU' 세그먼트 배열   


volatile int temp = 0, temp1 = 0, a = 3;                                    // 타이머 변수, 인터럽트가 일어나면 증가
int t = 7812;                                                           // 8분주로 scaling , temp == t 가 되면 1초를 의미
int stopgo = 0;                                                         // stopgo 변수, 1이면 타이머 인터럽트 동작 수행, 0이면 정지
int running_time = 0;                                                   // 타이머 인터럽트를 통해 running_time 변수를 증가시켜 7segment display 시간을 설정


int getN = 0;                                               // get()을 통해 RGB값을 받아오기 전엔 0, RGB값을 받아오면 1로 설정됨
char buffer[5];                                             // putty를 통해 입력받은 ascii값을 잠깐 저장하기 위한 버퍼 배열
int R, G, B;                                                // RGB 변수
int flag;

char red[100] = "input red : ";
char green[100] = "input green : ";
char blue[100] = "input blue : ";                   // putty 상에 커서 출력을 위한 문자열
char wrong[100] = "wrong number !!";                // 만약 RGB 변수의 입력에서 오류가 있었다면 출력

void init_port()                                    // LED 관련 레지스터 설정
{
    DDRA = 0xFF;                                    // 7segment 모두 출력으로 사용
    DDRE = 0x0E;                                    // UART 통신, led ctrl, led data 모두 사용
    PORTE = 0x04;                                   // led ctrl 먼저 사용
    PORTA = 0x0F;                                   // 숫자판 4개 모두 출력으로 사용
    DDRG = 0x00;

}

void  init_sound()                  // 외부 회로 동작을 위한 초기값 세팅
{
    DDRD = 0XFF;                    // 미사용 GPIO(D PORT)를 활용, 모두 출력으로 사용, 사운드 사용예정
    PORTD = 0X00;                   // D PORT 00초기화(사운드 없음)
}

void timer_setting() {                  // 타이머 관련 레지스터 설정
    TCCR0 = _BV(CS01);              // 8분주로 설정
    TCNT0 = 0x00;                   // 인터럽트 발생시 다시 0으로 초기화
    TIMSK = 0x01;                   // 오버플로우 인터럽트 사용
    TIFR = 0xFF;                    // flag 초기화
}

void init_interrupt(void) {			// 인터럽트 관련 설정 초기화
    EIMSK = 0x00;				// 0으로 초기화
    EICRA = 0x00;
    EICRB = 0x02;				// 외부 인터럽트 4번을 falling edge로 사용
    EIMSK = 0x10;				// 외부 인터럽트 4번enable
    EIFR = 0xFF;			// 인터럽트 플래그 초기화
}

void init_uart(void) {              // UART 통신 관련 설정
    UBRR0H = 0;
    UBRR0L = 103;                   // Baud rate 설정

    UCSR0B = 0x18;                  // 통신, 수신 설정

    UCSR0C = 0x06;                  // 데이터 타입 설정
}

float compute_kernel(float* x, float data1, float data2, float data3) {

    float kernel = 0.0;
    kernel += x[0] * data1;
    kernel += x[1] * data2;
    kernel += x[2] * data3;                     //트레이닝을 통해 생성된 RGB 모델값들과 실제 입력 RGB데이터 비교

    return kernel;
}

void put_string(char string[]) {					// 스트링 출력, R,G,B 입력창 표시를 위해 사용
    int cnt = 0;
    char tx;

    while (1) {
        tx = string[cnt++];                         // 스트링을 putty로 전달하기 위해 tx변수에 전달한다.

        while ((UCSR0A & 0x20) == 0x00) {}
        UDR0 = tx;                                  // tx변수를 putty에 출력, 즉 스트링을 putty상에 출력한다.

        if (string[cnt] == 0) break;            // 만약 NULL 문자, 즉 입력이 끝나면 반복문을 빠져나간다.

    }
}

void get() {                                    // putty 를 통해 R, G, B 값을 입력받는 함수
    int i, cnt = 0, t = 0;
    char tx, rx;

    put_string(red);							//  R 변수 입력
    while (1) {

        while ((UCSR0A & 0x80) == 0x00) {}
        rx = UDR0;                              // 숫자 입력받음

        if (rx == 13) break;                    // 엔터누를 때 까지

        tx = rx;
        buffer[cnt++] = rx;                     // 입력받은 아스키값 버퍼에 저장, cnt는 숫자 자리수를 의미

        while ((UCSR0A & 0x20) == 0x00) {}

        UDR0 = tx;                              // echoing (입력받은값을 다시 putty에 출력)
    }
    if (cnt == 1) {                         // 한 자리수 입력 받을 시
        R = buffer[0] - 48;                 // 아스키 값을 int type으로 변환하여 R에 저장
        newline();                          // 줄 바꿈
    }
    else if (cnt == 2) {                                    // 두 자리수 입력 받을 시
        R = (buffer[0] - 48) * 10 + (buffer[1] - 48);       // 아스키 값을 int type으로 변환하여 R에 저장
        newline();                                          // 줄 바꿈
    }
    else if (cnt == 3) {                                                                   // 세 자리수 입력 받을 시
        R = (buffer[0] - 48) * 100 + (buffer[1] - 48) * 10 + (buffer[2] - 48);             // 아스키 값을 int type으로 변환하여 R에 저장
        newline();                                                                         // 줄 바꿈
    }
    else {                      // 만약 위에 해당하지 않는 입력 받을 시
        put_string(wrong);          // 잘못 입력되었음을 출력
        newline();                  // 줄 바꿈
    }

    cnt = 0;                // 숫자 자리수 초기화
    put_string(green);								// G 변수 입력							
    while (1) {

        while ((UCSR0A & 0x80) == 0x00) {}
        rx = UDR0;                              // 숫자 입력받음

        if (rx == 13) break;                    // 엔터누를 때 까지

        tx = rx;
        buffer[cnt++] = rx;                     // 입력받은 아스키값 버퍼에 저장, cnt는 숫자 자리수를 의미

        while ((UCSR0A & 0x20) == 0x00) {}

        UDR0 = tx;                              // echoing (입력받은값을 다시 putty에 출력)
    }
    if (cnt == 1) {                         // 한 자리수 입력 받을 시
        G = buffer[0] - 48;                 // 아스키 값을 int type으로 변환하여 G에 저장
        newline();                          // 줄 바꿈
    }
    else if (cnt == 2) {                                        // 두 자리수 입력 받을 시
        G = (buffer[0] - 48) * 10 + (buffer[1] - 48);           // 아스키 값을 int type으로 변환하여 G에 저장
        newline();                                              // 줄 바꿈
    }
    else if (cnt == 3) {                                                            // 세 자리수 입력 받을 시
        G = (buffer[0] - 48) * 100 + (buffer[1] - 48) * 10 + (buffer[2] - 48);      // 아스키 값을 int type으로 변환하여 G에 저장
        newline();                                                                  // 줄 바꿈
    }
    else {                      // 만약 위에 해당하지 않는 입력 받을 시
        put_string(wrong);      // 잘못 입력되었음을 출력
        newline();              // 줄 바꿈
    }


    cnt = 0;               // 숫자 자리수 초기화
    put_string(blue);								// B 변수 입력
    while (1) {

        while ((UCSR0A & 0x80) == 0x00) {}
        rx = UDR0;                              // 숫자 입력받음

        if (rx == 13) break;                    // 엔터누를 때 까지

        tx = rx;
        buffer[cnt++] = rx;                     // 입력받은 아스키값 버퍼에 저장, cnt는 숫자 자리수를 의미

        while ((UCSR0A & 0x20) == 0x00) {}

        UDR0 = tx;                              // echoing (입력받은값을 다시 putty에 출력)
    }
    if (cnt == 1) {                         // 한 자리수 입력 받을 시
        B = buffer[0] - 48;                 // 아스키 값을 int type으로 변환하여 B에 저장
        newline();                          // 줄 바꿈
    }
    else if (cnt == 2) {                                        // 두 자리수 입력 받을 시
        B = (buffer[0] - 48) * 10 + (buffer[1] - 48);           // 아스키 값을 int type으로 변환하여 B에 저장
        newline();                                              // 줄 바꿈
    }
    else if (cnt == 3) {                                                            // 세 자리수 입력 받을 시
        B = (buffer[0] - 48) * 100 + (buffer[1] - 48) * 10 + (buffer[2] - 48);      // 아스키 값을 int type으로 변환하여 B에 저장
        newline();                                                                  // 줄 바꿈
    }
    else {                      // 만약 위에 해당하지 않는 입력 받을 시
        put_string(wrong);      // 잘못 입력되었음을 출력
        newline();              // 줄 바꿈
    }
}

void newline() {								// 줄바꿈
    while ((UCSR0A & 0x20) == 0x00);
    UDR0 = '\r';						// 커서 맨앞으로 당김

    while ((UCSR0A & 0x20) == 0x00);
    UDR0 = '\n';						// 줄바꿈
}


int predict(float* x) {             // putty 입력 변수 RGB 값을 대입하여 분석 진행
    float kernels[803] = { 0, };
    kernels[0] = compute_kernel(x, 215.0, 217.0, 251.0);
    kernels[1] = compute_kernel(x, 217.0, 220.0, 251.0);
    kernels[2] = compute_kernel(x, 220.0, 222.0, 252.0);
    kernels[3] = compute_kernel(x, 221.0, 224.0, 255.0);
    kernels[4] = compute_kernel(x, 222.0, 228.0, 255.0);
    kernels[5] = compute_kernel(x, 218.0, 226.0, 255.0);
    kernels[6] = compute_kernel(x, 217.0, 223.0, 255.0);
    kernels[7] = compute_kernel(x, 221.0, 226.0, 255.0);
    kernels[8] = compute_kernel(x, 223.0, 226.0, 255.0);
    kernels[9] = compute_kernel(x, 224.0, 226.0, 255.0);
    kernels[10] = compute_kernel(x, 222.0, 225.0, 255.0);
    kernels[11] = compute_kernel(x, 221.0, 227.0, 255.0);
    kernels[12] = compute_kernel(x, 225.0, 230.0, 255.0);
    kernels[13] = compute_kernel(x, 223.0, 228.0, 255.0);
    kernels[14] = compute_kernel(x, 222.0, 225.0, 255.0);
    kernels[15] = compute_kernel(x, 218.0, 223.0, 255.0);
    kernels[16] = compute_kernel(x, 219.0, 225.0, 255.0);
    kernels[17] = compute_kernel(x, 219.0, 224.0, 255.0);
    kernels[18] = compute_kernel(x, 215.0, 220.0, 253.0);
    kernels[19] = compute_kernel(x, 213.0, 218.0, 251.0);
    kernels[20] = compute_kernel(x, 82.0, 85.0, 129.0);
    kernels[21] = compute_kernel(x, 83.0, 86.0, 130.0);
    kernels[22] = compute_kernel(x, 78.0, 81.0, 125.0);
    kernels[23] = compute_kernel(x, 75.0, 78.0, 122.0);
    kernels[24] = compute_kernel(x, 76.0, 79.0, 123.0);
    kernels[25] = compute_kernel(x, 191.0, 199.0, 246.0);
    kernels[26] = compute_kernel(x, 191.0, 199.0, 246.0);
    kernels[27] = compute_kernel(x, 197.0, 204.0, 253.0);
    kernels[28] = compute_kernel(x, 197.0, 204.0, 254.0);
    kernels[29] = compute_kernel(x, 76.0, 81.0, 120.0);
    kernels[30] = compute_kernel(x, 119.0, 174.0, 235.0);
    kernels[31] = compute_kernel(x, 121.0, 176.0, 237.0);
    kernels[32] = compute_kernel(x, 196.0, 202.0, 249.0);
    kernels[33] = compute_kernel(x, 195.0, 201.0, 248.0);
    kernels[34] = compute_kernel(x, 197.0, 203.0, 250.0);
    kernels[35] = compute_kernel(x, 191.0, 202.0, 254.0);
    kernels[36] = compute_kernel(x, 114.0, 171.0, 232.0);
    kernels[37] = compute_kernel(x, 187.0, 195.0, 248.0);
    kernels[38] = compute_kernel(x, 71.0, 80.0, 117.0);
    kernels[39] = compute_kernel(x, 122.0, 176.0, 236.0);
    kernels[40] = compute_kernel(x, 117.0, 173.0, 232.0);
    kernels[41] = compute_kernel(x, 59.0, 75.0, 141.0);
    kernels[42] = compute_kernel(x, 102.0, 103.0, 153.0);
    kernels[43] = compute_kernel(x, 103.0, 106.0, 157.0);
    kernels[44] = compute_kernel(x, 126.0, 180.0, 240.0);
    kernels[45] = compute_kernel(x, 120.0, 177.0, 232.0);
    kernels[46] = compute_kernel(x, 121.0, 179.0, 231.0);
    kernels[47] = compute_kernel(x, 122.0, 180.0, 232.0);
    kernels[48] = compute_kernel(x, 123.0, 180.0, 232.0);
    kernels[49] = compute_kernel(x, 124.0, 178.0, 231.0);
    kernels[50] = compute_kernel(x, 57.0, 71.0, 137.0);
    kernels[51] = compute_kernel(x, 55.0, 69.0, 135.0);
    kernels[52] = compute_kernel(x, 103.0, 104.0, 155.0);
    kernels[53] = compute_kernel(x, 104.0, 107.0, 158.0);
    kernels[54] = compute_kernel(x, 40.0, 58.0, 111.0);
    kernels[55] = compute_kernel(x, 102.0, 105.0, 156.0);
    kernels[56] = compute_kernel(x, 40.0, 59.0, 110.0);
    kernels[57] = compute_kernel(x, 40.0, 58.0, 111.0);
    kernels[58] = compute_kernel(x, 91.0, 95.0, 146.0);
    kernels[59] = compute_kernel(x, 103.0, 106.0, 157.0);
    kernels[60] = compute_kernel(x, 40.0, 59.0, 110.0);
    kernels[61] = compute_kernel(x, 91.0, 95.0, 146.0);
    kernels[62] = compute_kernel(x, 42.0, 63.0, 108.0);
    kernels[63] = compute_kernel(x, 27.0, 56.0, 107.0);
    kernels[64] = compute_kernel(x, 37.0, 60.0, 106.0);
    kernels[65] = compute_kernel(x, 38.0, 61.0, 106.0);
    kernels[66] = compute_kernel(x, 40.0, 63.0, 108.0);
    kernels[67] = compute_kernel(x, 146.0, 142.0, 201.0);
    kernels[68] = compute_kernel(x, 124.0, 123.0, 179.0);
    kernels[69] = compute_kernel(x, 125.0, 124.0, 180.0);
    kernels[70] = compute_kernel(x, 148.0, 142.0, 199.0);
    kernels[71] = compute_kernel(x, 126.0, 125.0, 181.0);
    kernels[72] = compute_kernel(x, 147.0, 141.0, 198.0);
    kernels[73] = compute_kernel(x, 148.0, 142.0, 199.0);
    kernels[74] = compute_kernel(x, 149.0, 143.0, 200.0);
    kernels[75] = compute_kernel(x, 99.0, 103.0, 152.0);
    kernels[76] = compute_kernel(x, 123.0, 126.0, 177.0);
    kernels[77] = compute_kernel(x, 148.0, 142.0, 199.0);
    kernels[78] = compute_kernel(x, 149.0, 143.0, 200.0);
    kernels[79] = compute_kernel(x, 150.0, 144.0, 201.0);
    kernels[80] = compute_kernel(x, 99.0, 103.0, 152.0);
    kernels[81] = compute_kernel(x, 123.0, 126.0, 177.0);
    kernels[82] = compute_kernel(x, 146.0, 143.0, 199.0);
    kernels[83] = compute_kernel(x, 149.0, 143.0, 200.0);
    kernels[84] = compute_kernel(x, 150.0, 144.0, 201.0);
    kernels[85] = compute_kernel(x, 150.0, 144.0, 201.0);
    kernels[86] = compute_kernel(x, 99.0, 103.0, 152.0);
    kernels[87] = compute_kernel(x, 145.0, 144.0, 200.0);
    kernels[88] = compute_kernel(x, 148.0, 145.0, 201.0);
    kernels[89] = compute_kernel(x, 149.0, 146.0, 202.0);
    kernels[90] = compute_kernel(x, 151.0, 145.0, 202.0);
    kernels[91] = compute_kernel(x, 99.0, 102.0, 153.0);
    kernels[92] = compute_kernel(x, 147.0, 144.0, 200.0);
    kernels[93] = compute_kernel(x, 150.0, 144.0, 201.0);
    kernels[94] = compute_kernel(x, 151.0, 145.0, 202.0);
    kernels[95] = compute_kernel(x, 151.0, 145.0, 202.0);
    kernels[96] = compute_kernel(x, 147.0, 144.0, 199.0);
    kernels[97] = compute_kernel(x, 147.0, 144.0, 199.0);
    kernels[98] = compute_kernel(x, 150.0, 145.0, 200.0);
    kernels[99] = compute_kernel(x, 151.0, 146.0, 201.0);
    kernels[100] = compute_kernel(x, 152.0, 146.0, 201.0);
    kernels[101] = compute_kernel(x, 152.0, 146.0, 201.0);
    kernels[102] = compute_kernel(x, 151.0, 145.0, 200.0);
    kernels[103] = compute_kernel(x, 146.0, 141.0, 196.0);
    kernels[104] = compute_kernel(x, 147.0, 142.0, 197.0);
    kernels[105] = compute_kernel(x, 149.0, 143.0, 198.0);
    kernels[106] = compute_kernel(x, 150.0, 144.0, 199.0);
    kernels[107] = compute_kernel(x, 153.0, 144.0, 200.0);
    kernels[108] = compute_kernel(x, 153.0, 144.0, 200.0);
    kernels[109] = compute_kernel(x, 152.0, 143.0, 199.0);
    kernels[110] = compute_kernel(x, 147.0, 141.0, 196.0);
    kernels[111] = compute_kernel(x, 148.0, 142.0, 197.0);
    kernels[112] = compute_kernel(x, 149.0, 143.0, 198.0);
    kernels[113] = compute_kernel(x, 150.0, 144.0, 199.0);
    kernels[114] = compute_kernel(x, 152.0, 143.0, 199.0);
    kernels[115] = compute_kernel(x, 148.0, 142.0, 197.0);
    kernels[116] = compute_kernel(x, 149.0, 143.0, 198.0);
    kernels[117] = compute_kernel(x, 150.0, 144.0, 199.0);
    kernels[118] = compute_kernel(x, 151.0, 145.0, 200.0);
    kernels[119] = compute_kernel(x, 146.0, 141.0, 196.0);
    kernels[120] = compute_kernel(x, 147.0, 142.0, 197.0);
    kernels[121] = compute_kernel(x, 149.0, 144.0, 199.0);
    kernels[122] = compute_kernel(x, 150.0, 145.0, 200.0);
    kernels[123] = compute_kernel(x, 150.0, 145.0, 200.0);
    kernels[124] = compute_kernel(x, 146.0, 141.0, 196.0);
    kernels[125] = compute_kernel(x, 147.0, 142.0, 197.0);
    kernels[126] = compute_kernel(x, 148.0, 143.0, 198.0);
    kernels[127] = compute_kernel(x, 149.0, 144.0, 199.0);
    kernels[128] = compute_kernel(x, 149.0, 144.0, 199.0);
    kernels[129] = compute_kernel(x, 33.0, 84.0, 146.0);
    kernels[130] = compute_kernel(x, 31.0, 82.0, 144.0);
    kernels[131] = compute_kernel(x, 32.0, 83.0, 145.0);
    kernels[132] = compute_kernel(x, 91.0, 95.0, 148.0);
    kernels[133] = compute_kernel(x, 132.0, 129.0, 184.0);
    kernels[134] = compute_kernel(x, 92.0, 96.0, 149.0);
    kernels[135] = compute_kernel(x, 132.0, 129.0, 184.0);
    kernels[136] = compute_kernel(x, 131.0, 128.0, 183.0);
    kernels[137] = compute_kernel(x, 131.0, 128.0, 183.0);
    kernels[138] = compute_kernel(x, 130.0, 127.0, 182.0);
    kernels[139] = compute_kernel(x, 93.0, 96.0, 147.0);
    kernels[140] = compute_kernel(x, 95.0, 96.0, 146.0);
    kernels[141] = compute_kernel(x, 95.0, 96.0, 146.0);
    kernels[142] = compute_kernel(x, 96.0, 100.0, 148.0);
    kernels[143] = compute_kernel(x, 95.0, 96.0, 146.0);
    kernels[144] = compute_kernel(x, 95.0, 99.0, 147.0);
    kernels[145] = compute_kernel(x, 93.0, 94.0, 144.0);
    kernels[146] = compute_kernel(x, 94.0, 98.0, 146.0);
    kernels[147] = compute_kernel(x, 92.0, 93.0, 143.0);
    kernels[148] = compute_kernel(x, 39.0, 102.0, 146.0);
    kernels[149] = compute_kernel(x, 42.0, 107.0, 151.0);
    kernels[150] = compute_kernel(x, 91.0, 92.0, 142.0);
    kernels[151] = compute_kernel(x, 114.0, 116.0, 170.0);
    kernels[152] = compute_kernel(x, 38.0, 101.0, 145.0);
    kernels[153] = compute_kernel(x, 90.0, 91.0, 141.0);
    kernels[154] = compute_kernel(x, 89.0, 93.0, 142.0);
    kernels[155] = compute_kernel(x, 91.0, 95.0, 146.0);
    kernels[156] = compute_kernel(x, 88.0, 92.0, 141.0);
    kernels[157] = compute_kernel(x, 88.0, 92.0, 141.0);
    kernels[158] = compute_kernel(x, 89.0, 93.0, 142.0);
    kernels[159] = compute_kernel(x, 87.0, 91.0, 140.0);
    kernels[160] = compute_kernel(x, 88.0, 92.0, 141.0);
    kernels[161] = compute_kernel(x, 89.0, 93.0, 142.0);
    kernels[162] = compute_kernel(x, 87.0, 91.0, 140.0);
    kernels[163] = compute_kernel(x, 88.0, 92.0, 141.0);
    kernels[164] = compute_kernel(x, 86.0, 90.0, 139.0);
    kernels[165] = compute_kernel(x, 86.0, 90.0, 139.0);
    kernels[166] = compute_kernel(x, 86.0, 90.0, 139.0);
    kernels[167] = compute_kernel(x, 117.0, 119.0, 173.0);
    kernels[168] = compute_kernel(x, 117.0, 119.0, 173.0);
    kernels[169] = compute_kernel(x, 117.0, 119.0, 173.0);
    kernels[170] = compute_kernel(x, 113.0, 115.0, 169.0);
    kernels[171] = compute_kernel(x, 113.0, 115.0, 169.0);
    kernels[172] = compute_kernel(x, 114.0, 116.0, 170.0);
    kernels[173] = compute_kernel(x, 114.0, 116.0, 170.0);
    kernels[174] = compute_kernel(x, 114.0, 116.0, 170.0);
    kernels[175] = compute_kernel(x, 113.0, 115.0, 169.0);
    kernels[176] = compute_kernel(x, 84.0, 88.0, 136.0);
    kernels[177] = compute_kernel(x, 85.0, 89.0, 138.0);
    kernels[178] = compute_kernel(x, 91.0, 94.0, 145.0);
    kernels[179] = compute_kernel(x, 91.0, 93.0, 147.0);
    kernels[180] = compute_kernel(x, 92.0, 94.0, 148.0);
    kernels[181] = compute_kernel(x, 93.0, 95.0, 149.0);
    kernels[182] = compute_kernel(x, 94.0, 96.0, 150.0);
    kernels[183] = compute_kernel(x, 95.0, 97.0, 151.0);
    kernels[184] = compute_kernel(x, 95.0, 97.0, 151.0);
    kernels[185] = compute_kernel(x, 96.0, 98.0, 152.0);
    kernels[186] = compute_kernel(x, 100.0, 102.0, 156.0);
    kernels[187] = compute_kernel(x, 102.0, 104.0, 158.0);
    kernels[188] = compute_kernel(x, 103.0, 105.0, 159.0);
    kernels[189] = compute_kernel(x, 102.0, 104.0, 158.0);
    kernels[190] = compute_kernel(x, 99.0, 101.0, 155.0);
    kernels[191] = compute_kernel(x, 96.0, 98.0, 152.0);
    kernels[192] = compute_kernel(x, 95.0, 97.0, 151.0);
    kernels[193] = compute_kernel(x, 86.0, 88.0, 136.0);
    kernels[194] = compute_kernel(x, 85.0, 89.0, 138.0);
    kernels[195] = compute_kernel(x, 88.0, 91.0, 142.0);
    kernels[196] = compute_kernel(x, 87.0, 91.0, 144.0);
    kernels[197] = compute_kernel(x, 88.0, 92.0, 145.0);
    kernels[198] = compute_kernel(x, 89.0, 93.0, 146.0);
    kernels[199] = compute_kernel(x, 90.0, 94.0, 147.0);
    kernels[200] = compute_kernel(x, 91.0, 95.0, 148.0);
    kernels[201] = compute_kernel(x, 166.0, 191.0, 253.0);
    kernels[202] = compute_kernel(x, 167.0, 188.0, 250.0);
    kernels[203] = compute_kernel(x, 161.0, 189.0, 253.0);
    kernels[204] = compute_kernel(x, 162.0, 190.0, 254.0);
    kernels[205] = compute_kernel(x, 163.0, 191.0, 255.0);
    kernels[206] = compute_kernel(x, 163.0, 191.0, 255.0);
    kernels[207] = compute_kernel(x, 144.0, 179.0, 252.0);
    kernels[208] = compute_kernel(x, 142.0, 177.0, 250.0);
    kernels[209] = compute_kernel(x, 141.0, 176.0, 249.0);
    kernels[210] = compute_kernel(x, 144.0, 179.0, 252.0);
    kernels[211] = compute_kernel(x, 143.0, 178.0, 251.0);
    kernels[212] = compute_kernel(x, 142.0, 177.0, 250.0);
    kernels[213] = compute_kernel(x, 140.0, 175.0, 248.0);
    kernels[214] = compute_kernel(x, 144.0, 181.0, 255.0);
    kernels[215] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[216] = compute_kernel(x, 140.0, 177.0, 251.0);
    kernels[217] = compute_kernel(x, 139.0, 176.0, 250.0);
    kernels[218] = compute_kernel(x, 142.0, 179.0, 253.0);
    kernels[219] = compute_kernel(x, 142.0, 179.0, 253.0);
    kernels[220] = compute_kernel(x, 141.0, 178.0, 252.0);
    kernels[221] = compute_kernel(x, 137.0, 174.0, 248.0);
    kernels[222] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[223] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[224] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[225] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[226] = compute_kernel(x, 142.0, 179.0, 253.0);
    kernels[227] = compute_kernel(x, 142.0, 179.0, 253.0);
    kernels[228] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[229] = compute_kernel(x, 143.0, 180.0, 254.0);
    kernels[230] = compute_kernel(x, 144.0, 181.0, 255.0);
    kernels[231] = compute_kernel(x, 37.0, 85.0, 151.0);
    kernels[232] = compute_kernel(x, 35.0, 83.0, 149.0);
    kernels[233] = compute_kernel(x, 36.0, 84.0, 150.0);
    kernels[234] = compute_kernel(x, 26.0, 71.0, 115.0);
    kernels[235] = compute_kernel(x, 28.0, 73.0, 116.0);
    kernels[236] = compute_kernel(x, 28.0, 73.0, 116.0);
    kernels[237] = compute_kernel(x, 149.0, 156.0, 219.0);
    kernels[238] = compute_kernel(x, 154.0, 159.0, 222.0);
    kernels[239] = compute_kernel(x, 149.0, 154.0, 215.0);
    kernels[240] = compute_kernel(x, 146.0, 151.0, 212.0);
    kernels[241] = compute_kernel(x, 173.0, 173.0, 237.0);
    kernels[242] = compute_kernel(x, 161.0, 162.0, 223.0);
    kernels[243] = compute_kernel(x, 157.0, 158.0, 219.0);
    kernels[244] = compute_kernel(x, 158.0, 159.0, 220.0);
    kernels[245] = compute_kernel(x, 167.0, 211.0, 255.0);
    kernels[246] = compute_kernel(x, 179.0, 182.0, 243.0);
    kernels[247] = compute_kernel(x, 181.0, 182.0, 243.0);
    kernels[248] = compute_kernel(x, 180.0, 181.0, 242.0);
    kernels[249] = compute_kernel(x, 180.0, 183.0, 244.0);
    kernels[250] = compute_kernel(x, 182.0, 185.0, 246.0);
    kernels[251] = compute_kernel(x, 169.0, 171.0, 235.0);
    kernels[252] = compute_kernel(x, 178.0, 186.0, 245.0);
    kernels[253] = compute_kernel(x, 138.0, 147.0, 214.0);
    kernels[254] = compute_kernel(x, 172.0, 177.0, 240.0);
    kernels[255] = compute_kernel(x, 182.0, 187.0, 248.0);
    kernels[256] = compute_kernel(x, 178.0, 186.0, 245.0);
    kernels[257] = compute_kernel(x, 143.0, 152.0, 219.0);
    kernels[258] = compute_kernel(x, 133.0, 143.0, 213.0);
    kernels[259] = compute_kernel(x, 139.0, 148.0, 215.0);
    kernels[260] = compute_kernel(x, 150.0, 157.0, 220.0);
    kernels[261] = compute_kernel(x, 150.0, 157.0, 220.0);
    kernels[262] = compute_kernel(x, 158.0, 196.0, 228.0);
    kernels[263] = compute_kernel(x, 147.0, 155.0, 215.0);
    kernels[264] = compute_kernel(x, 149.0, 157.0, 217.0);
    kernels[265] = compute_kernel(x, 142.0, 151.0, 218.0);
    kernels[266] = compute_kernel(x, 143.0, 152.0, 220.0);
    kernels[267] = compute_kernel(x, 143.0, 150.0, 205.0);
    kernels[268] = compute_kernel(x, 148.0, 155.0, 212.0);
    kernels[269] = compute_kernel(x, 153.0, 160.0, 217.0);
    kernels[270] = compute_kernel(x, 148.0, 156.0, 215.0);
    kernels[271] = compute_kernel(x, 148.0, 156.0, 216.0);
    kernels[272] = compute_kernel(x, 145.0, 154.0, 211.0);
    kernels[273] = compute_kernel(x, 149.0, 156.0, 211.0);
    kernels[274] = compute_kernel(x, 171.0, 178.0, 241.0);
    kernels[275] = compute_kernel(x, 138.0, 147.0, 214.0);
    kernels[276] = compute_kernel(x, 155.0, 161.0, 220.0);
    kernels[277] = compute_kernel(x, 144.0, 151.0, 214.0);
    kernels[278] = compute_kernel(x, 142.0, 150.0, 217.0);
    kernels[279] = compute_kernel(x, 139.0, 148.0, 215.0);
    kernels[280] = compute_kernel(x, 149.0, 156.0, 219.0);
    kernels[281] = compute_kernel(x, 142.0, 149.0, 212.0);
    kernels[282] = compute_kernel(x, 144.0, 150.0, 215.0);
    kernels[283] = compute_kernel(x, 146.0, 152.0, 217.0);
    kernels[284] = compute_kernel(x, 145.0, 151.0, 216.0);
    kernels[285] = compute_kernel(x, 139.0, 145.0, 210.0);
    kernels[286] = compute_kernel(x, 166.0, 172.0, 237.0);
    kernels[287] = compute_kernel(x, 165.0, 171.0, 236.0);
    kernels[288] = compute_kernel(x, 149.0, 155.0, 220.0);
    kernels[289] = compute_kernel(x, 145.0, 151.0, 216.0);
    kernels[290] = compute_kernel(x, 148.0, 154.0, 219.0);
    kernels[291] = compute_kernel(x, 152.0, 158.0, 223.0);
    kernels[292] = compute_kernel(x, 153.0, 159.0, 224.0);
    kernels[293] = compute_kernel(x, 139.0, 145.0, 210.0);
    kernels[294] = compute_kernel(x, 168.0, 174.0, 239.0);
    kernels[295] = compute_kernel(x, 152.0, 158.0, 223.0);
    kernels[296] = compute_kernel(x, 140.0, 146.0, 211.0);
    kernels[297] = compute_kernel(x, 144.0, 150.0, 215.0);
    kernels[298] = compute_kernel(x, 152.0, 158.0, 223.0);
    kernels[299] = compute_kernel(x, 173.0, 179.0, 244.0);
    kernels[300] = compute_kernel(x, 152.0, 158.0, 223.0);
    kernels[301] = compute_kernel(x, 141.0, 147.0, 212.0);
    kernels[302] = compute_kernel(x, 142.0, 148.0, 213.0);
    kernels[303] = compute_kernel(x, 149.0, 155.0, 220.0);
    kernels[304] = compute_kernel(x, 168.0, 177.0, 241.0);
    kernels[305] = compute_kernel(x, 155.0, 191.0, 221.0);
    kernels[306] = compute_kernel(x, 154.0, 164.0, 234.0);
    kernels[307] = compute_kernel(x, 152.0, 162.0, 232.0);
    kernels[308] = compute_kernel(x, 42.0, 114.0, 178.0);
    kernels[309] = compute_kernel(x, 41.0, 112.0, 176.0);
    kernels[310] = compute_kernel(x, 153.0, 191.0, 221.0);
    kernels[311] = compute_kernel(x, 150.0, 188.0, 218.0);
    kernels[312] = compute_kernel(x, 148.0, 186.0, 216.0);
    kernels[313] = compute_kernel(x, 148.0, 186.0, 216.0);
    kernels[314] = compute_kernel(x, 149.0, 188.0, 216.0);
    kernels[315] = compute_kernel(x, 150.0, 188.0, 218.0);
    kernels[316] = compute_kernel(x, 153.0, 192.0, 224.0);
    kernels[317] = compute_kernel(x, 56.0, 131.0, 207.0);
    kernels[318] = compute_kernel(x, 51.0, 126.0, 200.0);
    kernels[319] = compute_kernel(x, 49.0, 124.0, 198.0);
    kernels[320] = compute_kernel(x, 42.0, 110.0, 177.0);
    kernels[321] = compute_kernel(x, 42.0, 110.0, 177.0);
    kernels[322] = compute_kernel(x, 42.0, 110.0, 177.0);
    kernels[323] = compute_kernel(x, 55.0, 132.0, 205.0);
    kernels[324] = compute_kernel(x, 51.0, 126.0, 200.0);
    kernels[325] = compute_kernel(x, 51.0, 126.0, 200.0);
    kernels[326] = compute_kernel(x, 51.0, 127.0, 199.0);
    kernels[327] = compute_kernel(x, 51.0, 127.0, 199.0);
    kernels[328] = compute_kernel(x, 44.0, 116.0, 186.0);
    kernels[329] = compute_kernel(x, 42.0, 113.0, 177.0);
    kernels[330] = compute_kernel(x, 93.0, 134.0, 226.0);
    kernels[331] = compute_kernel(x, 148.0, 156.0, 223.0);
    kernels[332] = compute_kernel(x, 153.0, 174.0, 212.0);
    kernels[333] = compute_kernel(x, 149.0, 169.0, 210.0);
    kernels[334] = compute_kernel(x, 151.0, 172.0, 210.0);
    kernels[335] = compute_kernel(x, 147.0, 167.0, 208.0);
    kernels[336] = compute_kernel(x, 151.0, 168.0, 211.0);
    kernels[337] = compute_kernel(x, 89.0, 136.0, 227.0);
    kernels[338] = compute_kernel(x, 91.0, 136.0, 227.0);
    kernels[339] = compute_kernel(x, 80.0, 131.0, 223.0);
    kernels[340] = compute_kernel(x, 82.0, 130.0, 225.0);
    kernels[341] = compute_kernel(x, 176.0, 185.0, 248.0);
    kernels[342] = compute_kernel(x, 161.0, 169.0, 236.0);
    kernels[343] = compute_kernel(x, 140.0, 161.0, 193.0);
    kernels[344] = compute_kernel(x, 163.0, 175.0, 217.0);
    kernels[345] = compute_kernel(x, 161.0, 173.0, 215.0);
    kernels[346] = compute_kernel(x, 161.0, 173.0, 215.0);
    kernels[347] = compute_kernel(x, 164.0, 176.0, 218.0);
    kernels[348] = compute_kernel(x, 137.0, 159.0, 194.0);
    kernels[349] = compute_kernel(x, 144.0, 169.0, 195.0);
    kernels[350] = compute_kernel(x, 140.0, 164.0, 188.0);
    kernels[351] = compute_kernel(x, 137.0, 160.0, 186.0);
    kernels[352] = compute_kernel(x, 138.0, 164.0, 194.0);
    kernels[353] = compute_kernel(x, 135.0, 163.0, 180.0);
    kernels[354] = compute_kernel(x, 131.0, 157.0, 174.0);
    kernels[355] = compute_kernel(x, 170.0, 191.0, 252.0);
    kernels[356] = compute_kernel(x, 170.0, 191.0, 253.0);
    kernels[357] = compute_kernel(x, 167.0, 188.0, 250.0);
    kernels[358] = compute_kernel(x, 132.0, 159.0, 180.0);
    kernels[359] = compute_kernel(x, 133.0, 157.0, 177.0);
    kernels[360] = compute_kernel(x, 132.0, 152.0, 177.0);
    kernels[361] = compute_kernel(x, 92.0, 119.0, 140.0);
    kernels[362] = compute_kernel(x, 129.0, 151.0, 169.0);
    kernels[363] = compute_kernel(x, 125.0, 144.0, 165.0);
    kernels[364] = compute_kernel(x, 128.0, 144.0, 167.0);
    kernels[365] = compute_kernel(x, 124.0, 143.0, 164.0);
    kernels[366] = compute_kernel(x, 125.0, 143.0, 166.0);
    kernels[367] = compute_kernel(x, 110.0, 128.0, 151.0);
    kernels[368] = compute_kernel(x, 122.0, 139.0, 160.0);
    kernels[369] = compute_kernel(x, 123.0, 140.0, 159.0);
    kernels[370] = compute_kernel(x, 123.0, 140.0, 159.0);
    kernels[371] = compute_kernel(x, 124.0, 141.0, 162.0);
    kernels[372] = compute_kernel(x, 109.0, 128.0, 149.0);
    kernels[373] = compute_kernel(x, 120.0, 137.0, 156.0);
    kernels[374] = compute_kernel(x, 123.0, 138.0, 157.0);
    kernels[375] = compute_kernel(x, 122.0, 139.0, 158.0);
    kernels[376] = compute_kernel(x, 123.0, 140.0, 161.0);
    kernels[377] = compute_kernel(x, 116.0, 135.0, 156.0);
    kernels[378] = compute_kernel(x, 100.0, 160.0, 230.0);
    kernels[379] = compute_kernel(x, 100.0, 160.0, 230.0);
    kernels[380] = compute_kernel(x, 118.0, 143.0, 223.0);
    kernels[381] = compute_kernel(x, 123.0, 146.0, 226.0);
    kernels[382] = compute_kernel(x, 123.0, 146.0, 226.0);
    kernels[383] = compute_kernel(x, 129.0, 150.0, 231.0);
    kernels[384] = compute_kernel(x, 132.0, 151.0, 232.0);
    kernels[385] = compute_kernel(x, 140.0, 158.0, 235.0);
    kernels[386] = compute_kernel(x, 137.0, 154.0, 233.0);
    kernels[387] = compute_kernel(x, 144.0, 163.0, 238.0);
    kernels[388] = compute_kernel(x, 152.0, 171.0, 239.0);
    kernels[389] = compute_kernel(x, 152.0, 170.0, 241.0);
    kernels[390] = compute_kernel(x, 151.0, 169.0, 240.0);
    kernels[391] = compute_kernel(x, 149.0, 166.0, 239.0);
    kernels[392] = compute_kernel(x, 147.0, 164.0, 237.0);
    kernels[393] = compute_kernel(x, 143.0, 159.0, 235.0);
    kernels[394] = compute_kernel(x, 140.0, 156.0, 232.0);
    kernels[395] = compute_kernel(x, 157.0, 179.0, 244.0);
    kernels[396] = compute_kernel(x, 155.0, 177.0, 242.0);
    kernels[397] = compute_kernel(x, 106.0, 133.0, 214.0);
    kernels[398] = compute_kernel(x, 106.0, 133.0, 214.0);
    kernels[399] = compute_kernel(x, 158.0, 176.0, 241.0);
    kernels[400] = compute_kernel(x, 157.0, 177.0, 242.0);
    kernels[401] = compute_kernel(x, 155.0, 161.0, 206.0);
    kernels[402] = compute_kernel(x, 148.0, 154.0, 213.0);
    kernels[403] = compute_kernel(x, 146.0, 157.0, 209.0);
    kernels[404] = compute_kernel(x, 146.0, 157.0, 209.0);
    kernels[405] = compute_kernel(x, 133.0, 142.0, 229.0);
    kernels[406] = compute_kernel(x, 147.0, 153.0, 212.0);
    kernels[407] = compute_kernel(x, 147.0, 151.0, 209.0);
    kernels[408] = compute_kernel(x, 150.0, 157.0, 206.0);
    kernels[409] = compute_kernel(x, 160.0, 157.0, 212.0);
    kernels[410] = compute_kernel(x, 145.0, 151.0, 224.0);
    kernels[411] = compute_kernel(x, 134.0, 143.0, 230.0);
    kernels[412] = compute_kernel(x, 151.0, 153.0, 218.0);
    kernels[413] = compute_kernel(x, 154.0, 161.0, 210.0);
    kernels[414] = compute_kernel(x, 149.0, 153.0, 211.0);
    kernels[415] = compute_kernel(x, 144.0, 150.0, 209.0);
    kernels[416] = compute_kernel(x, 148.0, 156.0, 209.0);
    kernels[417] = compute_kernel(x, 145.0, 156.0, 208.0);
    kernels[418] = compute_kernel(x, 148.0, 154.0, 213.0);
    kernels[419] = compute_kernel(x, 155.0, 161.0, 206.0);
    kernels[420] = compute_kernel(x, 172.0, 211.0, 255.0);
    kernels[421] = compute_kernel(x, 42.0, 117.0, 195.0);
    kernels[422] = compute_kernel(x, 27.0, 95.0, 130.0);
    kernels[423] = compute_kernel(x, 81.0, 81.0, 145.0);
    kernels[424] = compute_kernel(x, 42.0, 52.0, 116.0);
    kernels[425] = compute_kernel(x, 108.0, 130.0, 155.0);
    kernels[426] = compute_kernel(x, 121.0, 163.0, 206.0);
    kernels[427] = compute_kernel(x, 74.0, 134.0, 240.0);
    kernels[428] = compute_kernel(x, 12.0, 56.0, 127.0);
    kernels[429] = compute_kernel(x, 148.0, 194.0, 228.0);
    kernels[430] = compute_kernel(x, 96.0, 131.0, 228.0);
    kernels[431] = compute_kernel(x, 54.0, 68.0, 134.0);
    kernels[432] = compute_kernel(x, 113.0, 173.0, 255.0);
    kernels[433] = compute_kernel(x, 112.0, 169.0, 255.0);
    kernels[434] = compute_kernel(x, 114.0, 171.0, 255.0);
    kernels[435] = compute_kernel(x, 46.0, 121.0, 219.0);
    kernels[436] = compute_kernel(x, 64.0, 85.0, 140.0);
    kernels[437] = compute_kernel(x, 85.0, 107.0, 118.0);
    kernels[438] = compute_kernel(x, 60.0, 87.0, 191.0);
    kernels[439] = compute_kernel(x, 40.0, 76.0, 152.0);
    kernels[440] = compute_kernel(x, 109.0, 167.0, 202.0);
    kernels[441] = compute_kernel(x, 199.0, 251.0, 255.0);
    kernels[442] = compute_kernel(x, 228.0, 244.0, 255.0);
    kernels[443] = compute_kernel(x, 63.0, 119.0, 230.0);
    kernels[444] = compute_kernel(x, 33.0, 78.0, 145.0);
    kernels[445] = compute_kernel(x, 190.0, 219.0, 233.0);
    kernels[446] = compute_kernel(x, 35.0, 65.0, 152.0);
    kernels[447] = compute_kernel(x, 171.0, 184.0, 230.0);
    kernels[448] = compute_kernel(x, 183.0, 187.0, 255.0);
    kernels[449] = compute_kernel(x, 182.0, 196.0, 255.0);
    kernels[450] = compute_kernel(x, 180.0, 203.0, 255.0);
    kernels[451] = compute_kernel(x, 163.0, 190.0, 255.0);
    kernels[452] = compute_kernel(x, 124.0, 155.0, 255.0);
    kernels[453] = compute_kernel(x, 36.0, 40.0, 118.0);
    kernels[454] = compute_kernel(x, 125.0, 114.0, 186.0);
    kernels[455] = compute_kernel(x, 211.0, 201.0, 255.0);
    kernels[456] = compute_kernel(x, 200.0, 195.0, 255.0);
    kernels[457] = compute_kernel(x, 190.0, 189.0, 255.0);
    kernels[458] = compute_kernel(x, 163.0, 182.0, 255.0);
    kernels[459] = compute_kernel(x, 143.0, 177.0, 255.0);
    kernels[460] = compute_kernel(x, 190.0, 189.0, 255.0);
    kernels[461] = compute_kernel(x, 176.0, 182.0, 255.0);
    kernels[462] = compute_kernel(x, 180.0, 187.0, 255.0);
    kernels[463] = compute_kernel(x, 182.0, 185.0, 255.0);
    kernels[464] = compute_kernel(x, 176.0, 185.0, 255.0);
    kernels[465] = compute_kernel(x, 165.0, 183.0, 255.0);
    kernels[466] = compute_kernel(x, 140.0, 165.0, 255.0);
    kernels[467] = compute_kernel(x, 182.0, 184.0, 255.0);
    kernels[468] = compute_kernel(x, 175.0, 186.0, 255.0);
    kernels[469] = compute_kernel(x, 160.0, 186.0, 255.0);
    kernels[470] = compute_kernel(x, 114.0, 171.0, 255.0);
    kernels[471] = compute_kernel(x, 218.0, 228.0, 238.0);
    kernels[472] = compute_kernel(x, 189.0, 188.0, 255.0);
    kernels[473] = compute_kernel(x, 158.0, 112.0, 201.0);
    kernels[474] = compute_kernel(x, 169.0, 113.0, 190.0);
    kernels[475] = compute_kernel(x, 105.0, 122.0, 225.0);
    kernels[476] = compute_kernel(x, 104.0, 121.0, 224.0);
    kernels[477] = compute_kernel(x, 105.0, 122.0, 225.0);
    kernels[478] = compute_kernel(x, 94.0, 91.0, 147.0);
    kernels[479] = compute_kernel(x, 94.0, 91.0, 147.0);
    kernels[480] = compute_kernel(x, 96.0, 93.0, 149.0);
    kernels[481] = compute_kernel(x, 97.0, 94.0, 150.0);
    kernels[482] = compute_kernel(x, 97.0, 94.0, 150.0);
    kernels[483] = compute_kernel(x, 95.0, 92.0, 148.0);
    kernels[484] = compute_kernel(x, 96.0, 93.0, 149.0);
    kernels[485] = compute_kernel(x, 97.0, 94.0, 150.0);
    kernels[486] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[487] = compute_kernel(x, 103.0, 98.0, 159.0);
    kernels[488] = compute_kernel(x, 102.0, 97.0, 158.0);
    kernels[489] = compute_kernel(x, 87.0, 84.0, 140.0);
    kernels[490] = compute_kernel(x, 89.0, 86.0, 142.0);
    kernels[491] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[492] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[493] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[494] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[495] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[496] = compute_kernel(x, 105.0, 100.0, 161.0);
    kernels[497] = compute_kernel(x, 104.0, 99.0, 160.0);
    kernels[498] = compute_kernel(x, 103.0, 98.0, 159.0);
    kernels[499] = compute_kernel(x, 87.0, 84.0, 140.0);
    kernels[500] = compute_kernel(x, 89.0, 86.0, 142.0);
    kernels[501] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[502] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[503] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[504] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[505] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[506] = compute_kernel(x, 94.0, 91.0, 147.0);
    kernels[507] = compute_kernel(x, 105.0, 100.0, 161.0);
    kernels[508] = compute_kernel(x, 104.0, 99.0, 160.0);
    kernels[509] = compute_kernel(x, 103.0, 98.0, 159.0);
    kernels[510] = compute_kernel(x, 88.0, 85.0, 141.0);
    kernels[511] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[512] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[513] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[514] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[515] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[516] = compute_kernel(x, 93.0, 90.0, 146.0);
    kernels[517] = compute_kernel(x, 95.0, 92.0, 148.0);
    kernels[518] = compute_kernel(x, 102.0, 97.0, 158.0);
    kernels[519] = compute_kernel(x, 102.0, 97.0, 158.0);
    kernels[520] = compute_kernel(x, 103.0, 98.0, 159.0);
    kernels[521] = compute_kernel(x, 103.0, 98.0, 159.0);
    kernels[522] = compute_kernel(x, 88.0, 85.0, 141.0);
    kernels[523] = compute_kernel(x, 90.0, 87.0, 143.0);
    kernels[524] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[525] = compute_kernel(x, 92.0, 89.0, 145.0);
    kernels[526] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[527] = compute_kernel(x, 91.0, 88.0, 144.0);
    kernels[528] = compute_kernel(x, 93.0, 90.0, 146.0);
    kernels[529] = compute_kernel(x, 95.0, 92.0, 148.0);
    kernels[530] = compute_kernel(x, 102.0, 97.0, 158.0);
    kernels[531] = compute_kernel(x, 130.0, 159.0, 143.0);
    kernels[532] = compute_kernel(x, 68.0, 69.0, 130.0);
    kernels[533] = compute_kernel(x, 81.0, 75.0, 130.0);
    kernels[534] = compute_kernel(x, 147.0, 156.0, 194.0);
    kernels[535] = compute_kernel(x, 152.0, 152.0, 200.0);
    kernels[536] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[537] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[538] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[539] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[540] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[541] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[542] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[543] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[544] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[545] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[546] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[547] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[548] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[549] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[550] = compute_kernel(x, 151.0, 154.0, 199.0);
    kernels[551] = compute_kernel(x, 99.0, 97.0, 143.0);
    kernels[552] = compute_kernel(x, 103.0, 103.0, 143.0);
    kernels[553] = compute_kernel(x, 106.0, 109.0, 154.0);
    kernels[554] = compute_kernel(x, 106.0, 112.0, 155.0);
    kernels[555] = compute_kernel(x, 108.0, 115.0, 154.0);
    kernels[556] = compute_kernel(x, 153.0, 150.0, 200.0);
    kernels[557] = compute_kernel(x, 109.0, 118.0, 152.0);
    kernels[558] = compute_kernel(x, 206.0, 195.0, 245.0);
    kernels[559] = compute_kernel(x, 154.0, 150.0, 202.0);
    kernels[560] = compute_kernel(x, 111.0, 118.0, 151.0);
    kernels[561] = compute_kernel(x, 112.0, 117.0, 148.0);
    kernels[562] = compute_kernel(x, 114.0, 113.0, 157.0);
    kernels[563] = compute_kernel(x, 114.0, 112.0, 158.0);
    kernels[564] = compute_kernel(x, 195.0, 184.0, 230.0);
    kernels[565] = compute_kernel(x, 197.0, 188.0, 231.0);
    kernels[566] = compute_kernel(x, 199.0, 192.0, 235.0);
    kernels[567] = compute_kernel(x, 153.0, 150.0, 199.0);
    kernels[568] = compute_kernel(x, 203.0, 196.0, 241.0);
    kernels[569] = compute_kernel(x, 153.0, 149.0, 201.0);
    kernels[570] = compute_kernel(x, 206.0, 197.0, 248.0);
    kernels[571] = compute_kernel(x, 154.0, 151.0, 201.0);
    kernels[572] = compute_kernel(x, 80.0, 72.0, 102.0);
    kernels[573] = compute_kernel(x, 46.0, 68.0, 96.0);
    kernels[574] = compute_kernel(x, 150.0, 209.0, 255.0);
    kernels[575] = compute_kernel(x, 130.0, 209.0, 255.0);
    kernels[576] = compute_kernel(x, 135.0, 191.0, 216.0);
    kernels[577] = compute_kernel(x, 131.0, 185.0, 218.0);
    kernels[578] = compute_kernel(x, 104.0, 104.0, 144.0);
    kernels[579] = compute_kernel(x, 125.0, 184.0, 234.0);
    kernels[580] = compute_kernel(x, 134.0, 179.0, 240.0);
    kernels[581] = compute_kernel(x, 140.0, 175.0, 241.0);
    kernels[582] = compute_kernel(x, 146.0, 171.0, 243.0);
    kernels[583] = compute_kernel(x, 151.0, 171.0, 248.0);
    kernels[584] = compute_kernel(x, 151.0, 170.0, 251.0);
    kernels[585] = compute_kernel(x, 224.0, 227.0, 248.0);
    kernels[586] = compute_kernel(x, 222.0, 229.0, 246.0);
    kernels[587] = compute_kernel(x, 224.0, 228.0, 247.0);
    kernels[588] = compute_kernel(x, 228.0, 224.0, 253.0);
    kernels[589] = compute_kernel(x, 225.0, 223.0, 253.0);
    kernels[590] = compute_kernel(x, 226.0, 222.0, 255.0);
    kernels[591] = compute_kernel(x, 226.0, 222.0, 255.0);
    kernels[592] = compute_kernel(x, 226.0, 222.0, 255.0);
    kernels[593] = compute_kernel(x, 226.0, 223.0, 255.0);
    kernels[594] = compute_kernel(x, 226.0, 223.0, 255.0);
    kernels[595] = compute_kernel(x, 224.0, 224.0, 254.0);
    kernels[596] = compute_kernel(x, 224.0, 225.0, 253.0);
    kernels[597] = compute_kernel(x, 224.0, 225.0, 253.0);
    kernels[598] = compute_kernel(x, 224.0, 224.0, 254.0);
    kernels[599] = compute_kernel(x, 222.0, 224.0, 255.0);
    kernels[600] = compute_kernel(x, 219.0, 219.0, 255.0);
    kernels[601] = compute_kernel(x, 214.0, 213.0, 252.0);
    kernels[602] = compute_kernel(x, 152.0, 150.0, 196.0);
    kernels[603] = compute_kernel(x, 120.0, 186.0, 234.0);
    kernels[604] = compute_kernel(x, 132.0, 180.0, 238.0);
    kernels[605] = compute_kernel(x, 141.0, 175.0, 241.0);
    kernels[606] = compute_kernel(x, 152.0, 170.0, 247.0);
    kernels[607] = compute_kernel(x, 155.0, 168.0, 246.0);
    kernels[608] = compute_kernel(x, 153.0, 172.0, 245.0);
    kernels[609] = compute_kernel(x, 151.0, 172.0, 247.0);
    kernels[610] = compute_kernel(x, 121.0, 185.0, 233.0);
    kernels[611] = compute_kernel(x, 135.0, 179.0, 238.0);
    kernels[612] = compute_kernel(x, 147.0, 173.0, 243.0);
    kernels[613] = compute_kernel(x, 154.0, 173.0, 241.0);
    kernels[614] = compute_kernel(x, 151.0, 173.0, 244.0);
    kernels[615] = compute_kernel(x, 149.0, 170.0, 252.0);
    kernels[616] = compute_kernel(x, 68.0, 89.0, 104.0);
    kernels[617] = compute_kernel(x, 119.0, 184.0, 228.0);
    kernels[618] = compute_kernel(x, 134.0, 179.0, 236.0);
    kernels[619] = compute_kernel(x, 147.0, 174.0, 241.0);
    kernels[620] = compute_kernel(x, 159.0, 169.0, 247.0);
    kernels[621] = compute_kernel(x, 163.0, 169.0, 246.0);
    kernels[622] = compute_kernel(x, 156.0, 174.0, 239.0);
    kernels[623] = compute_kernel(x, 151.0, 175.0, 241.0);
    kernels[624] = compute_kernel(x, 149.0, 170.0, 251.0);
    kernels[625] = compute_kernel(x, 20.0, 52.0, 101.0);
    kernels[626] = compute_kernel(x, 115.0, 184.0, 227.0);
    kernels[627] = compute_kernel(x, 132.0, 179.0, 235.0);
    kernels[628] = compute_kernel(x, 145.0, 174.0, 241.0);
    kernels[629] = compute_kernel(x, 155.0, 172.0, 239.0);
    kernels[630] = compute_kernel(x, 150.0, 173.0, 241.0);
    kernels[631] = compute_kernel(x, 149.0, 170.0, 252.0);
    kernels[632] = compute_kernel(x, 42.0, 89.0, 111.0);
    kernels[633] = compute_kernel(x, 128.0, 178.0, 238.0);
    kernels[634] = compute_kernel(x, 141.0, 173.0, 244.0);
    kernels[635] = compute_kernel(x, 153.0, 171.0, 242.0);
    kernels[636] = compute_kernel(x, 150.0, 172.0, 244.0);
    kernels[637] = compute_kernel(x, 24.0, 54.0, 111.0);
    kernels[638] = compute_kernel(x, 35.0, 72.0, 100.0);
    kernels[639] = compute_kernel(x, 65.0, 108.0, 125.0);
    kernels[640] = compute_kernel(x, 134.0, 179.0, 236.0);
    kernels[641] = compute_kernel(x, 148.0, 174.0, 241.0);
    kernels[642] = compute_kernel(x, 156.0, 169.0, 248.0);
    kernels[643] = compute_kernel(x, 155.0, 168.0, 246.0);
    kernels[644] = compute_kernel(x, 154.0, 170.0, 247.0);
    kernels[645] = compute_kernel(x, 37.0, 68.0, 99.0);
    kernels[646] = compute_kernel(x, 83.0, 124.0, 139.0);
    kernels[647] = compute_kernel(x, 107.0, 180.0, 230.0);
    kernels[648] = compute_kernel(x, 152.0, 184.0, 233.0);
    kernels[649] = compute_kernel(x, 162.0, 173.0, 230.0);
    kernels[650] = compute_kernel(x, 161.0, 171.0, 241.0);
    kernels[651] = compute_kernel(x, 150.0, 166.0, 242.0);
    kernels[652] = compute_kernel(x, 155.0, 174.0, 255.0);
    kernels[653] = compute_kernel(x, 39.0, 73.0, 102.0);
    kernels[654] = compute_kernel(x, 157.0, 183.0, 220.0);
    kernels[655] = compute_kernel(x, 179.0, 183.0, 224.0);
    kernels[656] = compute_kernel(x, 167.0, 179.0, 233.0);
    kernels[657] = compute_kernel(x, 159.0, 171.0, 235.0);
    kernels[658] = compute_kernel(x, 161.0, 170.0, 238.0);
    kernels[659] = compute_kernel(x, 163.0, 175.0, 241.0);
    kernels[660] = compute_kernel(x, 157.0, 178.0, 233.0);
    kernels[661] = compute_kernel(x, 151.0, 173.0, 244.0);
    kernels[662] = compute_kernel(x, 30.0, 64.0, 94.0);
    kernels[663] = compute_kernel(x, 72.0, 116.0, 129.0);
    kernels[664] = compute_kernel(x, 147.0, 174.0, 195.0);
    kernels[665] = compute_kernel(x, 173.0, 187.0, 216.0);
    kernels[666] = compute_kernel(x, 165.0, 182.0, 215.0);
    kernels[667] = compute_kernel(x, 169.0, 184.0, 216.0);
    kernels[668] = compute_kernel(x, 167.0, 188.0, 216.0);
    kernels[669] = compute_kernel(x, 158.0, 185.0, 211.0);
    kernels[670] = compute_kernel(x, 142.0, 169.0, 220.0);
    kernels[671] = compute_kernel(x, 80.0, 114.0, 137.0);
    kernels[672] = compute_kernel(x, 61.0, 86.0, 120.0);
    kernels[673] = compute_kernel(x, 23.0, 45.0, 117.0);
    kernels[674] = compute_kernel(x, 77.0, 113.0, 137.0);
    kernels[675] = compute_kernel(x, 69.0, 119.0, 137.0);
    kernels[676] = compute_kernel(x, 36.0, 44.0, 97.0);
    kernels[677] = compute_kernel(x, 138.0, 145.0, 170.0);
    kernels[678] = compute_kernel(x, 131.0, 137.0, 172.0);
    kernels[679] = compute_kernel(x, 107.0, 109.0, 143.0);
    kernels[680] = compute_kernel(x, 64.0, 46.0, 107.0);
    kernels[681] = compute_kernel(x, 140.0, 147.0, 174.0);
    kernels[682] = compute_kernel(x, 132.0, 138.0, 175.0);
    kernels[683] = compute_kernel(x, 106.0, 105.0, 145.0);
    kernels[684] = compute_kernel(x, 105.0, 107.0, 141.0);
    kernels[685] = compute_kernel(x, 133.0, 135.0, 175.0);
    kernels[686] = compute_kernel(x, 130.0, 132.0, 167.0);
    kernels[687] = compute_kernel(x, 130.0, 132.0, 173.0);
    kernels[688] = compute_kernel(x, 134.0, 136.0, 170.0);
    kernels[689] = compute_kernel(x, 126.0, 126.0, 172.0);
    kernels[690] = compute_kernel(x, 133.0, 133.0, 173.0);
    kernels[691] = compute_kernel(x, 125.0, 123.0, 175.0);
    kernels[692] = compute_kernel(x, 132.0, 137.0, 170.0);
    kernels[693] = compute_kernel(x, 140.0, 194.0, 205.0);
    kernels[694] = compute_kernel(x, 135.0, 134.0, 176.0);
    kernels[695] = compute_kernel(x, 128.0, 125.0, 180.0);
    kernels[696] = compute_kernel(x, 126.0, 125.0, 167.0);
    kernels[697] = compute_kernel(x, 118.0, 122.0, 157.0);
    kernels[698] = compute_kernel(x, 128.0, 135.0, 168.0);
    kernels[699] = compute_kernel(x, 105.0, 158.0, 138.0);
    kernels[700] = compute_kernel(x, 137.0, 136.0, 178.0);
    kernels[701] = compute_kernel(x, 130.0, 128.0, 180.0);
    kernels[702] = compute_kernel(x, 127.0, 128.0, 166.0);
    kernels[703] = compute_kernel(x, 113.0, 121.0, 150.0);
    kernels[704] = compute_kernel(x, 121.0, 132.0, 160.0);
    kernels[705] = compute_kernel(x, 119.0, 120.0, 158.0);
    kernels[706] = compute_kernel(x, 141.0, 194.0, 204.0);
    kernels[707] = compute_kernel(x, 138.0, 138.0, 174.0);
    kernels[708] = compute_kernel(x, 132.0, 130.0, 176.0);
    kernels[709] = compute_kernel(x, 160.0, 206.0, 217.0);
    kernels[710] = compute_kernel(x, 152.0, 147.0, 209.0);
    kernels[711] = compute_kernel(x, 153.0, 145.0, 205.0);
    kernels[712] = compute_kernel(x, 156.0, 145.0, 201.0);
    kernels[713] = compute_kernel(x, 147.0, 143.0, 214.0);
    kernels[714] = compute_kernel(x, 154.0, 149.0, 200.0);
    kernels[715] = compute_kernel(x, 150.0, 143.0, 210.0);
    kernels[716] = compute_kernel(x, 151.0, 145.0, 210.0);
    kernels[717] = compute_kernel(x, 145.0, 140.0, 215.0);
    kernels[718] = compute_kernel(x, 156.0, 146.0, 199.0);
    kernels[719] = compute_kernel(x, 151.0, 146.0, 207.0);
    kernels[720] = compute_kernel(x, 142.0, 148.0, 213.0);
    kernels[721] = compute_kernel(x, 137.0, 146.0, 214.0);
    kernels[722] = compute_kernel(x, 143.0, 146.0, 220.0);
    kernels[723] = compute_kernel(x, 146.0, 143.0, 215.0);
    kernels[724] = compute_kernel(x, 153.0, 152.0, 218.0);
    kernels[725] = compute_kernel(x, 146.0, 145.0, 213.0);
    kernels[726] = compute_kernel(x, 111.0, 173.0, 151.0);
    kernels[727] = compute_kernel(x, 154.0, 153.0, 203.0);
    kernels[728] = compute_kernel(x, 144.0, 146.0, 211.0);
    kernels[729] = compute_kernel(x, 143.0, 147.0, 218.0);
    kernels[730] = compute_kernel(x, 143.0, 146.0, 214.0);
    kernels[731] = compute_kernel(x, 18.0, 82.0, 153.0);
    kernels[732] = compute_kernel(x, 134.0, 132.0, 192.0);
    kernels[733] = compute_kernel(x, 133.0, 135.0, 176.0);
    kernels[734] = compute_kernel(x, 150.0, 153.0, 214.0);
    kernels[735] = compute_kernel(x, 141.0, 144.0, 219.0);
    kernels[736] = compute_kernel(x, 33.0, 103.0, 170.0);
    kernels[737] = compute_kernel(x, 34.0, 118.0, 176.0);
    kernels[738] = compute_kernel(x, 141.0, 150.0, 184.0);
    kernels[739] = compute_kernel(x, 142.0, 141.0, 180.0);
    kernels[740] = compute_kernel(x, 147.0, 147.0, 207.0);
    kernels[741] = compute_kernel(x, 148.0, 150.0, 191.0);
    kernels[742] = compute_kernel(x, 30.0, 83.0, 164.0);
    kernels[743] = compute_kernel(x, 33.0, 108.0, 170.0);
    kernels[744] = compute_kernel(x, 41.0, 128.0, 184.0);
    kernels[745] = compute_kernel(x, 142.0, 145.0, 176.0);
    kernels[746] = compute_kernel(x, 151.0, 146.0, 208.0);
    kernels[747] = compute_kernel(x, 148.0, 147.0, 213.0);
    kernels[748] = compute_kernel(x, 49.0, 131.0, 196.0);
    kernels[749] = compute_kernel(x, 148.0, 146.0, 206.0);
    kernels[750] = compute_kernel(x, 154.0, 147.0, 204.0);
    kernels[751] = compute_kernel(x, 152.0, 152.0, 198.0);
    kernels[752] = compute_kernel(x, 153.0, 152.0, 216.0);
    kernels[753] = compute_kernel(x, 148.0, 147.0, 209.0);
    kernels[754] = compute_kernel(x, 123.0, 117.0, 204.0);
    kernels[755] = compute_kernel(x, 144.0, 143.0, 217.0);
    kernels[756] = compute_kernel(x, 116.0, 172.0, 153.0);
    kernels[757] = compute_kernel(x, 145.0, 143.0, 219.0);
    kernels[758] = compute_kernel(x, 140.0, 141.0, 221.0);
    kernels[759] = compute_kernel(x, 147.0, 148.0, 206.0);
    kernels[760] = compute_kernel(x, 138.0, 141.0, 222.0);
    kernels[761] = compute_kernel(x, 130.0, 144.0, 216.0);
    kernels[762] = compute_kernel(x, 137.0, 141.0, 222.0);
    kernels[763] = compute_kernel(x, 133.0, 148.0, 204.0);
    kernels[764] = compute_kernel(x, 128.0, 142.0, 224.0);
    kernels[765] = compute_kernel(x, 134.0, 150.0, 167.0);
    kernels[766] = compute_kernel(x, 142.0, 152.0, 200.0);
    kernels[767] = compute_kernel(x, 133.0, 145.0, 227.0);
    kernels[768] = compute_kernel(x, 40.0, 125.0, 203.0);
    kernels[769] = compute_kernel(x, 130.0, 147.0, 173.0);
    kernels[770] = compute_kernel(x, 146.0, 150.0, 201.0);
    kernels[771] = compute_kernel(x, 133.0, 145.0, 227.0);
    kernels[772] = compute_kernel(x, 43.0, 130.0, 210.0);
    kernels[773] = compute_kernel(x, 146.0, 148.0, 206.0);
    kernels[774] = compute_kernel(x, 133.0, 144.0, 228.0);
    kernels[775] = compute_kernel(x, 130.0, 120.0, 203.0);
    kernels[776] = compute_kernel(x, 146.0, 146.0, 210.0);
    kernels[777] = compute_kernel(x, 150.0, 145.0, 220.0);
    kernels[778] = compute_kernel(x, 149.0, 153.0, 194.0);
    kernels[779] = compute_kernel(x, 146.0, 145.0, 211.0);
    kernels[780] = compute_kernel(x, 150.0, 146.0, 218.0);
    kernels[781] = compute_kernel(x, 28.0, 96.0, 173.0);
    kernels[782] = compute_kernel(x, 44.0, 132.0, 202.0);
    kernels[783] = compute_kernel(x, 53.0, 57.0, 135.0);
    kernels[784] = compute_kernel(x, 146.0, 147.0, 208.0);
    kernels[785] = compute_kernel(x, 150.0, 146.0, 217.0);
    kernels[786] = compute_kernel(x, 43.0, 124.0, 191.0);
    kernels[787] = compute_kernel(x, 134.0, 139.0, 188.0);
    kernels[788] = compute_kernel(x, 127.0, 124.0, 174.0);
    kernels[789] = compute_kernel(x, 147.0, 150.0, 201.0);
    kernels[790] = compute_kernel(x, 151.0, 147.0, 218.0);
    kernels[791] = compute_kernel(x, 41.0, 129.0, 199.0);
    kernels[792] = compute_kernel(x, 140.0, 143.0, 198.0);
    kernels[793] = compute_kernel(x, 155.0, 150.0, 212.0);
    kernels[794] = compute_kernel(x, 147.0, 145.0, 215.0);
    kernels[795] = compute_kernel(x, 147.0, 146.0, 214.0);
    kernels[796] = compute_kernel(x, 149.0, 144.0, 205.0);
    kernels[797] = compute_kernel(x, 148.0, 151.0, 195.0);
    kernels[798] = compute_kernel(x, 149.0, 147.0, 206.0);
    kernels[799] = compute_kernel(x, 149.0, 147.0, 207.0);
    kernels[800] = compute_kernel(x, 153.0, 154.0, 204.0);
    kernels[801] = compute_kernel(x, 89.0, 91.0, 109.0);
    kernels[802] = compute_kernel(x, 85.0, 87.0, 105.0);
    float decision = 1.716607266073;
    float temp = 0.0;
    temp += +kernels[0] * -1.0 + kernels[1] * -1.0 + kernels[2] * -1.0 + kernels[3] * -1.0 + kernels[4] * -1.0 + kernels[5] * -1.0 + kernels[6] * -1.0 + kernels[7] * -1.0 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -1.0 + kernels[12] * -1.0 + kernels[13] * -1.0 + kernels[14] * -1.0 + kernels[15] * -1.0 + kernels[16] * -1.0 + kernels[17] * -1.0 + kernels[18] * -1.0 + kernels[19] * -1.0 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -1.0 + kernels[24] * -1.0 + kernels[25] * -1.0 + kernels[26] * -1.0 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -1.0 + kernels[30] * -1.0 + kernels[31] * -1.0 + kernels[32] * -1.0 + kernels[33] * -1.0 + kernels[34] * -1.0 + kernels[35] * -0.671815076305 + kernels[36] * -1.0 + kernels[37] * -1.0 + kernels[38] * -1.0 + kernels[39] * -1.0 + kernels[40] * -0.79691861657 + kernels[41] * -1.0 + kernels[42] * -1.0 + kernels[43] * -1.0 + kernels[44] * -1.0 + kernels[45] * -1.0 + kernels[46] * -1.0 + kernels[47] * -1.0 + kernels[48] * -1.0 + kernels[49] * -1.0 + kernels[50] * -1.0 + kernels[51] * -0.959211379787 + kernels[52] * -1.0 + kernels[53] * -1.0 + kernels[54] * -0.870111493538 + kernels[55] * -1.0 + kernels[56] * -1.0 + kernels[57] * -1.0 + kernels[58] * -1.0 + kernels[59] * -1.0 + kernels[60] * -1.0 + kernels[61] * -1.0 + kernels[62] * -1.0 + kernels[63] * -1.0 + kernels[64] * -1.0 + kernels[65] * -1.0 + kernels[66] * -1.0 + kernels[67] * -1.0 + kernels[68] * -1.0 + kernels[69] * -1.0 + kernels[70] * -1.0 + kernels[71] * -1.0 + kernels[72] * -1.0 + kernels[73] * -1.0 + kernels[74] * -1.0 + kernels[75] * -1.0 + kernels[76] * -1.0 + kernels[77] * -1.0 + kernels[78] * -1.0 + kernels[79] * -1.0 + kernels[80] * -1.0 + kernels[81] * -1.0 + kernels[82] * -1.0 + kernels[83] * -1.0 + kernels[84] * -1.0 + kernels[85] * -1.0 + kernels[86] * -1.0 + kernels[87] * -1.0 + kernels[88] * -1.0 + kernels[89] * -1.0 + kernels[90] * -1.0 + kernels[91] * -1.0 + kernels[92] * -1.0 + kernels[93] * -1.0;
    temp += +kernels[94] * -1.0 + kernels[95] * -1.0 + kernels[96] * -1.0 + kernels[97] * -1.0 + kernels[98] * -1.0 + kernels[99] * -1.0 + kernels[100] * -1.0 + kernels[101] * -1.0 + kernels[102] * -1.0 + kernels[103] * -1.0 + kernels[104] * -1.0 + kernels[105] * -1.0 + kernels[106] * -1.0 + kernels[107] * -1.0 + kernels[108] * -1.0 + kernels[109] * -1.0 + kernels[110] * -1.0 + kernels[111] * -1.0 + kernels[112] * -1.0 + kernels[113] * -1.0 + kernels[114] * -1.0 + kernels[115] * -1.0 + kernels[116] * -1.0 + kernels[117] * -1.0 + kernels[118] * -1.0 + kernels[119] * -1.0 + kernels[120] * -1.0 + kernels[121] * -1.0 + kernels[122] * -1.0 + kernels[123] * -1.0 + kernels[124] * -1.0 + kernels[125] * -1.0 + kernels[126] * -1.0 + kernels[127] * -1.0 + kernels[128] * -1.0 + kernels[129] * -0.861155063215 + kernels[130] * -1.0 + kernels[131] * -1.0 + kernels[132] * -1.0 + kernels[133] * -1.0 + kernels[134] * -0.296431108479 + kernels[135] * -1.0 + kernels[136] * -1.0 + kernels[137] * -1.0 + kernels[138] * -1.0 + kernels[139] * -1.0 + kernels[140] * -1.0 + kernels[141] * -1.0 + kernels[142] * -1.0 + kernels[143] * -1.0 + kernels[144] * -1.0 + kernels[145] * -1.0 + kernels[146] * -0.748578011148 + kernels[147] * -1.0 + kernels[148] * -0.085830267231 + kernels[149] * -0.16416277372 + kernels[150] * -1.0 + kernels[151] * -0.166729482257 + kernels[152] * -1.0 + kernels[153] * -1.0 + kernels[154] * -1.0 + kernels[155] * -1.0 + kernels[156] * -1.0 + kernels[157] * -1.0 + kernels[158] * -1.0 + kernels[159] * -1.0 + kernels[160] * -1.0 + kernels[161] * -0.933466682057 + kernels[162] * -1.0 + kernels[163] * -1.0 + kernels[164] * -1.0 + kernels[165] * -1.0 + kernels[166] * -1.0 + kernels[167] * -0.501185728713 + kernels[168] * -1.0 + kernels[169] * -1.0 + kernels[170] * -1.0 + kernels[171] * -1.0 + kernels[172] * -0.356097886872 + kernels[173] * -1.0 + kernels[174] * -1.0 + kernels[175] * -1.0 + kernels[176] * -0.319061130448 + kernels[177] * -1.0 + kernels[178] * -1.0 + kernels[179] * -1.0 + kernels[180] * -1.0 + kernels[181] * -1;
    temp += +kernels[182] * -1.0 + kernels[183] * -1.0 + kernels[184] * -1.0 + kernels[185] * -1.0 + kernels[186] * -1.0 + kernels[187] * -1.0 + kernels[188] * -1.0 + kernels[189] * -1.0 + kernels[190] * -1.0 + kernels[191] * -1.0 + kernels[192] * -1.0 + kernels[193] * -1.0 + kernels[194] * -1.0 + kernels[195] * -1.0 + kernels[196] * -1.0 + kernels[197] * -1.0 + kernels[198] * -1.0 + kernels[199] * -1.0 + kernels[200] * -1.0 + kernels[201] * -0.009078993207 + kernels[202] * -0.163654826501 + kernels[203] * -1.0 + kernels[204] * -1.0 + kernels[205] * -1.0 + kernels[206] * -1.0 + kernels[207] * -1.0 + kernels[208] * -1.0 + kernels[209] * -1.0 + kernels[210] * -1.0 + kernels[211] * -1.0 + kernels[212] * -1.0 + kernels[213] * -1.0 + kernels[214] * -0.005611638952 + kernels[215] * -1.0 + kernels[216] * -1.0 + kernels[217] * -1.0 + kernels[218] * -1.0 + kernels[219] * -1.0 + kernels[220] * -1.0 + kernels[221] * -1.0 + kernels[222] * -1.0 + kernels[223] * -1.0 + kernels[224] * -1.0 + kernels[225] * -1.0 + kernels[226] * -1.0 + kernels[227] * -1.0 + kernels[228] * -1.0 + kernels[229] * -1.0 + kernels[230] * -0.047012036113 + kernels[231] * -1.0 + kernels[232] * -1.0 + kernels[233] * -1.0 + kernels[234] * -1.0 + kernels[235] * -0.37770293145 + kernels[236] * -0.85733049476 + kernels[237] * -1.0 + kernels[238] * -1.0 + kernels[239] * -1.0 + kernels[240] * -1.0 + kernels[241] * -1.0 + kernels[242] * -1.0 + kernels[243] * -1.0 + kernels[244] * -1.0 + kernels[245] * -1.0 + kernels[246] * -1.0 + kernels[247] * -1.0 + kernels[248] * -1.0 + kernels[249] * -1.0 + kernels[250] * -1.0 + kernels[251] * -0.251142312665 + kernels[252] * -0.251180589993 + kernels[253] * -1.0 + kernels[254] * -1.0 + kernels[255] * -1.0 + kernels[256] * -0.98207935523 + kernels[257] * -1.0 + kernels[258] * -1.0 + kernels[259] * -1.0 + kernels[260] * -1.0 + kernels[261] * -1.0 + kernels[262] * -0.179292348635 + kernels[263] * -1.0 + kernels[264] * -1.0 + kernels[265] * -1.0 + kernels[266] * -1.0 + kernels[267] * -1.0 + kernels[268] * -1.0;
    temp += +kernels[269] * -1.0 + kernels[270] * -1.0 + kernels[271] * -1.0 + kernels[272] * -1.0 + kernels[273] * -1.0 + kernels[274] * -1.0 + kernels[275] * -1.0 + kernels[276] * -1.0 + kernels[277] * -1.0 + kernels[278] * -1.0 + kernels[279] * -1.0 + kernels[280] * -1.0 + kernels[281] * -1.0 + kernels[282] * -1.0 + kernels[283] * -1.0 + kernels[284] * -1.0 + kernels[285] * -0.995317008805 + kernels[286] * -1.0 + kernels[287] * -1.0 + kernels[288] * -1.0 + kernels[289] * -1.0 + kernels[290] * -1.0 + kernels[291] * -1.0 + kernels[292] * -1.0 + kernels[293] * -0.042639372219 + kernels[294] * -1.0 + kernels[295] * -1.0 + kernels[296] * -1.0 + kernels[297] * -1.0 + kernels[298] * -1.0 + kernels[299] * -1.0 + kernels[300] * -1.0 + kernels[301] * -1.0 + kernels[302] * -1.0 + kernels[303] * -1.0 + kernels[304] * -1.0 + kernels[305] * -1.0 + kernels[306] * -1.0 + kernels[307] * -1.0 + kernels[308] * -1.0 + kernels[309] * -1.0 + kernels[310] * -1.0 + kernels[311] * -1.0 + kernels[312] * -1.0 + kernels[313] * -1.0 + kernels[314] * -1.0 + kernels[315] * -1.0 + kernels[316] * -0.752983722548 + kernels[317] * -0.939276632585 + kernels[318] * -1.0 + kernels[319] * -1.0 + kernels[320] * -0.335064655375 + kernels[321] * -1.0 + kernels[322] * -1.0 + kernels[323] * -1.0 + kernels[324] * -1.0 + kernels[325] * -1.0 + kernels[326] * -1.0 + kernels[327] * -0.630185713827 + kernels[328] * -1.0 + kernels[329] * -1.0 + kernels[330] * -1.0 + kernels[331] * -0.637913198553 + kernels[332] * -1.0 + kernels[333] * -0.69839898414 + kernels[334] * -1.0 + kernels[335] * -1.0 + kernels[336] * -1.0 + kernels[337] * -1.0 + kernels[338] * -0.152397606114 + kernels[339] * -0.471672149928 + kernels[340] * -1.0 + kernels[341] * -1.0 + kernels[342] * -1.0 + kernels[343] * -1.0 + kernels[344] * -1.0 + kernels[345] * -1.0 + kernels[346] * -1.0 + kernels[347] * -1.0 + kernels[348] * -1.0 + kernels[349] * -1.0 + kernels[350] * -1.0 + kernels[351] * -1.0 + kernels[352] * -1.0 + kernels[353] * -1.0 + kernels[354] * -1.0 + kernels[355] * -1.0;
    temp += +kernels[356] * -1.0 + kernels[357] * -1.0 + kernels[358] * -0.735079871745 + kernels[359] * -1.0 + kernels[360] * -1.0 + kernels[361] * -0.368640088121 + kernels[362] * -1.0 + kernels[363] * -1.0 + kernels[364] * -1.0 + kernels[365] * -1.0 + kernels[366] * -1.0 + kernels[367] * -1.0 + kernels[368] * -1.0 + kernels[369] * -1.0 + kernels[370] * -1.0 + kernels[371] * -1.0 + kernels[372] * -1.0 + kernels[373] * -1.0 + kernels[374] * -1.0 + kernels[375] * -1.0 + kernels[376] * -1.0 + kernels[377] * -0.781746346397 + kernels[378] * -0.000547659902 + kernels[379] * -0.707649659283 + kernels[380] * -1.0 + kernels[381] * -1.0 + kernels[382] * -1.0 + kernels[383] * -1.0 + kernels[384] * -1.0 + kernels[385] * -1.0 + kernels[386] * -1.0 + kernels[387] * -1.0 + kernels[388] * -1.0 + kernels[389] * -1.0 + kernels[390] * -1.0 + kernels[391] * -1.0 + kernels[392] * -1.0 + kernels[393] * -1.0 + kernels[394] * -0.469596437194 + kernels[395] * -1.0 + kernels[396] * -1.0 + kernels[397] * -0.275368358183 + kernels[398] * -1.0 + kernels[399] * -1.0 + kernels[400] * -1.0;
    decision = decision - temp;
    temp = 0.0;

    temp += +kernels[401] * 1.0 + kernels[402] * 1.0 + kernels[403] * 1.0 + kernels[404] * 1.0 + kernels[405] * 0.481830445713 + kernels[406] * 1.0 + kernels[407] * 1.0 + kernels[408] * 1.0 + kernels[409] * 1.0 + kernels[410] * 1.0 + kernels[411] * 1.0 + kernels[412] * 1.0 + kernels[413] * 1.0 + kernels[414] * 1.0 + kernels[415] * 1.0 + kernels[416] * 1.0 + kernels[417] * 1.0 + kernels[418] * 1.0 + kernels[419] * 1.0 + kernels[420] * 1.0 + kernels[421] * 1.0 + kernels[422] * 1.0 + kernels[423] * 1.0 + kernels[424] * 1.0 + kernels[425] * 1.0 + kernels[426] * 1.0 + kernels[427] * 1.0 + kernels[428] * 0.162758969097 + kernels[429] * 1.0 + kernels[430] * 1.0 + kernels[431] * 1.0 + kernels[432] * 1.0 + kernels[433] * 0.807919713621 + kernels[434] * 1.0 + kernels[435] * 0.009986638549 + kernels[436] * 1.0 + kernels[437] * 1.0 + kernels[438] * 0.859188242579 + kernels[439] * 1.0 + kernels[440] * 1.0 + kernels[441] * 1.0 + kernels[442] * 0.220894553271 + kernels[443] * 1.0 + kernels[444] * 1.0 + kernels[445] * 1.0 + kernels[446] * 1.0 + kernels[447] * 1.0 + kernels[448] * 1.0 + kernels[449] * 1.0 + kernels[450] * 1.0 + kernels[451] * 1.0 + kernels[452] * 0.108355960842 + kernels[453] * 0.550753234697 + kernels[454] * 1.0 + kernels[455] * 1.0 + kernels[456] * 1.0 + kernels[457] * 1.0 + kernels[458] * 1.0 + kernels[459] * 1.0 + kernels[460] * 1.0 + kernels[461] * 1.0 + kernels[462] * 1.0 + kernels[463] * 1.0 + kernels[464] * 1.0 + kernels[465] * 1.0 + kernels[466] * 1.0 + kernels[467] * 1.0 + kernels[468] * 1.0 + kernels[469] * 1.0 + kernels[470] * 1.0 + kernels[471] * 0.912838882342 + kernels[472] * 1.0 + kernels[473] * 1.0 + kernels[474] * 0.289992308018 + kernels[475] * 0.006766840917 + kernels[476] * 1.0 + kernels[477] * 0.280341642141 + kernels[478] * 1.0 + kernels[479] * 1.0 + kernels[480] * 1.0 + kernels[481] * 1.0 + kernels[482] * 1.0 + kernels[483] * 1.0 + kernels[484] * 1.0 + kernels[485] * 1.0 + kernels[486] * 1.0 + kernels[487] * 1.0 + kernels[488] * 0.025305813749 + kernels[489] * 1.0;
    temp += +kernels[490] * 1.0 + kernels[491] * 1.0 + kernels[492] * 1.0 + kernels[493] * 1.0 + kernels[494] * 1.0 + kernels[495] * 1.0 + kernels[496] * 1.0 + kernels[497] * 1.0 + kernels[498] * 1.0 + kernels[499] * 1.0 + kernels[500] * 1.0 + kernels[501] * 1.0 + kernels[502] * 1.0 + kernels[503] * 1.0 + kernels[504] * 1.0 + kernels[505] * 1.0 + kernels[506] * 1.0 + kernels[507] * 1.0 + kernels[508] * 1.0 + kernels[509] * 1.0 + kernels[510] * 1.0 + kernels[511] * 1.0 + kernels[512] * 1.0 + kernels[513] * 1.0 + kernels[514] * 1.0 + kernels[515] * 1.0 + kernels[516] * 1.0 + kernels[517] * 1.0 + kernels[518] * 0.528645341184 + kernels[519] * 1.0 + kernels[520] * 1.0 + kernels[521] * 1.0 + kernels[522] * 1.0 + kernels[523] * 1.0 + kernels[524] * 1.0 + kernels[525] * 1.0 + kernels[526] * 1.0 + kernels[527] * 1.0 + kernels[528] * 1.0 + kernels[529] * 1.0 + kernels[530] * 0.839184395077 + kernels[531] * 1.0 + kernels[532] * 1.0 + kernels[533] * 1.0 + kernels[534] * 1.0 + kernels[535] * 1.0 + kernels[536] * 1.0 + kernels[537] * 1.0 + kernels[538] * 1.0 + kernels[539] * 1.0 + kernels[540] * 1.0 + kernels[541] * 1.0 + kernels[542] * 1.0 + kernels[543] * 1.0 + kernels[544] * 1.0 + kernels[545] * 1.0 + kernels[546] * 1.0 + kernels[547] * 1.0 + kernels[548] * 1.0 + kernels[549] * 1.0 + kernels[550] * 1.0 + kernels[551] * 1.0 + kernels[552] * 1.0 + kernels[553] * 1.0 + kernels[554] * 1.0 + kernels[555] * 1.0 + kernels[556] * 1.0 + kernels[557] * 1.0 + kernels[558] * 0.206595116097 + kernels[559] * 1.0 + kernels[560] * 1.0 + kernels[561] * 1.0 + kernels[562] * 1.0 + kernels[563] * 1.0 + kernels[564] * 1.0 + kernels[565] * 0.15063589406 + kernels[566] * 1.0 + kernels[567] * 1.0 + kernels[568] * 1.0 + kernels[569] * 1.0 + kernels[570] * 1.0 + kernels[571] * 0.316473049394 + kernels[572] * 0.559659450383 + kernels[573] * 1.0 + kernels[574] * 1.0 + kernels[575] * 0.877245216538 + kernels[576] * 1.0 + kernels[577] * 1.0 + kernels[578] * 1.0 + kernels[579] * 1.0 + kernels[580] * 1.0;
    temp += +kernels[581] * 1.0 + kernels[582] * 1.0 + kernels[583] * 1.0 + kernels[584] * 1.0 + kernels[585] * 1.0 + kernels[586] * 1.0 + kernels[587] * 1.0 + kernels[588] * 1.0 + kernels[589] * 1.0 + kernels[590] * 1.0 + kernels[591] * 1.0 + kernels[592] * 1.0 + kernels[593] * 1.0 + kernels[594] * 1.0 + kernels[595] * 1.0 + kernels[596] * 1.0 + kernels[597] * 1.0 + kernels[598] * 1.0 + kernels[599] * 1.0 + kernels[600] * 1.0 + kernels[601] * 1.0 + kernels[602] * 1.0 + kernels[603] * 1.0 + kernels[604] * 1.0 + kernels[605] * 1.0 + kernels[606] * 1.0 + kernels[607] * 1.0 + kernels[608] * 1.0 + kernels[609] * 1.0 + kernels[610] * 1.0 + kernels[611] * 1.0 + kernels[612] * 1.0 + kernels[613] * 1.0 + kernels[614] * 1.0 + kernels[615] * 1.0 + kernels[616] * 0.089454345471 + kernels[617] * 1.0 + kernels[618] * 1.0 + kernels[619] * 1.0 + kernels[620] * 1.0 + kernels[621] * 1.0 + kernels[622] * 1.0 + kernels[623] * 1.0 + kernels[624] * 1.0 + kernels[625] * 0.29818637763 + kernels[626] * 1.0 + kernels[627] * 1.0 + kernels[628] * 1.0 + kernels[629] * 1.0 + kernels[630] * 1.0 + kernels[631] * 1.0 + kernels[632] * 1.0 + kernels[633] * 1.0 + kernels[634] * 1.0 + kernels[635] * 1.0 + kernels[636] * 1.0 + kernels[637] * 1.0 + kernels[638] * 1.0 + kernels[639] * 1.0 + kernels[640] * 1.0 + kernels[641] * 1.0 + kernels[642] * 1.0 + kernels[643] * 1.0 + kernels[644] * 1.0 + kernels[645] * 1.0 + kernels[646] * 1.0 + kernels[647] * 1.0 + kernels[648] * 1.0 + kernels[649] * 1.0 + kernels[650] * 1.0 + kernels[651] * 1.0 + kernels[652] * 0.574770892893 + kernels[653] * 1.0 + kernels[654] * 1.0 + kernels[655] * 1.0 + kernels[656] * 1.0 + kernels[657] * 1.0 + kernels[658] * 1.0 + kernels[659] * 1.0 + kernels[660] * 1.0 + kernels[661] * 1.0 + kernels[662] * 0.974704642769 + kernels[663] * 0.350449339401 + kernels[664] * 0.366301021282 + kernels[665] * 1.0 + kernels[666] * 1.0 + kernels[667] * 1.0 + kernels[668] * 1.0 + kernels[669] * 1.0 + kernels[670] * 1.0 + kernels[671] * 1.0 + kernels[672] * 1.0;
    temp += +kernels[673] * 0.977769432082 + kernels[674] * 1.0 + kernels[675] * 1.0 + kernels[676] * 0.441615426441 + kernels[677] * 0.830419159878 + kernels[678] * 1.0 + kernels[679] * 1.0 + kernels[680] * 0.51620692298 + kernels[681] * 1.0 + kernels[682] * 1.0 + kernels[683] * 0.072101063872 + kernels[684] * 1.0 + kernels[685] * 1.0 + kernels[686] * 0.3129658992 + kernels[687] * 1.0 + kernels[688] * 1.0 + kernels[689] * 1.0 + kernels[690] * 1.0 + kernels[691] * 1.0 + kernels[692] * 1.0 + kernels[693] * 1.0 + kernels[694] * 1.0 + kernels[695] * 1.0 + kernels[696] * 1.0 + kernels[697] * 1.0 + kernels[698] * 1.0 + kernels[699] * 0.160002495628 + kernels[700] * 1.0 + kernels[701] * 1.0 + kernels[702] * 1.0 + kernels[703] * 1.0 + kernels[704] * 1.0 + kernels[705] * 0.314361725786 + kernels[706] * 0.426171524846 + kernels[707] * 1.0 + kernels[708] * 1.0 + kernels[709] * 0.638034110073 + kernels[710] * 1.0 + kernels[711] * 1.0 + kernels[712] * 1.0 + kernels[713] * 1.0 + kernels[714] * 1.0 + kernels[715] * 1.0 + kernels[716] * 1.0 + kernels[717] * 1.0 + kernels[718] * 1.0 + kernels[719] * 1.0 + kernels[720] * 1.0 + kernels[721] * 1.0 + kernels[722] * 1.0 + kernels[723] * 1.0 + kernels[724] * 1.0 + kernels[725] * 1.0 + kernels[726] * 0.30228412785 + kernels[727] * 1.0 + kernels[728] * 1.0 + kernels[729] * 1.0 + kernels[730] * 1.0 + kernels[731] * 1.0 + kernels[732] * 1.0 + kernels[733] * 1.0 + kernels[734] * 1.0 + kernels[735] * 1.0 + kernels[736] * 1.0 + kernels[737] * 1.0 + kernels[738] * 1.0 + kernels[739] * 1.0 + kernels[740] * 1.0 + kernels[741] * 1.0 + kernels[742] * 1.0 + kernels[743] * 1.0 + kernels[744] * 1.0 + kernels[745] * 1.0 + kernels[746] * 1.0 + kernels[747] * 1.0 + kernels[748] * 1.0 + kernels[749] * 1.0 + kernels[750] * 1.0 + kernels[751] * 1.0 + kernels[752] * 1.0 + kernels[753] * 1.0 + kernels[754] * 1.0 + kernels[755] * 1.0 + kernels[756] * 1.0 + kernels[757] * 1.0 + kernels[758] * 1.0 + kernels[759] * 1.0 + kernels[760] * 1.0;
    temp += +kernels[761] * 1.0 + kernels[762] * 1.0 + kernels[763] * 1.0 + kernels[764] * 1.0 + kernels[765] * 1.0 + kernels[766] * 1.0 + kernels[767] * 1.0 + kernels[768] * 1.0 + kernels[769] * 1.0 + kernels[770] * 1.0 + kernels[771] * 1.0 + kernels[772] * 1.0 + kernels[773] * 1.0 + kernels[774] * 1.0 + kernels[775] * 1.0 + kernels[776] * 1.0 + kernels[777] * 1.0 + kernels[778] * 1.0 + kernels[779] * 1.0 + kernels[780] * 1.0 + kernels[781] * 0.659400875056 + kernels[782] * 1.0 + kernels[783] * 1.0 + kernels[784] * 1.0 + kernels[785] * 1.0 + kernels[786] * 1.0 + kernels[787] * 1.0 + kernels[788] * 1.0 + kernels[789] * 1.0 + kernels[790] * 1.0 + kernels[791] * 1.0 + kernels[792] * 1.0 + kernels[793] * 1.0 + kernels[794] * 1.0 + kernels[795] * 1.0 + kernels[796] * 1.0 + kernels[797] * 1.0 + kernels[798] * 1.0 + kernels[799] * 1.0 + kernels[800] * 1.0 + kernels[801] * 1.0 + kernels[802] * 0.348746601361;
    decision = decision - temp;


    return decision > 0 ? 0 : 1;
}


int predictLabel(float* x) {        // 입력 받은 RGB값을 predict 함수에 대입
    return predict(x);              // 사람피부 경우 "0", 비사람피부 경우 "1" 리턴
}

void wait()                     // 데이터 입력 전 대기 상태(----) 출력
{
    int b = 0;
    for (b = 0; b < 4; b++)
    {
        PORTE = 0x04;
        PORTA = FND_SEGPOS[b];
        PORTE = 0x00;
        PORTE = 0x08;
        PORTA = 0x40;
        PORTE = 0x00;
        _delay_ms(1);
    }
}

void hu()                       // 각 세그먼트 자리에 맞게 "HU" 출력
{
    PORTA = 0x04;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_HU[0];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(1);
    PORTA = 0x08;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_HU[1];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(1);
}

void nohu()                     //각 세그먼트 자리에 맞게 "noHU" 출력
{
    PORTA = 0x01;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_NHU[0];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(3);
    PORTA = 0x02;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_NHU[1];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(3);
    PORTA = 0x04;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_NHU[2];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(3);
    PORTA = 0x08;
    PORTE = 0X04;
    PORTE = 0X00;
    PORTA = FND_NHU[3];
    PORTE = 0X08;
    PORTE = 0X00;
    _delay_ms(3);
}

void sound_on()
{
    PORTD = 0X10;           //D 포트의 29번 GPIO PIN 출력 ON
}

void sound_off()
{
    PORTD = 0X00;           //D 포트의 29번 GPIO PIN 출력 OFF
}


int main()
{
    init_port();                    // led 설정 초기화
    init_interrupt();               // 외부 인터럽트 설정 초기화
    timer_setting();                // 타이머 인터럽트 설정 초기화
    init_uart();                    // UART 통신 설정 초기화
    init_sound();                   // sound 관련 설정 초기화
    int j;
    char tx, rx;
    int cnt;

    sei();                          // interrupt on

    float x_sample[3];

    while (1) {                     // 무한 반복
        while (1) {                     // getN 이 0이면 반복 
            flag = 1;                   
            wait();                     // 7segment에 ---- 출력
            if (getN) {                 // getN이 1이면 (RGB 변수를 putty로 부터 받아오면)
                getN = 0;               // getN을 0으로 바꾸고 (초기화)
                break;                  // 반복문 탈출
            }
        }

        x_sample[0] = B;
        x_sample[1] = G;
        x_sample[2] = R;                // 받아온 RGB 변수들을 x_sample 배열에 저장

        a = predictLabel(x_sample);     // 훈련시킨 알고리즘을 통해 a값 출력 (a가 0이면 사람피부, a가 1이면 사람피부가 아님)

        stopgo = 1;                     // 타이머 인터럽트를 동작시키기 위함

        while (1) {

            if (a == 0)                 //predict 결과가 0(사람피부)이면 HU출력 함수 실행
            {
                hu();
            }
            if (a == 1)                 //predict 결과가 1(비사람피부)이면 noHU출려 함수 실행
            {
                nohu();
            }
            if (running_time == 10) break;              // 타이머 인터럽트를 통해 10초가 지나게 되면 display를 중단
        }
        running_time = 0;                               // running_time 변수 초기화
        stopgo = 0;                     // 타이머 인터럽트 중단
        a = 3;                          // a=3으로 설정하여 아무런 동작도 하지 않도록 설정
    }
    return 0;
}

ISR(TIMER0_OVF_vect) {                  // 타이머 인터럽트
    if (stopgo == 1) {              // stopgo가 1이면 다음의 동작들을 수행한다.
        temp++;                     // temp 증가
        if (temp == t / 2)          // 0.5초 경우 진행
        {
            if (a == 1)             // no skin 경우 진행
            {
                sound_on();         // 사운드 on
                temp++;
            }
            temp++;
        }
        if (temp == t) {            // temp==t, 즉 1초가 되면
            if (a == 1)             // a가 1(비사람피부)이면 실행
            {
                sound_off();            // 0.5초 뒤 사운드 오프
                temp = 0;
            }
            if (a == 0)             // skin 경우 진행
            {
                if (running_time == 0)  // running_time 변수가 0 일경우 진행 (단한번)
                {
                    sound_on();         // 사운드 on
                    temp = 0;
                }
                else if (running_time == 1)     // running_time 변수가 1 일경우 진행 (단한번)
                {
                    sound_off();        // 1초 동안 소리가 on 되며 한번만 진행 됨
                    temp = 0;
                }
            }

            temp = 0;               // temp 초기화
            running_time++;         // running_time 변수 증가
        }
    }
}


ISR(INT4_vect) {					// 4번 인터럽트 누르면 RGB putty로부터 받아오는 동작 수행
    _delay_ms(200);            // 스위치 에러를 줄이기 위한 딜레이 함수
    if (flag == 1) {           // 중복 인터럽트를 막기 위한 flag
        get();                      // RGB값을 putty로부터 입력받기 위한 get 함수
        getN = 1;                   // 값을 받아왔으면 getN=1 하여 변수를 받아왔음을 알린다.
        flag = 0;                   // flag 초기화
    }

}

