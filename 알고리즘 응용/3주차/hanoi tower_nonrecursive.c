#include <stdio.h>

#define MAX 20

int stack[MAX];
int top;

int push(int n) {					// stack �� ���� �о�ִ� push �Լ��̴�. �Ű������� ���޹��� ���� push�Ѵ�.
	if (top == MAX) {
		printf(" stack overflow!\n");
		return -1;
	}
	stack[++top] = n;
	return n;
}

int pop() {                        // stack�� ����ִ� ���� pop, �� ������.
	if (top < 0) {
		printf(" stack underflow!\n");
		return -1;
	}
	return stack[top--];
}

int is_stack_empty() {             // stack�� ������� Ȯ���ϴ� �Լ��̴�.
	if (top == -1) return 1;       // ���� ���������Ͱ� -1�� ����Ű�� ������ ������Ƿ� 1�� ��ȯ�Ѵ�.
	return 0;                      // ���� ���������Ͱ� ������������� 0�� ��ȯ�Ѵ�.
}

void move(int from, int to) {
	printf("\nMove from %d to %d", from, to);
}

void nr_hanoi(int n, int from, int by, int to) {          // �ռ� �ٷ�� recursive�� hanoi �Լ��� non recursive ���·� ��ȯ�� �Լ��̴�.
	int done = 0;
	top = -1;
	while (!done) {
		while (n > 1) {
			push(to);
			push(by);
			push(from);
			push(n);

			n--;
			push(to);
			to = by;
			by = pop();
		}
		move(from, to);
		if (!is_stack_empty()) {
			n = pop();
			from = pop();
			by = pop();
			to = pop();
			move(from, to);

			n--;
			push(from);       // from ������ ���ÿ� push�Ͽ���.
			from = by;       // from������ by ���� �ִ´�.
			by = pop();      // by ������ ���ÿ� �ִ� from���� pop�Ͽ� �ִ´�.
		}
		else done = 1;
	}

}


void main() {
	int n = 0;
	printf("lf you want to quit, input -1");
	while (1) {
		printf("\n\nInput the height of hanoi tower: ");
		scanf("%d", &n);
		if (n > -1)
			nr_hanoi(n, 1, 2, 3);
		else break;
	}
}