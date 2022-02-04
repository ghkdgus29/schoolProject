#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define SIZE 10

int intcmp(const void* a, const void* b) {			// a-b���� ��ȯ�ϴ� �Լ�
	return (*(int*)a - *(int*)b);					// �ڷ����� ���� type casting �����ؾߵ�
}

void gen_select_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {	// �Ϲ�ȭ�� selection sort
	void* min;			// void Ÿ������ ���� ,, base �� ������ �ּ�, nelem�� ������ ����, width�� �������� type(�޸� ����)
	int min_idx;
	int x, y;

	min = malloc(width);		// min �� width��ŭ�� �޸� ���� ����

	for (y = 0; y < nelem - 1; y++) {
		min_idx = y;
		memcpy(min, (char*)base + y * width, width);		// min = a[y]
		for (x = y + 1; x < nelem; x++) {
			if (fcmp(min, (char*)base + x * width) > 0) {		// if (min > a[x]) 
				memcpy(min, (char*)base + x * width, width);	// min = a[x];
				min_idx = x;
			}
		}
		memcpy((char*)base + min_idx * width, (char*)base + y * width, width);		// a[min_idx] = a[y];
		memcpy((char*)base + y * width, min, width);								// a[y] = min
	}
	free(min);
}

void main() {
	int number[SIZE] = { 9, 3, 1, 7, 4, 6 ,9, 2, 10, 5 };

	LARGE_INTEGER freq, start, stop;
	double diff;

	QueryPerformanceFrequency(&freq);// computer frequency
	QueryPerformanceCounter(&start);// starting point


	gen_select_sort(number, SIZE, sizeof(int), intcmp);
	for (int i = 0; i < 10000000; i++); // ���̷���


	QueryPerformanceCounter(&stop); //stopping point
	diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	printf("%lf\n", diff); // �ð����
	printf("���İ�� : ");
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}
	printf("\n");
}