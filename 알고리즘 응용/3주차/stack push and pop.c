#include <stdio.h>
#define MAX 10

int stack[MAX];
int top;

int push(int t) {
	if (top >= MAX - 1) {
		printf(" stack overflow! \n");
		return -1;
	}
	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf(" stack underflow! \n");
		return -1;
	}
	return stack[top--];
}

void main() {
	int val, input, n;
	top = -1;
	while (1) {
		printf("\n�޴��� �����ϼ���.\n1. Push\n2. Pop\n3. Show\n4. ����\n\n");
		scanf("%d", &n);
		if (n == 1) {
			printf("���� ���ڸ� �Է��ϼ���(����������=%d) : ", top);
			scanf("%d", &input);
			push(input);
		}
		else if (n == 2) {
			val = pop();
			if (top != -1)
			printf("���ڰ� ���Խ��ϴ�.(����������=%d) : %d\n", top, val);
		}
		else if (n == 3) {
			printf("���� ���: ");
			for (int i = 0; i <= top; i++) printf("%d ", stack[top - i]);
		}
		else break;
	}
}