#include <stdio.h>            // printf와 같은 함수를 사용하기 위해 선언하는 헤더파일

void main() {
	char str[1001];              // 1000글자 까지 받을 수 있는 str배열 생성
	int blank = 0;     //  문자열의 공백 개수를 저장할 blank 변수를 0으로 초기화
	printf("Plz input your text : ");   // printf 함수를 통해 문자열을 입력하라는 안내를 한다
	scanf("%[^\n]s", str);      // scanf 함수를 통해 str배열에 키보드 입력값을 저장한다. 이때 %[^\n]s 를 통해 키보드엔터를 누르기 전까지의 값이 저장된다. 

	for (int index = 0; index < 1001; index++) {     // for 루프를 이용하여 배열전체를 확인하였다. index를 늘려가며 동작을 수행하고 str 배열의 크기만큼 for 루프를 반복한다.
		if (str[index] == ' ') blank++;            // 만약 배열에서 ' ' (공백)을 발견할 시 blank++ 를 통해 blank 변수를 늘려주었다.
	}

	printf("%s have %d space", str, blank);      // printf 함수를 통해 내가 입력한 문자열을 보여주고 blank 변수를 보여줌으로써 빈칸수를 나타낸다. 
}
