#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define SIZE 10

int intcmp(const void* a, const void* b) {			// a-b값을 반환하는 함수
	return (*(int*)a - *(int*)b);					// 자료형에 따라 type casting 수정해야됨
}

void gen_select_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {	// 일반화된 selection sort
	void* min;			// void 타입으로 선언 ,, base 는 데이터 주소, nelem은 데이터 길이, width는 데이터의 type(메모리 공간)
	int min_idx;
	int x, y;

	min = malloc(width);		// min 에 width만큼의 메모리 공간 생성

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
	for (int i = 0; i < 10000000; i++); // 더미루프


	QueryPerformanceCounter(&stop); //stopping point
	diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	printf("%lf\n", diff); // 시간출력
	printf("정렬결과 : ");
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}
	printf("\n");
}