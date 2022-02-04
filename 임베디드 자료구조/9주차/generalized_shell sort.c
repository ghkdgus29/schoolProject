#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int intcmp(const void* a, const void* b) {		// a-b 값을 반환하는 함수
	return (*(int*)a - *(int*)b);				// type casing은 자료형마다 수정
}

void gen_shell_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {
	int i, j, k, h;					// base 데이터 주소, nelem 데이터 길이, width 데이터 메모리 크기
	void* v;
	v = malloc(width);				// width 만큼의 메모리 공간 t에 할당

	for (h = nelem / 2; h > 0; h /= 2) {
		for (i = 0; i < h; i++) {
			for (j = i + h; j < nelem; j += h) {
				memcpy(v, (char*)base + j * width, width);		// v = a[j]
				k = j;
				while (k > h - 1 && fcmp((char*)base + (k - h) * width, v) > 0) {	// while (k>h-1 && a[k-h]>v)
					memcpy((char*)base + k * width, (char*)base + (k - h) * width, width);	// a[k] = a[k-h]
					k -= h;
				}
				memcpy((char*)base + k * width, v, width);		// a[k] = v			
			}
		}
	}

	free(v);
}

void main() {
	int number[SIZE] = { 5, 3, 2, 6, 12, 14, 4, 7, 0, 3 };

	gen_shell_sort(number, SIZE, sizeof(int), intcmp);

	for (int i = 0; i < 10; i++) printf("%3d", number[i]);

}