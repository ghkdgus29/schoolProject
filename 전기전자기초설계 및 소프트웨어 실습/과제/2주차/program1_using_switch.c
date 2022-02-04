#include <stdio.h>   // 사전에 정의된 표준 라이브러리 함수들을
#include <stdlib.h>  // 사용하기 위해 헤더파일을 선언한다.

int temperature() {      //섭씨와 화씨를 변환하는 temperature()함수를 선언한다.
	float fahr = 0, celsius;  // fahr 는 화씨를 의미하며 0으로 초기화, celsius는 섭씨를 의미한다.
	int lower, upper, step;  // lower는 화씨의 최소값을 의미하고 upper는 화씨의 최대값을 의미한다.
							 // step은 화씨의 온도 증가값을 의미한다.
	lower = 0;               // 최소값 0 으로 설정
	upper = 300;             // 최대값 300으로 설정
	step = 20;		         // 증가값 20으로 설정
	while (fahr <= upper)    // while 문을 이용해 화씨fahr 이 300이하일 경우 무한 루프를 돈다.
	{
		celsius = (5.0 / 9.0) * (fahr - 32.0);  // 섭씨 celsius를 화씨와의 관계식을 이용해 입력한다.
		printf("\nfahr = %3.0f celsius = %6.1f\n\n", fahr, celsius); // printf함수를 통해 화씨와 섭씨값을 출력한다.
		fahr = fahr + step; // 화씨값을 증가시켜준다. 증가시킴으로써 결국에는 무한루프를 빠져나갈 수 있다.
	}
	return 0;  // temperature() 함수의 종료를 의미한다.
}

int strlen() {      // 문자열의 길이를 출력하는 strlen() 함수를 선언한다.
	int i = 0;      // 문자열의 길이를 나타내는 i 변수를 0으로 초기화, 선언한다.
	rewind(stdin);  // 입력버퍼에 이상한 값들이 있을 수 있으므로 입력 버퍼값을 비워준다.
	unsigned char str[50];  // 문자열을 입력받기 위해 str[50]배열을 선언한다. 최대 49자 까지 입력가능하다.
	printf("\n문자열을 입력하세요(엔터를 누르면 입력 종료) : ");  // 문자열을 입력하라는 표시를 위함.
	scanf("%[^\n]", str); // scanf 함수를 이용하여 문자열을 str배열에 입력한다. 이때 %[^\n] 을 통해 띄어쓰기도 입력가능하다.

	while (str[i] != '\0') {  // while 문을 통해 str배열의 문자열 마지막부분 '\0'이 나올때 까지 루프를 돈다.
		++i;        // i를 증가시킴으로써 문자열의 길이를 알 수 있다.
	}

	printf("\n문자열 길이 : %d\n\n", i); // printf함수를 통해 i, 즉 문자열 길이를 출력한다.
	return 0;  // strlen() 함수를 종료한다.
}

int cal() {   // 계산을 하기 위한 cal()함수를 선언한다.
	int a, b, n, result; // a , b는 계산하려는 변수들이고 n은 계산모드를 나타내는 변수이며, result는 a , b 의 연산값을 의미한다.
	while (1) {    // 무한루프를 의미하며 중간에 조건break를 통해 무한루프를 종료시킬 수 있다.
		printf("\n1. ADD, 2. MUL, 3: quit\n번호를 입력하세요: "); // 1은 덧셈, 2는 곱셈, 3은 종료를 나타냄을 화면상에 출력한다.
		scanf("%d", &n); // n변수에 키보드입력값을 저장한다.
		if (n == 3) break; // n변수에 3이 입력되면 무한루프를 빠져나간다. 
		printf("\n두 수를 입력하세요: "); // n변수에 1, 2가 입력되면 두 수를 입력받는다.
		scanf("%d %d", &a, &b); // 띄어쓰기를 통해 입력받는 두 수를 구분할 수 있다. 
		if (n == 1) {          // n에 1이 입력되면 덧셈을 실행한다.
			result = a + b;   // a + b 덧셈을 실행 후 result에 저장한다.
			printf("%d + %d = %d\n\n", a, b, result); // a + b = result 값을 출력한다.
		}
		else if (n == 2) {      // n에 2가 입력되면 곱셈을 실행한다.
			result = a * b;    // a * b 곱셈을 실행 후 result에 저장한다.
			printf("%d * %d = %d\n\n", a, b, result);  // a * b = result 값을 출력한다.
		}
		else break;  //n에 1, 2, 3 이외에 숫자가 입력될 시 무한루프를 빠져나간다.
	}
	return 0; // 함수 종료를 의미한다.
}

int count() {  // 문장의 문장수, 문장의 단어수, 문장의 글자수를 출력하는 count() 함수를 선언한다.
	rewind(stdin);  // 입력버퍼에 남아있는 값들을 모두 지워주어 오류를 방지한다.
	int len = 0, count_sen = 0, blank = 0;  // 문자열의 길이를 나타내는 len, 문자열의 문장수를 나타내는 count_sen, 문자열의 공백(띄어쓰기)을 의미하는 blank 변수들을 0으로 초기화한다.
	char string[100]; // 문자열을 입력받을 string[100]배열을 설정한다. 99자이하로 입력가능하다.
	printf("\n문자열을 입력하세요.(99자 이하, 엔터를 누르면 입력종료) :");  //문자열 입력을 안내받는다.
	scanf("%[^\n]", string);  //scanf 를 통해 string배열에 문자를 입력한다. 이때 띄어쓰기도 가능하도록 %[^\n]을 사용하였다.

	while (string[len] != '\0') ++len;  // 무한루프를 통해 입력받은 문자열의 길이len을 알아낸다.
	for (int i = 0; i < len; i++) {     // for문을 통해 문자열의 길이만큼 다음의 동작들을 수행한다.
		if (string[i] == '.') count_sen++;        // 문장에서 . ! ? 를 문장의 끝으로 생각하고 
		else if (string[i] == '!') count_sen++;   // 다음의 글자들이 나올경우 문장수를 나타내는
		else if (string[i] == '?') count_sen++;   // count_sen 변수를 증가시킨다.
		else if (string[i] == ' ') blank++;   // 문장의 공백이 나타날 경우 blank 변수를 증가시킨다.
		else continue;  // 만약 공백이나 마침표등이 나오지 않았을 경우 for문을 수행한다.
	}

	printf("\n 문장수 : %d, word 수 : %d, character 수 : %d\n\n", count_sen, blank + 1, len - blank - count_sen); //문장수, 단어수, 글자수를 출력한다. 
	return 0;  // 함수를 종료한다.                                                                                  이때 글자수는 (문자열의 길이 값) - (공백) - (문장수) 를 통해 연산하였다.
}



int main() { // 메인함수를 실행한다.
	int n;  // 변수 n을 선언한다.

	while (1) { // 무한루프를 동작한다.
		printf("1. 온도 변환(섭씨 <-> 화씨)\n\n2. length of string\n\n");												// 메뉴들을 출력한다. 메뉴의 번호들을 입력하여 원하는
		printf("3. Simple calculator\n\n4. 문장의 문장 수 , word 수, character 수 count\n\n5. 프로그램 종료\n\n");      // 메뉴의 기능들을 수행할 수 있다.
		printf("\n메뉴를 입력하세요(번호) : ");
		scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다.
		if (n >= 5) break;  //switch case문을 이용하였기 때문에 무한루프를 빠져나가기 위한 종료조건을 설정하였다. 5이상의 숫자를 n에 입력받으면 무한루프를 빠져나간다.
		switch (n) {           // 키보드 입력받은 n값에 따라 수행하는 동작이 달라진다.
		case 1: temperature();  // n에 1이 입력되었을 때 temperatrue() 함수를 실행한다.
			break;            // switch문을 빠져나간다.
		case 2: strlen();      // n에 2가 입력되었을 때 strlen() 함수를 실행한다.                             // switch( ) ~ case 문을 이용하여 메뉴를 설정하였다.
			break;            // switch문을 빠져나간다.                                                       // 그 이외의 코드는 앞서 작성하였던 if문을 이용한 메뉴 설정과 동일하기에 
		case 3: cal();         // n에 3이 입력되었을 때 cal() 함수를 실행한다.                                // 따로 첨부하지 않는다. switch 문에서는 case를 수행후 break를 통해 switch 문을
			break;            // switch문을 빠져나간다.                                                       // 빠져 나가는 것이 핵심이다. 그렇지 않으면 밑에 있는 case들을 모두 수행하게 된다.
		case 4: count();       // n에 4가 입력되었을 때 count() 함수를 실행한다.
			break;            // switch문을 빠져나간다.
		default: break;        // n에 그 외의 값이 입력되었을 때 switch 문을 빠져나간다.
		}
	}

	return 0;  // 함수를 종료한다.
}