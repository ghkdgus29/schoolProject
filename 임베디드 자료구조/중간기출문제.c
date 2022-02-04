#include <stdio.h>
#include <stdlib.h>
#include <string.h>				// ��Ʈ���� ����ϱ� ���� �������

#define MAX 100

int stack[MAX];
int top;


int name2int(char a) {
	return (a  - '0');
}

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

int calcPostFixEq(char* src) {		// postfix �Լ�, ���ڿ�(����)�� �Է¹ް� ������ ���۵��� �����Ѵ�.
	char op;
	int a, b;
	int sum;

	init_stack();

	while (*src) {
		
		if (*src == '+' || *src == '-' || *src == '*' || *src == '/') {		// step 2 : �����ȣ���� ������ �Է¹迭�� �ִ� �����ȣ���� ���ÿ� push�Ѵ�.
			op = *src++;
			a = pop();
			b = pop();
			if (op == '+') sum = a + b;
			else if (op == '-') sum = a - b;
			else if (op == '*') sum = a * b;
			else if (op == '/') sum = a / b;
			
			push(sum);
			printf("sum %d\n", sum);


		}
		else if (*src >= '0' && *src <= '9') {					// step 3 : 0~9 ���ش��ϴ� ���ڵ��� ������ ��¹迭�� �����Ѵ�.
			do {
				push(name2int(*src++));
				
			} while (*src >= '0' && *src <= '9');
		}
		else src++;							// step 4 : '(' �� ������ �׳� �Ѿ��.
	}
	return pop();
							// ��¹迭�� ������ �κп� NULL���� �־��ش�. %s�� ����� ����ϱ� ����
}

void main() {
	char eqn[100] = "3 9 / 3 9 * - 5 -";
	int result;
	
	result = calcPostFixEq(eqn);
	printf("result = %d\n", result);
}
