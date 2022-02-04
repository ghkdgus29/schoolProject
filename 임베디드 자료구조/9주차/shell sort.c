#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

void shell_sort(int* a, int N) {		// shell sort �Լ�, a�� ������ �ּ�, N�� ������ ����
	int i, j, k, h, v;
	for (h = N / 2; h > 0; h /= 2) {		// h�� ���͹�, ��ü ������ ���̸� ������ �������� 0�� �ɶ� ���� �ݺ�
		for (i = 0; i < h; i++) {			// 0���� ���͹�-1 ���� �ݺ� (i)
			for (j = i + h; j < N; j += h) {		// i+h, �� ���͹���ŭ ������ �ݺ�
				v = a[j];					// j��° ���� v�� ����
				k = j;						// k�� �ε����� ����� insert sort �Ұ���
				while (k > h - 1 && a[k - h] > v) {		// k�� ���͹����� ũ�� ���͹����� a[j]���� ū���� ������ ���͹����� ����
					a[k] = a[k - h];		// ���������� ���͹� ĭ����ŭ �о�ִ´�.
					k -= h;					// k�ε����� ���͹� ����ŭ ���ҽ�Ų��
				}
				a[k] = v;			// �ݺ����� ��ġ�� k�ε����� a[j]�� ������ ��ġ�� �ȴ�.

			}
		}
	}
}

void main() {
	int number[SIZE] = { 5, 3, 2, 6, 12, 14, 4, 7, 0, 3 };

	shell_sort(number, 10);

	for (int i = 0; i < SIZE; i++) printf("%3d", number[i]);
}