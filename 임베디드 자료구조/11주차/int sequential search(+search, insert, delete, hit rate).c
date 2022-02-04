#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

int seq_search(int key, int* a, int* num) {		// key�� ã�� ��, a�� ������, num�� ������ ����
	int i = 0;
	while (a[i] != key && i < *num)i++;			// key���� ã�� �� ���� �ε��� ����

	return (i < *num ? i : -1);					// ���� key���� ��ã���� i = *num �̴ϱ� -1��ȯ
}												//  key���� ã���� �ش� key�� �ε��� ��ȯ

int seq_insert(int key, int* a, int* num) {		// key���� ������(a) �ǵڿ� ����
	a[(*num)++] = key;			// �� �ڿ� ����

	return *num - 1;			// �� �ڰ�, key�� �ε��� ��ȯ
}

int seq_delete(int key, int* a, int* num) {		// key���� a�����Ϳ��� ã�� ����
	int p, i;
	if (*num > 0) {		// ������ ���̰� 0���� Ŀ�ߵ�
		if ((p = seq_search(key, a, num)) < 0) return -1;		// ���� p�� -1, �� ã�� key���� ������ -1��ȯ

		for (i = p + 1; i < *num; i++) a[i - 1] = a[i];			// key���� ã���� p(key ��ġ)��ġ ���� ������ ��ĭ�� ������ ���ܾ���
		(*num)--;			// ������ ���� ��ĭ ���̱�
		return p;			// ������ ������ ��ġ ��ȯ
	}
	return -1;			// ������ ���� 0���� ������ -1��ȯ
}

int seq_f_search(int key, int* a, int* num) {		// key���� a�����Ϳ��� ã��, ã�� key���� �� ������ ����
	int p, i;
	if ((p = seq_search(key, a, num)) < 0) return -1;			// ���� p�� -1,�� ã�� key���� ������ -1��ȯ

	for (i = p - 1; i >= 0; i--) a[i + 1] = a[i];				//key���� ã���� 1���� p��ġ���� ��ĭ �� ������ �б�(key ��ġ)
	a[0] = key;			// key�� �� ������
	return 0;
}


void main() {
	int data[SIZE] = { 1, 4, 7, 34, 23, 6, 356, 674 , 123, -54 };
	int nitem = 10;
	int index, i;


	index = seq_search(34, data, &nitem);						// search ã��

	printf("34�� %d��°�� �ֽ��ϴ�.\n", index+1);				// 1������ߵ�, �迭�̴ϱ�

	if (nitem < SIZE) seq_insert(453, data, &nitem);			// 453 ����
	else printf("Memory is full !!\n");							// ���� ������ ������ ��á���� ���Խ���

	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4d", data[i]);

	printf("\n");
	seq_delete(23, data, &nitem);								// 23 ����


	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4d", data[i]);

	printf("\n");
	seq_f_search(-54, data, &nitem);							//-54�� ã�� �� ���� �Ǿ����� ����

	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4d", data[i]);
	


	

	
}