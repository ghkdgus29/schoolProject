#include <stdio.h>            // printf 함수등을 사용하기 위한 헤더파일 선언
#include <string.h>           // strcmp, strcpy 함수등을 사용하기 위한 헤더파일 선언

void main() {
	char str[100][11];           // 최대 100단어까지 10글자 저장할 수 있는 str배열을 만든다.
	char temp[11];               // 배열을 비교 분류할 때 배열을 임시로 저장할 공간 temp배열을 만든다.
	int n;                       // 입력할 단어 개수를 키보드 입력을 통해 n변수에 받는다.
	printf("입력할 단어 개수를 입력하세요 : ");       // printf 함수를 통해 단어 개수 입력을 안내 받는다.
	scanf("%d", &n);             // scanf함수를 통해 n변수에 입력할 단어 개수를 저장한다.

	for (int i = 0; i < n; i++) {                 // for 루프는 str배열의 처음부터 n개까지 단어를 입력하기 위함이다.
		printf("%d번째 단어를 입력하세요(10자 이하) : ", i + 1);     // printf함수를 통해 비교할 단어를 입력한다.
		scanf("%s", str[i]);	 // str배열에 차례차례 입력값을 저장한다.
	}

	for (int i = 0; i < n - 1; i++) {             //  버블정렬을 하기위한 
		for (int j = 0; j < n - 1 - i; j++) {     //  for 루프 두개이다. 
			if (strcmp(str[j], str[j+1]) > 0) {   // strcmp함수를 통해 str배열에서 앞단어가 뒷단어보다 크면 둘의 순서를 바꾼다.
				strcpy(temp, str[j]);              // temp배열에 str[j]값을 복사해서 붙여넣는다.                                         배열을 이용하여
				strcpy(str[j], str[j + 1]);        // str[j]배열에 str[j+1]값을 복사해서 붙여넣는다.                                     단어들의 비교정렬을 수행하였다.
				strcpy(str[j + 1], temp);          // str[j+1]배열에 temp배열값을 복사해 붙여넣는다. 
			}                                   // 위 과정을 통해 a ~ z 순서로 단어들이 배열된다.
		}
	}
	printf("Sorting a to z : ");            // 결과값을 안내하기위해 printf함수를 사용하였다.
	for (int i = 0; i < n; i++) {           // 결과값들을 순차적으로 보여주기위한 for 루프이다.
		printf(" %s ", str[i]);             // 배열을 순차적으로 출력한다.
	}
}