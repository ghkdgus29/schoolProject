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


int main() { // 메인함수를 실행한다.
	unsigned int n;  // 변수 n을 선언한다. 음수는 입력불가하다.
	
	printf("1. 온도 변환(섭씨 <-> 화씨)\n\n2. 프로그램종료\n\n");			 // printf 함수를 통해 메뉴를 입력한다. 1번은 온도변환을 수행하고 2번은 프로그램을 종료한다.							
	printf("\n메뉴를 입력하세요(번호) : ");                                  
	scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다.
		while (n < 2) {    // 만약 입력받은 n이 1이 아니면 프로그램을 종료한다.
			temperature();  // n이 1번일 때 temperature() 함수를 실행한다.
			printf("1. 온도 변환(섭씨 <-> 화씨)\n\n2. 프로그램종료\n\n");   // printf 함수를 통해 메뉴를 입력한다. 1번은 온도변환을 수행하고 2번은 프로그램을 종료한다.
			printf("\n메뉴를 입력하세요(번호) : ");
			scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다. 만약 입력받은 n이 2이상이면 프로그램을 종료한다
		}

	
	return 0;  // 함수를 종료한다.
}