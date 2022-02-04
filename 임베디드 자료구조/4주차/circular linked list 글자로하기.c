#include <stdio.h>
#include <stdlib.h>

char int2name(int k) {
	return ('A' + k - 1);
}

int name2int(char a) {
	return (a + 1 - 'A');
}

typedef struct node {
	int key;
	struct node* next;
}node;

node* head;

void insert_nodes(int k) {
	node* t;
	int i;
	t = (node*)calloc(1, sizeof(node));
	t->key = 1;
	head = t;
	for (i = 2; i <= k; i++) {
		t->next = (node*)malloc(sizeof(node));
		t = t->next;
		t->key = i;
	}
	t->next = head; // circular LL ÀÇ ÇÙ½É!!
}

void delete_after(node* t) {
	node* s;
	s = t->next;
	t->next = t->next->next;
	free(s);
}

void josephus(int n, int m) {
	node* t;
	int i;
	insert_nodes(n);
	t = head;
	printf("\nAnswer : ");
	while (t != t->next) {
		for (i = 0; i < m - 1; i++) t = t->next;
		printf("%c ", int2name(t->next->key));
		delete_after(t);
	}
	printf("%c", int2name(t->key));
}

void main() {
	char n;
	int m;
	
	printf("Enter N and M : ");
	scanf("%c %d", &n, &m);

	josephus(name2int(n), m);
}