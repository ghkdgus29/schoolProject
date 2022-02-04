#include <stdio.h>

#define SIZE 10

void bubble_sort(int* a, int N) {		// data주소 a, data 길이 N
	int i, j, t;

	for (i = 0; i < N - 1; i++) {		// 모든 데이터 개수 - 1 번 만큼 반복
		for (j = 1; j < N - i; j++) {	// 1부터 N-i-1 만큼 반복
			if (a[j - 1] > a[j]) {		// 만약 이전 값이 이후 값보다 크면
				t = a[j - 1];
				a[j - 1] = a[j];
				a[j] = t;				// swap
			}
		}
	}
}

void main() {
	int number[SIZE] = { 9,3,1,7,9,4,5,6,2,11 };

	bubble_sort(number, 10);
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}
}