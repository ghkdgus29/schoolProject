#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int intcmp(const void* a, const void* b) {		// a - b ��ȯ�ϴ� �Լ�
	return (*(int*)a - *(int*)b);				// �ڷ����� ���� type casting �����ؾߵ�
}

void gen_bubble_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {
	int i, j;					// base ������ �ּ�, nelem ������ ����, width ������ ũ��(�޸� ����)
	void* t;
	t = malloc(width);			// width ũ���� �޸� ������ t�� �Ҵ�

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