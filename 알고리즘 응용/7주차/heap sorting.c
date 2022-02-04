#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 255

char i_to_a(int k) {
	return k + 'A';
}

void upheap(int* a, int k) {
	int v;
	v = a[k];
	a[0] = INT_MAX;
	while (a[k / 2] <= v && k / 2 > 0) {
		a[k] = a[k / 2];
		k /= 2;
	}
	a[k] = v;
}

void downheap(int* a, int N, int k) {
	int i, v;
	v = a[k];
	while (k <= N / 2) {
		i = k << 1;				// 2*k
		if (i < N && a[i] < a[i + 1]) i++;
		if (v >= a[i]) break;
		a[k] = a[i];
		k = i;
	}
	a[k] = v;
}

void insert(int* a, int* N, int v) {
	a[++(*N)] = v;
	upheap(a, *N);
}

int extract(int* a, int* N) {
	int v = a[1];
	a[1] = a[(*N)--];
	downheap(a, *N, 1);

	return v;
}

void heap_sort(int*a, int N) {
	int i;
	int hn = 0;
	
	for (i = 1; i <= N; i++) insert(a, &hn, a[i]);

	for (i = hn; i >= 1; i--) a[i] = extract(a, &hn);
}
