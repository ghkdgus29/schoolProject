#include <stdio.h>
#define MAX 10

int stack[MAX];
int top;

void init_stack() {
	top = -1;
}

int push(int t) {
	if (top >= MAX - 1) {
		printf("\nStack overflow !!!\n");
		return -1;
	}

	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf("\nStack underflow !!!\n");
		return -1;
	}
	return stack[top--];
}

void stack_print() {
	if (top < 0) printf("\nStack empty!!\n");
	printf("\nstack top to bottom : ");
	for (int i = top; i >= 0; i--) printf("%3d", stack[i]);
}

void main() {
	int k;
	init_stack();

	printf("1. Push 3, 6, 9, 1, 6 ,3 and print top->bottom");
	push(3);
	push(6);
	push(9);
	push(1);
	push(6);
	push(3);
	stack_print();

	printf("\n\n2. Conduct pop and print top->bottom");
	k = pop();
	printf("\npop = %d", k);
	stack_print();

	printf("\n\n3. Push 4, 8, 7, 2, 0");
	push(4);
	push(8);
	push(7);
	push(2);
	push(0);
	stack_print();

	printf("\n\n4. Initialize your stack");
	init_stack();
	stack_print();

	printf("\n\n5. Conduct pop");
	pop();

	

}