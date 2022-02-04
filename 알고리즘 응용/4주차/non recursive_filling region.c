#include <stdio.h>

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,1,1,1,0,0,
					0,1,0,0,0,1,0,
					0,1,0,0,0,1,0,
					0,1,0,0,1,0,0,
					0,0,1,1,1,0,0,
					0,0,0,0,0,0,0, };

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
		if (!(canvas[y][x] == 1 || canvas[y][x] == 2)) {
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
	int i, j;
	non_recur_fill(4, 3);
	
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}
	
}