#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTS_library.h"

typedef struct wcount {
	int count;				// �ܾ ��� ���Դ��� count ����
	char word[20];			// �ܾ� ����
}wcount;

node* head;					// tree base
const char delim[] = " \t\r\n,.-";			// ������ �ƽ�Ű���� ������ NULL�� �Է��ϰ� �ܾ�ȭ

int wcount_cmp(const void* a, const void* b) {
	return strcmp(((wcount*)a)->word, ((wcount*)b)->word);		// wcount �ּҸ� �Է¹����� �ּҿ� �ش��ϴ� word�� �����Ͽ� �ܾ� ũ�� ��		
}

void print_wcount(void* a) {
	printf("%s : %d\n", ((wcount*)a)->word, ((wcount*)a)->count);	// �ش� Ʈ���� �����ϴ� ������ �ּ��� count�� word ���
}

void main() {
	int i, num = 0, cnt = 0;

	char* tok;			// ������ �ܾ��� �ּҸ� ������ ������
	char str[256];
	wcount wc;
	wcount* wcp;

	init_tree(&head);

	FILE* fp;
	fp = fopen("wordcount.txt", "r");

	fgets(str, 256, fp);		// ���Ͽ��� �� �� �о�ͼ� str�迭�� (�ִ� 256��) �����ϱ�
	do {					// 256�� ���� file�� ���� ��츦 ����� do while�� ����
		tok = strtok(str, delim);	// str�κ��� ������ �ܾ��� �ּҸ� tok�� ����
		while (tok) {			// tok�� �� �̻� �ܾ ������� ���� �� ���� �ݺ� 
			strcpy(wc.word, tok);			// tok�� ����� �ܾ wc.word�� ����
			wcp = (wcount*)btv_search(&wc, head, &num, sizeof(wcount), wcount_cmp);		// wc�ּҸ� key�� Ʈ���� Ž��, wcp�� Ž������� �ּҸ� ��ȯ �޴´�.
			if (wcp == NULL) {		// ã�� �� ����, �� ó�� ã�� �ܾ���
				wc.count = 1;
				btv_insert(&wc, head, &num, sizeof(wcount), wcount_cmp);			// ó�� ã�� �ܾ��� �ּҸ� Ʈ���� ����
			}
			else
				(wcp->count)++;		// ã�� �� ����, ���� �ܾ� count�� ����
			tok = strtok(NULL, delim);	// ���� �ܾ���� ����
		}
		fgets(str, 256, fp);	// ������ ���� ������ �о�� str�迭�� ����

	} while (!feof(fp));	// ���� file�� ���̸� �ݺ��� ����

	printf("\n\n");

	btv_list(head->left, print_wcount);			// tree �׷����� ������ (�ܾ�:ī��Ʈ) ���

	printf("\n\n\n");

	btv_balance(head, &num, sizeof(wcount));		// balanced tree �׷��� ����

	btv_list(head->left, print_wcount);		// balanced tree �׷��� ���

	fclose(fp);
}