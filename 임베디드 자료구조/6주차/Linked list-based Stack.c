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

void clear() {
	node* t, * s;
	t = head->next;
	
	while (t != tail) {
		s = t;
		t = t->next;
		free(s);
	}
	head->next = tail;
}

void print_stack() {
	node* t;
	t = head->next;
	printf("\nStack top to bottom : ");
	if (t == tail) printf("Empty!!\n");
	while (t != tail) {
		printf("%-6d", t->key);
		t = t->next;
	}
}

void main() {
	int k;
	init_stack();

	printf("1. Push 3, 6, 9, 1, 6, 3 and print top to bottom");
	push(3);
	push(6);
	push(9);
	push(1);
	push(6);
	push(3);
	print_stack();

	printf("\n\n2. Conduct pop and print top to bottom");
	k = pop();
	printf("\nPop %d", k);
	print_stack();

	printf("\n\n3. Push 4, 8, 7, 2, 0");
	push(4);
	push(8);
	push(7);
	push(2);
	push(0);
	print_stack();

	printf("\n\n4. Initialize your stack");
	clear();
	print_stack();

	printf("\n\n5. Conduct pop");
	pop();

}