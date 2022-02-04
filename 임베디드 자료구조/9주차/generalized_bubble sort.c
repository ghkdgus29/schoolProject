#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int intcmp(const void* a, const void* b) {		// a - b 반환하는 함수
	return (*(int*)a - *(int*)b);				// 자료형에 따라 type casting 수정해야됨
}

void gen_bubble_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {
	int i, j;					// base 데이터 주소, nelem 데이터 길이, width 데이터 크기(메모리 공간)
	void* t;
	t = malloc(width);			// width 크기의 메모리 공간을 t에 할당

	for (i = 0; i < nelem - 1; i++) {
		for (j = 1; j < nelem - i; j++) {
			if (fcmp((char*)base + (j - 1) * width, (char*)base + j * width) > 0) {	// if (a[j-1]>a[j])
				memcpy(t, (char*)base + (j - 1) * width, width);							// t = a[j-1]
				memcpy((char*)base + (j - 1) * width, (char*)base + j * width, width);		// a[j-1] = a[j]
				memcpy((char*)base + j * width, t, width);									// a[j] = t
			}
		}
	}
	free(t);
}

void main() {
	int number[SIZE] = { 9, 3, 1, 7 , 9 , 4, 5, 6, 2, 11 };

	gen_bubble_sort(number, SIZE, sizeof(int), intcmp);
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}

}