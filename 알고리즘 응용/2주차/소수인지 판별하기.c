#include <stdio.h>

int is_prime(int N) {
	int j;

	for (j = 2; j < N; j++) {
		if (N % j == 0) return 0;
	}

	return 1;
}

void main() {
	int N;
	N = 5;
	printf("%d is %s prime", N, is_prime(N) ? "" : "not");
}