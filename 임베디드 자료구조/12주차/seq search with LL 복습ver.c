#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 5

typedef struct _node {
	struct _node* next;
}node;

void init_list(node** p) {
	(*p) = (node*)malloc(sizeof(node));
	(*p)->next = NULL;
}

typedef int (*FCMP)(const void*, const void*);
#define BASE(i) ((char*)base+(i)*width)

int cmp(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

void llv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* t;
	t = (node*)malloc(sizeof(node) + width);
	if (t == NULL) {
		printf("overflow!!\n");
		return -1;
	}
	memcpy(t + 1, key, width);

	t->next = base->next;
	base->next = t;

	(*num)++;
}

int llv_search(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* t;
	int cnt = 1;
	t = base->next;
	while (t != NULL && fcmp(t+1, key)!=0) {
		t = t->next;
		cnt++;
	}

	return t == NULL ? -1 : cnt;
}

int llv_delete(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* p, * s;
	if (*num > 0) {
		p = base;
		s = p->next;
		while (fcmp(s + 1, key) != 0 && s != NULL) {
			p = p->next;
			s = p->next;
		}
		if (s == NULL) {
			printf("no key!\n");
			return -1;
		}

		p->next = s->next;
		free(s);
		(*num)--;
		return 1;
	}
	printf("no data!\n");
	return -1;
}

node* llfv_search(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* p, * s;
	if (*num > 0) {
		p = base;
		s = base->next;
		while (s != NULL && fcmp(s + 1, key) != 0) {
			p = p->next;
			s = p->next;
		}
		if (s == NULL) {
			printf("no key!\n");
			return -1;
		}

		p->next = s->next;

		s->next = base->next;
		base->next = s;

		return 1;
	}
	printf("no data!\n");
	return -1;
}

void print(node* base, int size) {
	node* t;

	if (base->next == NULL) {
		printf("no data!\n\n");
		return -1;
	}

	for (t = base->next; t != NULL; t = t->next) {
		printf("%d  ", *(t+1));
	}


	printf("\n\n");
}

int main() {
	node* t;
	int p;
	int i;
	int size = 0, key = 9;
	int data[LEN] = { 3,1,9,7,5 };

	init_list(&t);

	for (i = 0; i < 5; i++) {
		llv_insert(data + i, t, &size, sizeof(int), cmp);
	}
	print(t, size);

	llv_insert(&key, t, &size, sizeof(int), cmp);
	print(t, size);

	key = 1;
	p = llv_search(&key, t, &size, sizeof(int), cmp);
	printf("find data at %d (value %d)\n\n", p, key);

	llv_delete(&key, t, &size, sizeof(int), cmp);
	print(t, size);

	key = 3;
	llfv_search(&key, t, &size, sizeof(int), cmp);
	print(t, size);
}