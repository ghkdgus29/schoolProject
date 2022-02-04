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

void nr_quick_sort(int* a, int N) {		// quick sort 함수, 
	int v, t;
	int i, j;
	int l, r;
	init_stack();
	l = 0;			// l 은 데이터의 시작부분
	r = N - 1;		// r 은 데이터의 끝부분
	push(r);
	push(l);		// l , r  push
	while (!is_stack_empty()) {
		l = pop();
		r = pop();		// l, r stack 에서 pop
		if (r - l + 1 > 1) {		// 데이터 길이 <= 1 이면 종료
			v = a[r];		// v 에 피벗값 저장
			i = l - 1;		// i는 왼쪽부터 인덱스 시작
			j = r;			// j는 오른쪽부터 인덱스 시작
			while (1) {
				while (a[++i] < v);		// 왼쪽부터 탐색중 피벗보다 큰 값이 있으면 stop
				while (a[--j] > v);		// 오른쪽부터 탐색중 피벗보다 작은 값이 있으면 stop
				if (i >= j) break;		// i=j 이면 반복문 탈출
				t = a[i];
				a[i] = a[j];
				a[j] = t;				// a[i] 값과 a[j]값을 바꾼다
			}
			t = a[i];
			a[i] = a[r];
			a[r] = t;				// 반복문 나오면 i = j 의 인덱스값과 피벗값 교체
			push(r);		//(오른쪽 부분 - 끝)
			push(i + 1);			// 피벗값의 우측 부분 push  (오른쪽 부분 - 시작)
			push(i - 1);	//(왼쪽 부분 - 끝)
			push(l);				// 피벗값의 좌측 부분 push	(왼쪽 부분 - 시작)
		}
	}
}

void main() {
	int a[15] = { 9, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };

	nr_quick_sort(a, 15);

	for (int i = 0; i < 15; i++) printf("%5d", a[i]);
}