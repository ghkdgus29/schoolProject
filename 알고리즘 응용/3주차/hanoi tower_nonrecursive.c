#include <stdio.h>

#define MAX 20

int stack[MAX];
int top;

int push(int n) {					// stack 에 값을 밀어넣는 push 함수이다. 매개변수로 전달받은 값을 push한다.
	if (top == MAX) {
		printf(" stack overflow!\n");
		return -1;
	}
	stack[++top] = n;
	return n;
}

int pop() {                        // stack에 들어있는 값을 pop, 즉 꺼낸다.
	if (top < 0) {
		printf(" stack underflow!\n");
		return -1;
	}
	return stack[top--];
}

int is_stack_empty() {             // stack이 비었는지 확인하는 함수이다.
	if (top == -1) return 1;       // 만약 스택포인터가 -1을 가리키면 스택이 비었으므로 1을 반환한다.
	return 0;                      // 만약 스택포인터가 비어있지않으면 0을 반환한다.
}

void move(int from, int to) {
	printf("\nMove from %d to %d", from, to);
}

void nr_hanoi(int n, int from, int by, int to) {          // 앞서 다뤘던 recursive한 hanoi 함수를 non recursive 형태로 변환한 함수이다.
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
			push(from);       // from 변수를 스택에 push하였다.
			from = by;       // from변수에 by 값을 넣는다.
			by = pop();      // by 변수에 스택에 있는 from값을 pop하여 넣는다.
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