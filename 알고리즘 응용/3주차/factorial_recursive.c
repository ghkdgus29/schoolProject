#include <stdio.h>

int factorial(int n) {
	if (n == 1) return 1;
	return n * factorial(n - 1);
}

void main() {
	int j, n, result;
	scanf("%d", &n);
	result = factorial(n);
	printf("factorial = %d\n", result);
}