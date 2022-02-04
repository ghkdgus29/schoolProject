#include <stdio.h>

void addNmul(int a, int b, int* a_add_b, int* a_mul_b) {
	*a_add_b = a + b;
	*a_mul_b = a * b;
}

void inc_CBV(int a) {
	a++;
}

void inc_CBR(int* a) {
	(*a)++;
}

int main() {
	int a, b;
	int add, mul;
	printf("input a : ");
	scanf("%d", &a);
	printf("input b : ");
	scanf("%d", &b);

	printf("a is %d and b is %d\n\n", a, b);

	inc_CBV(a);
	inc_CBR(&b);

	printf("Now a is %d and b is %d\n\n", a, b);

	addNmul(a, b, &add, &mul);

	printf("add : %d\n", add);
	printf("mul : %d\n", mul);

	
	return 0;
}