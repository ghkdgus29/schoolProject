#include <stdio.h>


void move(int from, int to) {
	printf("\nMove from %d to %d", from, to);
}

void hanoi(int n, int from, int by, int to)
{
	if (n == 1)
		move(from, to);
	else {
		hanoi(n - 1, from, to, by);
		move(from, to);
		hanoi(n - 1, by, from, to);
	}
}

void main() {
	int j = 0, n = 0;
	printf("lf you want to quit, input -1");
	while (1) {
		printf("\n\nInput the height of hanoi tower: ");
		scanf("%d", &n);
		if (n > -1)
			hanoi(n, 1, 2, 3);
		else break;
	}
}