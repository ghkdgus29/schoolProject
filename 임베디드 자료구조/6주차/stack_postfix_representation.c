#include <stdio.h>
#include <stdlib.h>
#include <string.h>				// 스트링을 사용하기 위한 헤더선언
#define MAX 50

int stack[MAX];
int top;

void init_stack() {				// 스택초기화 함수
	top = -1;
}

int push(int t) {				// 스택의 push 함수
	if (top >= MAX - 1) {
		printf("\nStack overflow !!!\n");
		return -1;
	}

	stack[++top] = t;
	return t;
}

int pop() {						// 스택의 pop 함수
	if (top < 0) {
		printf("\nStack underflow !!!\n");
		return -1;
	}
	return stack[top--];
}

void postfix(char* dst, char* src) {		// postfix 함수, 문자열(계산식)을 입력받고 다음의 동작들을 수행한다.
	char c;
	init_stack();

	while (*src) {
		if (*src == ')') {					// step 1 : ')'을 만나면 스택에있는 값을 꺼내어 출력용 배열에 저장한다.
			*dst++ = pop();
			*dst++ = ' ';
			src++;
		}
		else if (*src == '+' || *src == '-' || *src == '*' || *src == '/') {		// step 2 : 연산기호들을 만나면 입력배열에 있는 연산기호들을 스택에 push한다.
			push(*src);
			src++;
		}
		else if (*src >= '0' && *src <= '9') {					// step 3 : 0~9 에해당하는 숫자들을 만나면 출력배열에 저장한다.
			do {
				*dst++ = *src++;
			} while (*src >= '0' && *src <= '9');
			*dst++ = ' ';
		}
		else src++;							// step 4 : '(' 을 만나면 그냥 넘어간다.
	}
	*dst = 0;							// 출력배열에 마지막 부분에 NULL값을 넣어준다. %s를 사용해 출력하기 위함
}

void main() {
	char exp[256];						// 출력용 배열 , 출력값 저장
	char src[256];						// 입력용 배열 , 입력값 저장
	printf("식 입력 -> ");
	gets(src);

	postfix(exp, src);
	printf("Postfix representation : %s\n", exp);
}


