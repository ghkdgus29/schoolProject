#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int intcmp(const void* a, const void* b) {		// a-b ���� ��ȯ�ϴ� �Լ�
	return (*(int*)a - *(int*)b);				// type casing�� �ڷ������� ����
}

void gen_shell_sort(void* base, int nelem, int width, int(*fcmp)(const void*, const void*)) {
	int i, j, k, h;					// base ������ �ּ�, nelem ������ ����, width ������ �޸� ũ��
	void* v;
	v = malloc(width);				// width ��ŭ�� �޸� ���� t�� �Ҵ�

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