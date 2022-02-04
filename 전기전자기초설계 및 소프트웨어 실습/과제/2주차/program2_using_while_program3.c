#include <stdio.h>   // 사전에 정의된 표준 라이브러리 함수들을
#include <stdlib.h>  // 사용하기 위해 헤더파일을 선언한다.

int cal() {   // 계산을 하기 위한 cal()함수를 선언한다.
	int a, b, n, result; // a , b는 계산하려는 변수들이고 n은 계산모드를 나타내는 변수이며, result는 a , b 의 연산값을 의미한다.
	while (1) {    // 무한루프를 의미하며 중간에 break를 통해 무한루프를 종료시킬 수 있다.
		printf("\n1. ADD, 2. MUL, 3: quit\n번호를 입력하세요: "); // 1은 덧셈, 2는 곱셈, 3은 종료를 나타냄을 화면상에 출력한다.
		scanf("%d", &n); // n변수에 키보드입력값을 저장한다.
		while (n >= 3) return 0; // n변수에 3이상이 입력되면 함수를 종료한다.
		printf("\n두 수를 입력하세요(띄어쓰기로 구분): "); // n변수에 1, 2가 입력되면 두 수를 입력받는다.
		scanf("%d %d", &a, &b); // 띄어쓰기를 통해 입력받는 두 수를 구분할 수 있다. 
		while (n == 1) {          // n에 1이 입력되면 덧셈을 실행한다.
			result = a + b;   // a + b 덧셈을 실행 후 result에 저장한다.
			printf("%d + %d = %d\n\n", a, b, result); // a + b = result 값을 출력한다.
			break;            //while문을 빠져나간다.
		}
		while (n == 2) {      // n에 2가 입력되면 곱셈을 실행한다.
			result = a * b;    // a * b 곱셈을 실행 후 result에 저장한다.
			printf("%d * %d = %d\n\n", a, b, result);  // a * b = result 값을 출력한다.
			break;           //while문을 빠져나간다.
		}
             
	}
	
}
int main() { // 메인함수를 실행한다.
	unsigned int n;  // 변수 n을 선언한다. 음수는 입력불가하다.
	
	printf("1.Simple calculator \n\n2. 프로그램종료\n\n");			 // printf 함수를 통해 메뉴를 입력한다. 1번은 간단한 계산을 수행하고 2번은 프로그램을 종료한다.							
	printf("\n메뉴를 입력하세요(번호) : ");                                  
	scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다.
		while (n < 2) {    // 만약 입력받은 n이 1이 아니면 프로그램을 종료한다.
			cal();  // n이 1번일 때 cal() 함수를 실행한다.
			printf("\n1.Simple calculator \n\n2. 프로그램종료\n\n");   // printf 함수를 통해 메뉴를 입력한다. 1번은 간단한 계산을 수행하고 2번은 프로그램을 종료한다.
			printf("\n메뉴를 입력하세요(번호) : ");
			scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다. 만약 입력받은 n이 2이상이면 프로그램을 종료한다
		}

	
	return 0;  // 함수를 종료한다.
}