#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void downheap(char* a, int N, int k) {	// ������ ���� �Ʒ��� ����  (���ڿ������͸� char* a, ���������͸� int* a)
	int i, v;
	v = a[k];				// v �� root���� ���� (���� k=1)
	while (k <= N / 2) {		// k�� internal node �������� Ŀ���� �ݺ��� ����
		i = k << 1;			// i = 2*k, �� i���� k����� ���� �ڽ�
		if (i < N && a[i] < a[i + 1]) i++;			// ���� ������ �ڽ��� �� ũ�� ������ �ڽ��� i���� ����
		if (v >= a[i]) break;						// v��, �� root���� �ڽİ����� ũ�� ����
		a[k] = a[i];		// �θ𰪿� �ڽİ� ����, �� �ڽİ��� ���� �ø�
		k = i;				// k���(�θ���)�� i���(�ڽĳ��)�� �������� �ݺ�
	}
	a[k] = v;				// �ݺ����� ���� ��ġ�� k��忡 ó�� ���� root�� ����
}

void heap_sort_bottom(char* a, int N) {		 // N�� ������ ����, a�� �� �ּ�, bottom up ��� (���ڿ������͸� char* a, ���������͸� int* a)
	int k, t;
	for (k = N / 2; k > 0; k--) downheap(a, N, k);		// ��� internal node �� ���Ͽ� downheap ����

	while (N > 1) {				// ������ ���̰� 1���ϰ� �Ǹ� ����
		t = a[1];
		a[1] = a[N];
		a[N] = t;				// ���� �Ʒ��� �ִ� ����(������) ���� ���� �ִ� ���(ū��)�� ��ü
		downheap(a, --N, 1);	//	��ü�� ���� ū���� ���ΰ� �ٽ� ���� --> �������� ������ �ȴ�
	}
}								

// ����

int bi_search(int key, char* a, int N) {				// binary search �Լ� (�˻�) ( ���ڿ������͸� char* a, ���������͸� int* a)
	int mid;
	int left = 0;
	int right = N - 1;
	while (right >= left) {
		mid = left + (float)(key - a[left]) * (right - left) / (a[right] - a[left]);		// �߰��� ����  mid = (right +left) / 2; �� �ص� ��
		if (key == a[mid])return mid;				// key�� �߰����� ���
		if (key < a[mid]) right = mid - 1;			// key�� �߰������� ���� ���
		else left = mid + 1;						// key�� �߰������� Ŭ ���
	}
	return -1;					// ã�� key ���� ���� ��
}

int bi_insert(int key, char* a, int* N) {			// ����				(���ڿ������͸� char* a, ���������͸� int* a)
	int p = 0;
	int i;
	if (key > a[*N]) (*N)++;				// (�������� �Է� ����) ���� �����Ϳ� �ִ� ��� ������ ū ���� ���ԵǸ� ������ ���̸� ������Ų��. (heap�̶� *N�̰� �ٸ� �����̸� *N-1��
	while (key > a[p] && p < *N) p++;				// key���� a[p]������ ���� �� ���� p�� ���� (a[p]���� key������ �ٷ� ū��)
	for (i = *N; i > p; i--) a[i] = a[i - 1];		// ��ĭ �� �з����� (p+1�ڸ�����)
	a[p] = key;										// p�ڸ��� key�� �ֱ�
	(*N)++;						// ������ ���� ����

	return p;				// key���� ���� ��ġ ��ȯ
}

int bi_delete(int key, char* a, int* N) {			// ����       ( ���ڿ������͸� char* a, ���������͸� int* a)
	int p;
	int i;
	if (*N > 0) {
		if ((p = bi_search(key, a, *N)) < 0)				// ã�� ���� ������ -1��ȯ, p=-1
			return -1;
		for (i = p + 1; i < *N; i++) a[i - 1] = a[i];		// ��ĭ �� ���ܾ��� (p���� ������)
		a[*N-1] = '\0';						// (�������� ���� �� �ƴ�) string �Ϸ��� ����������� int�� �� ��
		(*N)--;					// ������ ���� ����
		return p;			// ������ key�� ��ġ��ȯ
	}
	return -1;		
}



void main() {
	int i;
	char str[20] = "TOSORTALGORITHM";
	char cpy[20] = { 0 };					// cpy ���ڿ�
	cpy[0] = ' ';							// ó������ ����
	int size = 15;							// �迭 ����
	
	for (i = 0; i < 15; i++) cpy[i + 1] = str[i];		// str�� cpy�� ����

	printf("����\n");
	for (i = 1; i <= 15; i++) printf("%3c", cpy[i]);	// ����
	
	heap_sort_bottom(cpy, size);						// ����	

	printf("\n\n����\n");
	for (i = 1; i <= 15; i++) {
		printf("%3c", cpy[i]);							// ���İ� ���	
	}
	int p = bi_search('S', cpy, size);
	printf("\n\nFind your key at %d (value %c)\n", p, cpy[p]);		// S Ž��

	p = bi_insert('Z', cpy, &size);					// Z ����

	printf("\n\nInsert Z and string\n");
	for (i = 1; i <= 16; i++) {
		printf("%3c", cpy[i]);						// ������ ���
	}

	printf("\n%s", cpy);							// ���ڿ��� ���

	p = bi_delete('A', cpy, &size);				// A����
	printf("\n%s\n", cpy);							// ������ ���ڿ��� ���
}