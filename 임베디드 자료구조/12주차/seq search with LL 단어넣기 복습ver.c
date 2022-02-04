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
	char* ptr;
	ptr = strchr((char*)a, *(char*)b);
	
	return ptr == NULL ? -1 : 0;
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
		printf("%s\n", (t+1));
	}


	printf("\n\n");
}

int main() {
	node* t;
	FILE* fp;
	fp = fopen("name.txt", "rt");
	int size = 0, i;
	char name[20];
	char find;
	init_list(&t);
	while (!feof(fp)) {
		fscanf(fp, "%s", name);
		llv_insert(name, t, &size, sizeof(name), cmp);
	}
	
	print(t, size);

	strcpy(name, "hyun");
	llv_insert(name, t, &size, sizeof(name), cmp);
	print(t, size);

	find = 'a';
	i = llv_search(&find, t, &size, sizeof(name), cmp);
	printf("%c가 들어간 단어는 %d번째 입니다.\n", find, i);
	print(t, size);


	llv_delete(&find, t, &size, sizeof(name), cmp);
	print(t, size);

	find = 'z';
	llfv_search(&find, t, &size, sizeof(name), cmp);
	print(t, size);
	

	fclose(fp);
	


	
}