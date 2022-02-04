#include <stdio.h>
#include <stdlib.h>

typedef struct dnode {
	int key;
	struct dnode* prev;
	struct dnode* next;
}dnode;

dnode* head, * tail;

void init_queue() {
	head = (dnode*)calloc(1, sizeof(dnode));
	tail = (dnode*)calloc(1, sizeof(dnode));
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

int put(int k) {
	dnode* t;
	if ((t = (dnode*)malloc(sizeof(dnode))) == NULL) {
		printf("Out of memory ! \n");
		return -1;
	}
	t->key = k;
	tail->prev->next = t;
	t->prev = tail->prev;
	tail->prev = t;
	t->next = tail;
	return k;
}

int get() {
	dnode* t;
	int k;
	t = head->next;
	if (t == tail) {
		printf("\nQueue underflow\n");
		return -1;
	}
	k = t->key;
	head->next = t->next;
	t->next->prev = head;
	free(t);
	return k;
}

void clear_queue() {
	dnode* s, * t;
	t = head->next;

	while (t != tail) {
		s = t;
		t = t->next;
		free(s);
	}

	head->next = tail;
	tail->prev = head;
}

void print_queue() {
	dnode* t;
	t = head->next;
	if (t == tail) printf("Queue empty !!\n");
	while (t != tail) {
		printf("%-6d", t->key);
		t = t->next;
	}
}

void main() {
	int k;
	init_queue();

	printf("1. Put 3, 6, 9, 1, 6, 3 and print front -> rear\n");
	put(3);
	put(6);
	put(9);
	put(1);
	put(6);
	put(3);
	print_queue();

	printf("\n\n2. Conduct get and print front->rear\n");
	k = get();
	printf("Get = %d\n", k);
	print_queue();

	printf("\n\n3. Put 4, 8, 7, 2, 0, 3\n");
	put(4);
	put(8);
	put(7);
	put(2);
	put(0);
	put(3);
	print_queue();

	printf("\n\n4. Initialize your queue\n");
	init_queue();
	print_queue();

	printf("\n\n5. Conduct get\n");
	get();
	print_queue();
}
