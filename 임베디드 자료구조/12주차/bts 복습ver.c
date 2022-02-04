#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
	int key;
	struct _node* right;
	struct _node* left;
}node;

void init_node(node** p) {
	(*p) = (node*)malloc(sizeof(node));
	(*p)->right = NULL;
	(*p)->left = NULL;
}

node* bti_search(int key, node* base, int* num) {
	node* s;
	s = base->left;
	printf("base ");
	while (s != NULL && s->key != key) {
		if (key > s->key) {
			s = s->right;
			printf("-> right ");
		}			
		else {
			s = s->left;
			printf("-> left ");
		}
			
	}

	if (s == NULL) {
		printf("no key!\n");
		return -1;
	}

	return s;	
}

node* bti_insert(int key, node* base, int* num) {
	node* s, *p;
	p = base;
	s = base->left;

	while (s!=NULL) {
		p = s;
		if (s->key > key)
			s = s->left;
		else
			s = s->right;
	}

	s = (node*)malloc(sizeof(node));
	s->key = key;
	s->left = NULL;
	s->right = NULL;
	if (p->key > key || p == base)
		p->left = s;
	else
		p->right = s;

	(*num)++;

}

node* bti_delete1(int key, node* base, int* num) {
	node* del, *son, *parent, *next;
	parent = base;
	del = parent->left;
	while (del != NULL && del->key != key) {
		parent = del;
		if (del->key > key)
			del = del->left;
		else
			del = del->right;
	}

	if (del == NULL)
		return NULL;

	if (del->right == NULL && del->left == NULL) {
		son = NULL;
	}

	else if (del->right != NULL && del->left != NULL) {
		next = del->right;
		if (next->left != NULL) {
			while (next->left->left != NULL)
				next = next->left;

			son = next->left;
			next->left = son->right;
			son->left = del->left;
			son->right = del->right;
		}
		else {
			son = next;
			son->left = del->left;
		}
	}
	else {
		if (del->right == NULL)
			son = del->left;
		else
			son = del->right;
	}

	if (parent->key > key || parent == base)
		parent->left = son;
	else
		parent->right = son;

	free(del);
	(*num)--;
}

node* bti_delete(int key, node* base, int* num) {
	node* parent, * son, * del, * next;
	parent = base;
	del = base->left;

	while (del != NULL && del->key != key) {
		parent = del;
		if (del->key > key)
			del = del->left;
		else
			del = del->right;
	}

	if (del == NULL)
		return NULL;

	if (del->right != NULL) {
		next = del->right;
		if (next->left != NULL) {
			while (next->left->left != NULL)
				next = next->left;
			son = next->left;
			next->left = son->right;
			son->left = del->left;
			son->right = del->right;

		}
		else {
			son = next;
			son->left = del->left;
		}
	}

	else {
		son = del->left;
	}

	if (parent->key > key || parent == base)
		parent->left = son;
	else
		parent->right = son;

	free(del);
	(*num)--;
}

#define MAX 100
#define init_queue() front=rear=0
#define is_queue_empty() (front==rear)
int front, rear;
node* queue[MAX];

void put(node* k) {
	if (rear+1 % MAX == front) {
		printf("overflow!\n");
		return -1;
	}

	queue[rear] = k;
	rear = ++rear% MAX;
}

node* get() {
	if (front == rear) {
		printf("empty!\n");
		return -1;
	}

	node* v = queue[front];
	front = ++front% MAX;
	return v;
}

void levelorder_traverse(node* t) {
	node* s;
	s = t->left;
	init_queue();
	
	if (s != NULL)
		put(s);

	while (!is_queue_empty()) {
		s = get();
		printf("%d ", s->key);

		if (s->left != NULL)
			put(s->left);

		if (s->right != NULL)
			put(s->right);
	}
	printf("\n\n");
}

int main() {
	node* t, * p;
	int size = 0;
	init_node(&t);

	bti_insert(5, t, &size);
	bti_insert(2, t, &size);
	bti_insert(7, t, &size);
	bti_insert(9, t, &size);
	bti_insert(1, t, &size);
	bti_insert(3, t, &size);
	bti_insert(4, t, &size);
	bti_insert(6, t, &size);
	levelorder_traverse(t);

	p = bti_search(9, t, &size);
	printf("\n%d ÀÖÀ½\n\n", p->key);

	bti_delete(9, t, &size);
	levelorder_traverse(t);



}