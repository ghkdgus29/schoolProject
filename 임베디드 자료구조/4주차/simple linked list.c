#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int key;
	struct node* next;
}node;

node* head, * tail;

void init_list() {
	head = (node *)calloc(1, sizeof(node));
	tail = (node *)calloc(1, sizeof(node));
	head->next = tail;
	tail->next = tail;
}

node* insert_after(int k, node* t) {			// ��� t(�ּ�) �ڿ� k������ ��� ����ֱ�
	node* temp;
	temp = (node*)calloc(1, sizeof(node));
	temp->key = k;
	temp->next = t->next;
	t->next = temp;

	return temp;
}

node* insert_node_before(int t, int k) {			//  value t���� value k�� �տ� �ֱ�
	node* s, * p, * r;
	p = head;
	s = p->next;
	while (s->key != k && s != tail) {
		p = p->next;
		s = p->next;
	}
	if (s != tail) {
		r = (node*)calloc(1, sizeof(node));
		r->key = t;
		p->next = r;
		r->next = s;
	}
	return p->next;
}

node* insert_node_after(int t, int k) {			//  value t���� value k�� �ڿ� �ֱ�
	node* s, * p, * r;
	p = head;
	s = p->next;
	while (s->key != k && s != tail) {
		p = p->next;
		s = p->next;
	}
	if (s != tail) {
		p = p->next;
		s = s->next;
		r = (node*)calloc(1, sizeof(node));
		r->key = t;
		p->next = r;
		r->next = s;
	}
	return p->next;
}

int delete_next(node* t) {			// t�ּ� ���ڿ� ��带 �����
	node* temp;
	if (t->next == tail) return 0;
	temp = t->next;
	t->next = t->next->next;
	free(temp);

	return 1;
}

node* find_node(int k) {			// k���� ���� ��带 ã�� �ּҸ� ��ȯ, count�� ���°�� �ִ��� �ǹ�
	node* temp;
	int count = 1;

	temp = head->next;
	while (temp->key != k && temp != tail) {
		temp = temp->next;
		count++;
	}
	if (temp == tail) {
		printf(" ã������ �� ����! \n");
	}
	else printf("����%d�� %d��° node�� �ֽ��ϴ�.\n",k, count);
	return temp;
}

int delete_node(int k) {		// k���� ���� ��� ����
	node* s, * p;
	p = head;
	s = p->next;
	while (s->key != k && s != tail) {
		p = p->next;
		s = p->next;
	}
	if (s != tail) {
		p->next = s->next;
		free(s);
		return 1;
	}
	else return 0;
}

node* delete_all() {
	node* s;
	node* t;
	t = head->next;
	while (t != tail) {
		s = t;
		t = t->next;
		free(s);
	}
	head->next = tail;
	return head;
}

void ordered_insert(int k) {			// �������� k����
	node* s, * p, * r;
	p = head;
	s = p->next;
	while (s->key <= k && s != tail) {
		p = p->next;
		s = p->next;
	}
	r = (node*)calloc(1, sizeof(node));
	r->key = k;
	p->next = r;
	r->next = s;
}

void print_list(node* t) {
	printf("����Ʈ ����: ");
	t = t->next;
	if (t == tail) printf("List is empty!\n");
	while (t != tail) {
		printf("%-8d", t->key);
		t = t->next;
	}
}

void main() {
	node* t;

	init_list();
	ordered_insert(9);
	ordered_insert(3);
	ordered_insert(5);
	ordered_insert(1);
	ordered_insert(7);

	printf("1. Print out initial list\n");
	print_list(head);

	printf("\n\n2. Find the node containing 5\n");
	find_node(5);

	printf("\n\n3. Insert 6 after 5 and Print out the current list\n");
	ordered_insert(6);
	print_list(head);

	printf("\n\n4. Delete node 5 and print out the result\n");
	delete_node(5);
	print_list(head);

	printf("\n\n5. Delete all the node\n");
	delete_node(1);
	delete_node(3);
	delete_node(6);
	delete_node(7);
	delete_node(9);
	print_list(head);

	free(tail);
	free(head);
}
