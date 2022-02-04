#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>		// �ð������� ���� ���

#define SIZE 10

void select_sort(int* a, int N) {		// seletion sort �Լ�, a�� �����͹迭, N�� �������� ����
	int min;
	int min_idx;
	int x, y;

	for (y = 0; y < N - 1; y++) {		// 0~ �ǳ����� �ι�°���� ����
		min_idx = y;				// ���� ���� ���� �ε����� y�� ����
		min = a[y];					// ���� ���� ���� a[y] �� ����
		for (x = y + 1; x < N; x++) {		// y��° ���� ������ �ǳ����� ����(�˻�)
			if (min > a[x]) {			// �� ���� �˻簪�� (x��° ����) �� min������ ���� ���� ������
				min = a[x];				// min�� (�ּҰ�)�� �� x��° ������ ������Ʈ
				min_idx = x;			// �ε����� ���� ������Ʈ
			}
		}
		a[min_idx] = a[y];				// ��� x�� �˻簡 ������ ���� ���� x��° �� ��ġ�� y�� ����ֱ�
		a[y] = min;						// y��° ��ġ�� min�� (���� ���� x��° ��) ����ֱ�
	}
}

void main() {
	int number[SIZE] = { 9,3,1,7,4,6,9,2,10,5 };			// ������

	LARGE_INTEGER freq, start, stop;
	double diff;

	QueryPerformanceFrequency(&freq);// computer frequency
	QueryPerformanceCounter(&start);// starting point

	select_sort(number, SIZE);							// ���� ����
	for (int i = 0; i < 10000000; i++); // ���̷���

	QueryPerformanceCounter(&stop); //stopping point
	diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	printf("%lf\n", diff); // �ð����

	printf("���İ�� : ");
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);			// ���ĵ� �� ���
	}
	printf("\n");


}