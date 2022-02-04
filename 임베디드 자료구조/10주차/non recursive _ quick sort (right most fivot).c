#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int key;
	struct node* next;
}node;

node* head, * tail;

void init_stack() {
	head = (node*)calloc(1, sizeof(node));
	tail = (node*)calloc(1, sizeof(node));
	head->next = tail;
	tail->next = tail;
}

int push(int k) {
	node* t;
	if ((t = (node*)malloc(sizeof(node))) == NULL) {
		printf("\nOut of memory !\n");
		return -1;
	}

	t->key = k;
	t->next = head->next;
	head->next = t;

	return k;
}

int pop() {
	node* t;
	int k;
	if (head->next == tail) {
		printf("\nStack underflow !\n");
		return -1;
	}
	t = head->next;
	k = t->key;
	head->next = t->next;
	free(t);

	return k;
}

int is_stack_empty() {
	return (head->next == tail);
}

void nr_quick_sort(int* a, int N) {		// quick sort �Լ�, 
	int v, t;
	int i, j;
	int l, r;
	init_stack();
	l = 0;			// l �� �������� ���ۺκ�
	r = N - 1;		// r �� �������� ���κ�
	push(r);
	push(l);		// l , r  push
	while (!is_stack_empty()) {
		l = pop();
		r = pop();		// l, r stack ���� pop
		if (r - l + 1 > 1) {		// ������ ���� <= 1 �̸� ����
			v = a[r];		// v �� �ǹ��� ����
			i = l - 1;		// i�� ���ʺ��� �ε��� ����
			j = r;			// j�� �����ʺ��� �ε��� ����
			while (1) {
				while (a[++i] < v);		// ���ʺ��� Ž���� �ǹ����� ū ���� ������ stop
				while (a[--j] > v);		// �����ʺ��� Ž���� �ǹ����� ���� ���� ������ stop
				if (i >= j) break;		// i=j �̸� �ݺ��� Ż��
				t = a[i];
				a[i] = a[j];
				a[j] = t;				// a[i] ���� a[j]���� �ٲ۴�
			}
			t = a[i];
			a[i] = a[r];
			a[r] = t;				// �ݺ��� ������ i = j �� �ε������� �ǹ��� ��ü
			push(r);		//(������ �κ� - ��)
			push(i + 1);			// �ǹ����� ���� �κ� push  (������ �κ� - ����)
			push(i - 1);	//(���� �κ� - ��)
			push(l);				// �ǹ����� ���� �κ� push	(���� �κ� - ����)
		}
	}
}

void main() {
	int a[15] = { 9, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };

	nr_quick_sort(a, 15);

	for (int i = 0; i < 15; i++) printf("%5d", a[i]);
}