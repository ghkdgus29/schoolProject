#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int intcmp(const void* a, const void* b) {		// a-b ���� ��ȯ�ϴ� �Լ�
	return(*(int*)a - *(int*)b);				// type casting�� �ڷ������� ����
}

void gen_insert_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {
	int i, j;					// base ������ �ּ�, nelem ������ ����, width ������ �޸� ũ��
	void* t;
	t = malloc(width);			// width ��ŭ�� �޸� ���� t�� �Ҵ�

	for (i = 1; i < nelem; i++) {
		memcpy(t, (char*)base + i * width, width);			// t = a[i]
		j = i;
		while (j > 0 && fcmp((char*)base + (j - 1) * width, t) > 0) {			// while (j>0 && a[j-1] > t)
			memcpy((char*)base + j * width, (char*)base + (j - 1) * width, width);	// a[j] = a[j-1]
			j--;
		}
		memcpy((char*)base + j * width, t, width);			// a[j] = t
	}

	free(t);
}

void main() {
	int number[SIZE] = { 5, 3, 2, 6, 12, 14, 4, 7, 0, 3 };

	gen_insert_sort(number, SIZE, sizeof(int), intcmp);

	for (int i = 0; i < 10; i++) printf("%3d", number[i]);
}