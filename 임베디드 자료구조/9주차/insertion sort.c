#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

void insert_sort(int* a, int N) {			// insert sort �Լ� , a�� �������ּ�, N�� ������ ����
	int i, j, t;
	for (i = 1; i < N; i++) {				// �ι�° �����ͺ��� �� �������� ���� �ݺ�
		t = a[i];					// t�� a[i] �� ����
		j = i;
		while (j > 0 && a[j - 1] > t) {		// ���� ���� j�ε����� (�� ó���� i�ε���) 0���� ũ��,  a[i]���� ũ��
			a[j] = a[j - 1];		// �ݺ��ؼ� ���� �ٲ۴� (������ �δ�)
			j--;					// j�ε����� ������ �̵�
		}
		a[j] = t;					// �ݺ����� ���������� �� j�ε����� a[i]�� ���� �ڸ��̴�.
	}
}

void indirect_insert_sort(int* a, int* index, int N) {	// ���� insert sort �Լ�, a�� ������ �ּ�, index�� �ε��� �ּ�, N�� ������ ����
	int i, j, t;
	for (i = 0; i < N; i++) index[i] = i;		// �ε��� �ʱ�ȭ 0 ~ N-1

	for (i = 1; i < N; i++) {
		t = index[i];				// t�� index[i] ����, �� a[i]�� �ε����� ����
		j = i;
		while (j > 0 && a[index[j - 1]] > a[t]) {		// ���������� ���� �������� ���簪���� ũ�ٸ� 
			index[j] = index[j - 1];				// �ε����� �ٲ۴� (������ �δ�)
			j--;									// �ε����� ������ �̵�
		}
		index[j] = t;		// �ݺ����� ���������� �� a[i]�� �ε����� index[j]�� ���� �ڸ��̴�.
	}
}

void main() {
	int i;
	int number[SIZE] = { 9, 3, 1, 7 , 9 , 4, 5, 6, 2, 11 };

	insert_sort(number, SIZE);
	for (i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}

	int number2[SIZE] = { 9, 3, 1, 7 , 9 , 4, 5, 6, 2, 11 };
	int index[SIZE];

	printf("\n�������� sort\n");
	indirect_insert_sort(number2, index, SIZE);
	for (i = 0; i < 10; i++) printf("%3d", number2[index[i]]);

	printf("\n����\n");
	for (i = 0; i < 10; i++) printf("%3d", number2[i]);
}