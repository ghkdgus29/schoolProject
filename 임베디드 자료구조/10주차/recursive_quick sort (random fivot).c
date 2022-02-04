#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quick_sort(int* a, int N) {
	int v, t;
	int i, j;
	int random;
	if (N > 0) random = rand() % N;
	if (N > 1) {				 //termination
		v = a[random];				// ���� �ǹ� ����
		a[random] = a[N - 1];		// ���� �ִ� �� ���� ���� ���� �ǹ��ڸ��� �̵�
		a[N - 1] = v;				// ���� �ǹ����� �� �������� �̵�
		i = -1;
		j = N - 1;
		while (1) {
			while (a[++i] < v);
			while (a[--j] > v);
			if (i >= j) break;
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];
		a[i] = a[N - 1];
		a[N - 1] = t;

		quick_sort(a, i);
		quick_sort(a + i + 1, N - i - 1);
	}
}

void main() {
	srand(time(NULL));

	int a[15] = { 9, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };

	quick_sort(a, 15);

	for (int i = 0; i < 15; i++) printf("%5d", a[i]);
}