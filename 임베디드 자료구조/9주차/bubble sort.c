#include <stdio.h>

#define SIZE 10

void bubble_sort(int* a, int N) {		// data�ּ� a, data ���� N
	int i, j, t;

	for (i = 0; i < N - 1; i++) {		// ��� ������ ���� - 1 �� ��ŭ �ݺ�
		for (j = 1; j < N - i; j++) {	// 1���� N-i-1 ��ŭ �ݺ�
			if (a[j - 1] > a[j]) {		// ���� ���� ���� ���� ������ ũ��
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