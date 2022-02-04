#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void insert_sort(int* a, int N) {
	int i, j, t;

	for (i = 1; i < N; i++) {
		t = a[i];
		j = i;
		while (j > 0 && a[j - 1] > t) {
			a[j] = a[j - 1];
			j--;
		}
		a[j] = t;
	}
}

void improve_quick_sort(int* a, int N) {
	int v, t;
	int i, j;
	int l, r;
	init_stack();
	l = 0;
	r = N - 1;
	push(r);
	push(l);
	while (!is_stack_empty()) {
		l = pop();
		r = pop();
		if (r - l + 1 > 200) {				// 자료가 200보다 길면 quick sort
			t = rand() % (r - l + 1) + l;
			v = a[t];
			a[t] = a[r];
			a[r] = v;
			i = l - 1;
			j = r;
			while (1) {
				while (a[++i] < v);
				while (a[--j] > v);
				if (i >= j) break;
				t = a[i];
				a[i] = a[j];
				a[j] = t;
			}
			t = a[i];
			a[i] = a[r];
			a[r] = t;
			push(r);
			push(i + 1);
			push(i - 1);
			push(l);
		}
		else insert_sort(a + l, r - l + 1);				//자료가 200보다 작으면 insert sort , a+l은 데이터의 시작주소, r-l+1은 데이터의 길이
	}
}

void main() {
	srand(time(NULL));

	int a[250];

	for (int i = 0; i < 250; i++) {
		a[i] = rand() % 1000;
	}

	for (int i = 0; i < 250; i++) printf("%4d", a[i]);
	printf("\n\n");

	improve_quick_sort(a, 250);

	for (int i = 0; i < 250; i++) printf("%4d", a[i]);
}