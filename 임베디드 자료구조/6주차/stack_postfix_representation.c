#include <stdio.h>
#include <stdlib.h>
#include <string.h>				// ��Ʈ���� ����ϱ� ���� �������
#define MAX 50

int stack[MAX];
int top;

void init_stack() {				// �����ʱ�ȭ �Լ�
	top = -1;
}

int push(int t) {				// ������ push �Լ�
	if (top >= MAX - 1) {
		printf("\nStack overflow !!!\n");
		return -1;
	}

	stack[++top] = t;
	return t;
}

int pop() {						// ������ pop �Լ�
	if (top < 0) {
		printf("\nStack underflow !!!\n");
		return -1;
	}
	return stack[top--];
}

void postfix(char* dst, char* src) {		// postfix �Լ�, ���ڿ�(����)�� �Է¹ް� ������ ���۵��� �����Ѵ�.
	char c;
	init_stack();

	while (*src) {
		if (*src == ')') {					// step 1 : ')'�� ������ ���ÿ��ִ� ���� ������ ��¿� �迭�� �����Ѵ�.
			*dst++ = pop();
			*dst++ = ' ';
			src++;
		}
		else if (*src == '+' || *src == '-' || *src == '*' || *src == '/') {		// step 2 : �����ȣ���� ������ �Է¹迭�� �ִ� �����ȣ���� ���ÿ� push�Ѵ�.
			push(*src);
			src++;
		}
		else if (*src >= '0' && *src <= '9') {					// step 3 : 0~9 ���ش��ϴ� ���ڵ��� ������ ��¹迭�� �����Ѵ�.
			do {
				*dst++ = *src++;
			} while (*src >= '0' && *src <= '9');
			*dst++ = ' ';
		}
		else src++;							// step 4 : '(' �� ������ �׳� �Ѿ��.
	}
	*dst = 0;							// ��¹迭�� ������ �κп� NULL���� �־��ش�. %s�� ����� ����ϱ� ����
}

void main() {
	char exp[256];						// ��¿� �迭 , ��°� ����
	char src[256];						// �Է¿� �迭 , �Է°� ����
	printf("�� �Է� -> ");
	gets(src);

	postfix(exp, src);
	printf("Postfix representation : %s\n", exp);
}


