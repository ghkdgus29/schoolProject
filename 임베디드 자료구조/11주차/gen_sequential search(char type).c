#include <stdio.h>
#include <stdlib.h>
#define SIZE 100
#define BASE(i) ((char*)base + (i)*width)			// (char*)base + y*width �� ��ũ���Լ��� ���� ���� ȣ��

int charcmp(const void* a, const void* b) {			// a , b ũ�� �� , �ڷ����� ���� ����ȯ ���־�ߵ�
	return (*(char*)a - *(char*)b);
}

typedef int (*FCMP)(const void*, const void*);		// function pointer�� ���� ȣ���ϱ� ���� typedef

int lv_search(void* key, void* base, int* num, int width, FCMP fcmp) {	// num(�����ͱ���), width �� int������ ���, ã���Լ�
	int i = 0;
	while (fcmp(BASE(i), key) != 0 && i < *num)i++;	// while (a[i] != key && i < *num)i++;

	return(i < *num ? i : -1);						// key�� ��ã���� -1, ã���� key�� �ε��� ��ȯ
}

int lv_insert(void* key, void* base, int* num, int width, FCMP fcmp) {		// num(�����ͱ���), width �� int������ ���,  �����Լ�
	memcpy(BASE((*num)++), key, width);			// a[(*num)++] = key; �ǵڿ� key ����

	return *num - 1;				// �� �ڰ� , �ε��� ��ȯ
}

int lv_delete(void* key, void* base, int* num, int width, FCMP fcmp) {		// num(�����ͱ���), width �� int������ ���, �����Լ�
	int p, i;
	if (*num > 0) {
		if ((p = lv_search(key, base, num, width, fcmp)) < 0) return -1;	//if ((p = seq_search(key, a, num)) < 0) return -1;	// ���� p�� -1, �� ã�� key���� ������ -1��ȯ

		for (i = p + 1; i < *num; i++) {
			memcpy(BASE(i - 1), BASE(i), width);		// for (i = p + 1; i < *num; i++) a[i - 1] = a[i]; p ��ġ���� ������ ��ĭ�� ������ ���ܾ���
		}
		(*num)--;
		return p;
	}
	return -1;
}

int lfv_search(void* key, void* base, int* num, int width, FCMP fcmp) {	// num(�����ͱ���), width �� int������ ���, ã���� ������ ���� �Լ�
	int i = 0, j;
	void* v;
	while (fcmp(BASE(i), key) != 0 && i < *num) i++;		// search
	if (i >= *num) return -1;								// ã�� �� ������ -1 ��ȯ

	v = malloc(width);						// temp (������ �ӽ� ����)
	memcpy(v, BASE(i), width);				// v �� ã�� �� ���� (�ε��� i)
	for (j = i - 1; j >= 0; j--) memcpy(BASE(j + 1), BASE(j), width);	// for (j = i - 1; j >= 0; j--) a[j + 1] = a[j]; 1���� i��ġ���� ��ĭ �� ������ �б�

	memcpy(BASE(0), v, width);			// a[0] = key;
	return 0;
}

void main() {
	char data[SIZE] = "HELLO";
	int nitem = 5;
	int index, i;

	char search = 'E';
	char insert = 'Y';
	char delete = 'O';
	char fsearch = 'Y';



	index = lv_search(&search, data, &nitem, sizeof(char), charcmp);						// search ã��

	printf("E�� %d��°�� �ֽ��ϴ�.\n", index + 1);				// 1������ߵ�, �迭�̴ϱ�

	if (nitem < SIZE) lv_insert(&insert, data, &nitem, sizeof(char), charcmp);			// Y ����
	else printf("Memory is full !!\n");							// ���� ������ ������ ��á���� ���Խ���

	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4c", data[i]);

	printf("\n");
	lv_delete(&delete, data, &nitem, sizeof(char), charcmp);								// O ����


	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4c", data[i]);

	printf("\n");
	lfv_search(&fsearch, data, &nitem, sizeof(char), charcmp);							//Y�� ã�� �� ���� �Ǿ����� ����

	for (i = 0; i < nitem; i++)									// �޸� ���� ���
		printf("%4c", data[i]);






}