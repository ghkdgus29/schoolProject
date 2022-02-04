#include <stdio.h>
#include <stdlib.h>
#define BLK 7

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,1,1,1,0,0,
					0,1,0,0,0,1,0,
					0,1,0,0,0,1,0,
					0,1,0,0,1,0,0,
					0,0,1,1,1,0,0,
					0,0,0,0,0,0,0, };

void print_canvas(int m[][BLK]) {
	int x, y;
	for (y = 0; y < BLK; y++) {
		for (x = 0; x < BLK; x++) {
			printf("%2d", m[y][x]);
		}
		printf("\n");
	}
}

void recur_fill(int x, int y) {
	if (canvas[y][x] > 0)
		return 0;

	canvas[y][x] = 2;

	recur_fill(x + 1, y);
	recur_fill(x - 1, y);
	recur_fill(x, y + 1);
	recur_fill(x, y - 1);
}

#define MAX 100
#define is_stack_empty() (top<0)
int stack[MAX];
int top;

void push(int k) {
	if (top >= MAX - 1) {
		printf("overflow!\n");
		return -1;
	}

	stack[++top] = k;
}

int pop() {
	if (top < 0) {
		printf("underflow!\n");
		return -1;
	}

	return stack[top--];
}



void non_recur_fill(int x, int y) {
	top = -1;
	push(x);
	push(y);
	while (!is_stack_empty()) {
		y = pop();
		x = pop();
		if (canvas[y][x] == 0) {
			canvas[y][x] = 2;
			push(x);
			push(y + 1);
			push(x);
			push(y - 1);
			push(x + 1);
			push(y);
			push(x - 1);
			push(y);
		}
	}
}

int main() {

	print_canvas(canvas);


	printf("\n\n");
	//recur_fill(4, 3);
	non_recur_fill(4, 3);
	print_canvas(canvas);
}