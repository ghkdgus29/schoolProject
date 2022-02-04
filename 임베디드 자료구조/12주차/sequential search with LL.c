#include <stdio.h>
#include <stdlib.h>
#define LEN 5
#define BASE(i) ((char*)base + (i)*width)

int intcmp(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

typedef int (*FCMP)(const void*, const void*);

typedef struct node {
	struct node* next;
}node;

void init_list(node** p) {
	*p = (node*)malloc(sizeof(node));
	(*p)->next = NULL;		// *(*p).next= NULL; �� ���� ǥ��
}

node* llv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// ã�� �Լ� (�ּ� ��ȯ)
	node* t;
	t = base->next;
	while (fcmp(t + 1, key) != 0 && t != NULL) t = t->next;		// t+1�� ������ �������, ���� ã�� �������� t�ּ� ��� �̵�
	return (t == NULL ? NULL : t + 1);		// ã�� �� ������ NULL ��ȯ, ���� ã���� t+1 (������ ��� ��) �ּ� ��ȯ
}

node* llv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {	// �����Լ�
	node* t;
	t = (node*)malloc(sizeof(node) + width);			// data�� �������� ���� �ϳ� �� ������ش�
	memcpy(t + 1, key, width);							// ������� �濡 data�� ����ִ´�.
	t->next = base->next;								
	base->next = t;							// �ּҸ� ��ũ�� ����Ʈ�� �̿��� ����
	(*num)++;								// ������ ���� ����
	return t;								// �����͸� ���� �ִ� �ּ� (�����ʹ� ���� �濡 ����) ��ȯ
}

node* llv_delete(void* key, node* base, int* num, int width, FCMP fcmp) {		// key ���� ã�� �ش� ��带 �����ϴ� �Լ�
	node* p, * s;
	p = base;
	s = p->next;

	while (fcmp(s + 1, key) != 0 && s != NULL) {			// �ش� ��� ã��
		p = p->next;					// p�� Ÿ�ٳ�� ���� ��� (������ ����)
		s = s->next;					// s�� Ÿ�� ���
	}
	if (s == NULL) return NULL;			// ��ã���� NULL ��ȯ

	p->next = s->next;					// s ������� ����
	free(s);
	(*num)--;							// ���� ����

	return p;
}

node* llfv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// key ���� ã�� �ش� ��带 �� ������ �̵���Ű�� �Լ�
	node* p, * s;
	p = base;
	s = p->next;
	while (fcmp(s + 1, key) != 0 && s != NULL) {			// �ش� ��� ã��
		p = p->next;				// p�� Ÿ�ٳ�� ���� ��� (������ ����)
		s = s->next;				// s�� Ÿ�� ���
	}
	if (s == NULL) return NULL;			// ��ã���� NULL ��ȯ

	p->next = s->next;						// s�ǳ� �ٰ� p ����
	s->next = base->next;					// s�� ���̽� �ڿ� ���� ����
	base->next = s;							// ���̽��� s �� ����

	return s;
}

int count(void* key, node* base, int* num, int width, FCMP fcmp) {		// �� �� °���� ã�� �Լ�
	node* t;
	int cnt = 1;			// ������ �� ����, ���� �ʰ� ���� ���� LL�� ���� �տ� ����Ǿ� �ִ�.
	t = base->next;
	while (fcmp(t + 1, key) != 0 && t != NULL) { // t+1�� ������ �������
		t = t->next;								// ���� ã�� �������� t�ּ� ��� �̵�
		cnt++;										// cnt�� �������� ���� ����
	}	
	return (t == NULL ? -1 : cnt);		// ã�� �� ������ NULL ��ȯ, ���� ã���� t+1 (������ ��� ��) ���°���� ��ȯ
}

void main() {
	void* p;
	node* t, * s;
	int i;
	int size = 0, key = 9;
	int data[LEN] = { 3, 1, 9, 7, 5 };

	init_list(&t);

	for (i = 0; i < 5; i++) {
		p = llv_insert(data + i, t, &size, sizeof(int), intcmp);				// ������ ���� (LL)

		printf("%2d", *((node*)p + 1));									// ������ �� ���
	}

	p = llv_search(&key, t, &size, sizeof(int), intcmp);			// p�� ã�� key���� �ּ� (void*)

	i = count(&key, t, &size, sizeof(int), intcmp);					// i�� ã�� key���� ���°���� ��Ÿ���� �Լ�

	printf("\n%d��ġ�� starting addr�� ���� %d��° �Դϴ�.\n", *(int*)p, i);

	llfv_search(&key, t, &size, sizeof(int), intcmp);				// ã�� ������ ���� �Լ�

	for (s = t->next; s != NULL; s = s->next) {
		printf("%2d", *(s + 1));
	}

	llv_delete(&key, t, &size, sizeof(int), intcmp);				// ���� �Լ�
	printf("\n");

	for (s = t->next; s != NULL; s = s->next) {
		printf("%2d", *(s + 1));
	}


}