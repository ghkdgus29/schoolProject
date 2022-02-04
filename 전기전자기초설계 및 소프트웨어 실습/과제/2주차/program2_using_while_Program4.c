#include <stdio.h>   // 사전에 정의된 표준 라이브러리 함수들을
#include <stdlib.h>  // 사용하기 위해 헤더파일을 선언한다.

int count() {  // 문장의 문장수, 문장의 단어수, 문장의 글자수를 출력하는 count() 함수를 선언한다.
	rewind(stdin);  // 입력버퍼에 남아있는 값들을 모두 지워주어 오류를 방지한다.
	int len = 0, count_sen = 0, blank = 0;  // 문자열의 길이를 나타내는 len, 문자열의 문장수를 나타내는 count_sen, 문자열의 공백(띄어쓰기)을 의미하는 blank 변수들을 0으로 초기화한다.
	char string[100]; // 문자열을 입력받을 string[100]배열을 설정한다. 99자이하로 입력가능하다.
	printf("\n문자열을 입력하세요.(99자 이하, 엔터를 누르면 입력종료) :");  //문자열 입력을 안내받는다.
	scanf("%[^\n]", string);  //scanf 를 통해 string배열에 문자를 입력한다. 이때 띄어쓰기도 가능하도록 %[^\n]을 사용하였다.

	while (string[len] != '\0') ++len;  // 무한루프를 통해 입력받은 문자열의 길이len을 알아낸다.
	for (int i = 0; i < len; i++) {     // for문을 통해 문자열의 길이만큼 다음의 동작들을 수행한다.
		while (string[i] == '.') {
			count_sen++;                          
			break;         // while문을 종료한다.                      
		}                                          // 문장에서 . ! ? 를 문장의 끝으로 생각하고
		while (string[i] == '!') {                // 다음의 글자들이 나올경우 문장수를 나타내는
			count_sen++;                          // count_sen 변수를 증가시킨다.
			break;       // while문을 종료한다.
		}
		while (string[i] == '?') {
			count_sen++;
			break;       // while문을 종료한다.
		}
		while (string[i] == ' ') {   
			blank++;                  // 문장의 공백이 나타날 경우 blank 변수를 증가시킨다.
			break;       // while문을 종료한다.
		}
	}

	printf("\n 문장수 : %d, word 수 : %d, character 수 : %d\n\n", count_sen, blank + 1, len - blank - count_sen); //문장수, 단어수, 글자수를 출력한다. 
	return 0;  // 함수를 종료한다.                                                                                  이때 글자수는 (문자열의 길이 값) - (공백) - (문장수) 를 통해 연산하였다.
}

int main() { // 메인함수를 실행한다.
	unsigned int n;  // 변수 n을 선언한다. 음수는 입력불가하다.

	printf("1.문장수, word수, 글자수 측정하기 \n\n2. 프로그램종료\n\n");			 // printf 함수를 통해 메뉴를 입력한다. 1번은 문장수, word수, 글자수 측정을 수행하고 2번은 프로그램을 종료한다.							
	printf("\n메뉴를 입력하세요(번호) : ");
	scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다.
	while (n < 2) {    // 만약 입력받은 n이 1이 아니면 프로그램을 종료한다.
		count();  // n이 1번일 때 count() 함수를 실행한다.
		printf("\n1.문장수, word수, 글자수 측정하기 \n\n2. 프로그램종료\n\n");   // printf 함수를 통해 메뉴를 입력한다.1번은 문장수, word수, 글자수 측정을 수행하고 2번은 프로그램을 종료한다.
		printf("\n메뉴를 입력하세요(번호) : ");
		scanf("%d", &n);  // scanf를 통해 메뉴의 번호를 입력받아 n에 저장한다. 만약 입력받은 n이 2이상이면 프로그램을 종료한다
	}


	return 0;  // 함수를 종료한다.
}