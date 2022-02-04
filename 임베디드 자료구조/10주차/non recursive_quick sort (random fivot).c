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


void nr_quick_sort(int* a, int N) {
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
		if (r - l + 1 > 1) { // termination
			t = rand() % (r - l + 1) + l;		// t = 랜덤 변수
			v = a[t];
			a[t] = a[r];
			a[r] = v;						// 랜덤 피벗과 맨 우측 끝값의 위치를 교체
			i = l - 1;
			j = r;
			while (1) {
				while (a[++i] < v);
				while (a[--j] > v);
				if (i >= j)break;
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
	}
}

void main() {
	srand(time(NULL));

	int a[15] = { 9, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };

	nr_quick_sort(a, 15);

	for (int i = 0; i < 15; i++) printf("%5d", a[i]);
}