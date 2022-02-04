#include <stdio.h>

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,0,1,0,0,0,
					0,0,0,1,0,0,0,
					0,1,1,1,1,0,0,
					0,0,0,0,0,1,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0, };    // filling region을 나타낼 canvas를 2차원배열로 선언

int stack[50];
int top;

int push(int n) {
	if (top == 50) {
		printf("stack overflow!\n");
		return -1;
	}
	stack[++top] = n;
	return n;
}

int pop() {
	if (top < 0) {
		printf("stack underflow!\n");
		return -1;
	}
	return stack[top--];
}

int is_stack_empty() {
	if (top == -1) return 1;
	return 0;
}


void non_recur_fill(int x, int y) {
	top = -1;
	push(y);
	push(x);
	while (!is_stack_empty()) {
		x = pop();
		y = pop();
		if (canvas[y][x] == 1) {
			canvas[y][x] = 2;
			push(y);
			push(x - 1);
			push(y);
			push(x + 1);
			push(y - 1);
			push(x);
			push(y + 1);
			push(x);
			
		}
	}
}
	

void main() {
	int i, j;							// for문 반복을 위한 변수 선언
	printf("BEFORE : \n");
	for (i = 0; i < 7; i++) {			
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	non_recur_fill(2, 3);

	printf("AFTER : \n");
	for (i = 0; i < 7; i++) {			// 수정된 canvas를 출력하기 위한 이중 for문이다.
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}
}