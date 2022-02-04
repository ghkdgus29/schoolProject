#include <stdio.h>
#include <stdlib.h>

typedef struct dnode {
	int key;
	struct dnode* prev;
	struct dnode* next;
}dnode;

dnode* head, * tail;

void init_dlist() {
	head = (dnode*)calloc(1, sizeof(dnode));
	tail = (dnode*)calloc(1, sizeof(dnode));
	head->next = tail;
	head->prev = head;
	tail->next = tail;
	tail->prev = head;
}

dnode* insert_dnode_ptr(int k, dnode* t) {		// t주소노드 앞에 k값을 가진 노드집어넣기
	dnode* i;
	if (t == head) return NULL;

	i = (dnode*)calloc(1, sizeof(dnode));
	i->key = k;
	t->prev->next = i;
	i->prev = t->prev;
	i->next = t;
	t->prev = i;

	return i;
}

int delete_dnode_ptr(dnode* p) {
	if (p == head || p == tail) return 0;

	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p);

	return 1;
}

dnode* find_dnode(int k) {
	dnode* s;
	s = head->next;
	while (s->key != k && s != tail) s = s->next;
	return s;
}

int delete_dnode(int k) {
	dnode* s;
	s = find_dnode(k);

	if (s != tail) delete_dnode_ptr(s);

	return 0;		
}

dnode* insert_dnode(int k, int t) {
	dnode* s;
	dnode* i = NULL;
	s = find_dnode(t);
	if (s != tail) insert_dnode_ptr(k, s);
	return i;	
}

dnode* ordered_insert(int k) {		// 오름차순 정리 1 , 3, 5 , 6
	dnode* s;
	s = head->next;
	while (s->key <= k && s != tail) s = s->next;
	insert_dnode_ptr(k, s);
	return s;
}

void delete_all() {
	dnode* p;
	dnode* s;
	p = head->next;
	while (p != tail) {
		s = p;
		p = p->next;
		free(s);	
	}
	head->next = tail;
	tail->prev = head;
}

void print_dlist(dnode* p) {		// head 말고 head->next를 주소로 받는다면 p=p->next 지우기
	printf("\n");
	p = p->next;
	while (p != tail) {
		printf("%-8d", p->key);
		p = p->next;
	}
}

void main() {
	dnode* t;

	init_dlist();
	ordered_insert(9);
	ordered_insert(3);
	ordered_insert(5);
	ordered_insert(1);
	ordered_insert(7);

	printf("Current list : ");
	print_dlist(head);

	printf("\n1. Insert 4 before 5\n ");
	ordered_insert(4);
	print_dlist(head);

	printf("\n2. Inserting node 2 before 9\n");
	insert_dnode(2, 9);
	print_dlist(head);

	printf("\n3. Delete node 4\n");
	delete_dnode(4);
	print_dlist(head);

	delete_all();
	free(head);
	free(tail);

}