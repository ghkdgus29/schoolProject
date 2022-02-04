#include <stdio.h>            // printf와 같은 함수를 사용하기 위해 선언하는 헤더파일

void main() {
	char str[1001];              // 1000글자 까지 받을 수 있는 str배열 생성
	int blank = 0, index = 0;     //  문자열의 공백 개수를 저장할 blank 변수와 문자열을 검사하기 위한 index 변수를 0으로 초기화한다.
	printf("Plz input your text : ");   // printf 함수를 통해 문자열을 입력하라는 안내를 한다
	scanf("%[^\n]s", str);      // scanf 함수를 통해 str배열에 키보드 입력값을 저장한다. 이때 %[^\n]s 를 통해 키보드엔터를 누르기 전까지의 값이 저장된다. 

	while (1) {                    // while 문을 이용하여 배열을 확인하였다.
		
		if (str[index] == '\0') break;   // 이때 배열전체를 확인할 필요 없이 입력받은 문자열의 끝부분 '\0'을 만나면 무한루프를 빠져나간다.
		else if (str[index] == ' ') blank++;   // 만약 배열을 확인하는 도중 ' ' 공백을 만나게 되면 blank++ 를 통해 blank 변수를 증가시킨다.
		index++;                              // index값을 증가시켜서 입력받은 문자열을 처음부터 끝까지 검사한다.
	}
	printf("%s have %d space", str, blank);      // printf 함수를 통해 내가 입력한 문자열을 보여주고 blank 변수를 보여줌으로써 빈칸수를 나타낸다. 
}
