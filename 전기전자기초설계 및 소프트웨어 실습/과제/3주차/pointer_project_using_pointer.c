#include <stdio.h>            // printf 함수등을 사용하기 위한 헤더파일 선언
#include <string.h>           // strcmp, strcpy 함수등을 사용하기 위한 헤더파일 선언

void main() {
	char str[100][11];           // 최대 100단어까지 10글자 저장할 수 있는 str배열을 만든다.
	char* temp;                   // 배열을 비교 분류할 때 배열의 주소를 임시 저장할 temp변수를 만든다.
	char* ptr[100];                // 배열을 비교 분류할 때 배열의 주소들을 저장할 공간 char*타입의 ptr 배열을 만든다.
	int n;                       // 입력할 단어 개수를 키보드 입력을 통해 n변수에 받는다.
	printf("입력할 단어 개수를 입력하세요 : ");       // printf 함수를 통해 단어 개수 입력을 안내 받는다.
	scanf("%d", &n);             // scanf함수를 통해 n변수에 입력할 단어 개수를 저장한다.

	for (int i = 0; i < n; i++) {                 // for 루프는 str배열의 처음부터 n개까지 단어를 입력하기 위함이다.
		printf("%d번째 단어를 입력하세요(10자 이하) : ", i + 1);     // printf함수를 통해 비교할 단어를 입력한다.
		scanf("%s", str[i]);	 // str배열에 차례차례 입력값을 저장한다.
		*(ptr + i) = str + i;       // ptr 배열에 str배열의 주소값들을 저장한다.
	}

	for (int i = 0; i < n - 1; i++) {             //  버블정렬을 하기위한 
		for (int j = 0; j < n - 1 - i; j++) {     //  for 루프 두개이다. 
			if (strcmp(*(ptr + j), *(ptr + j + 1)) > 0) {     // strcmp함수를 통해 ptr배열에서 앞단어가 뒷단어보다 크면 if문을 수행한다.
				temp = *(ptr + j);                         // temp에 ptr[j]에 있는 배열주소를 저장한다. (포인터로나타낸 같은 표현)                                       포인터를 이용하여 
				*(ptr + j) = *(ptr + j + 1);               // ptr[j]에 ptr[j+1]에 있는 배열주소를 저장한다.  (포인터로나타낸 같은 표현)                                  단어들의 비교정렬을 수행하였다.
				*(ptr + j + 1) = temp;                 // ptr[j+1]에 temp에 있는 배열주소를 저장한다. (포인터로나타낸 같은 표현)  
			}                                   // 위 과정을 통해 a ~ z 순서로 단어들이 배열된다.
		}
	}
	printf("Sorting a to z : ");            // 결과값을 안내하기위해 printf함수를 사용하였다.
	for (int i = 0; i < n; i++) {           // 결과값들을 순차적으로 보여주기위한 for 루프이다.
		printf(" %s ", *(ptr + i));             // 포인터를 이용하여 ptr배열을 순차적으로 출력한다.
	}
}