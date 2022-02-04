#include <stdio.h>
#include <stdlib.h>

void downheap(int* a, int N, int k) {	// ������ ���� �Ʒ��� ����
	int i, v;
	v = a[k];				// v �� �� ����
	while (k <= N / 2) {		// internal node ����
		i = k << 1;				// i = k*2
		if (i < N && a[i] < a[i + 1]) i++;			// ������ ���� �� ū ���� ��ü
		if (v >= a[i]) break;						// ���� v���� �ڽİ����� ũ�� ����
		a[k] = a[i];								// v���� �ڽİ����� ������ �ڸ���ü
		k = i;										// �ڽİ��� k�� ����
	}
	a[k] = v;					// ��� ��ü�� ������ �ش� �ڸ��� v�� ����
}

void heap_sort_bottom(int* a, int N) {		// bottom up ����� heap sort
	int k, t;
	for (k = N / 2; k > 0; k--) downheap(a, N, k);		// ��� internal node �� ���Ͽ� downheap ����

	while (N > 1) {
		t = a[1];
		a[1] = a[N];
		a[N] = t;				// ���� �Ʒ��� �ִ� ����(������) ���� ���� �ִ� ���(ū��)�� ��ü
		downheap(a, --N, 1);	//	��ü�� ���� ū���� ���ΰ� �ٽ� ���� --> �������� ������ �ȴ�
	}
}								

void main() {
	int a[15] = { INT_MAX, 4, -80, 1, 117, 13, 76, 39, -99, 2054, 10 , -10, 3, 67, -28 };

	heap_sort_bottom(a, 14);		// a[0]�� ���Ŀ� ������� �����Ƿ� ���� ������ ���̿��� 1�� ���ش�.

	for (int i = 1; i < 15; i++) {
		printf("%5d", a[i]);			// ���
	}
	printf("\n");
}