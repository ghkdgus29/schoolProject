#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

typedef struct node {
	int key;
	int red;
	struct node* left;
	struct node* right;
}node;

node* head;

void init_tree() {
	head = (node*)calloc(1, sizeof(node));
}

node* rotate(int key, node* pivot, node* base) {
	node* child, * gchild;
	if (key < pivot->key || pivot == head)
		child = pivot->left;
	else
		child = pivot->right;
	if (key < child->key) {		// L rotation
		gchild = child->left;
		child->left = gchild->right;
		gchild->right = child;
	}
	else {						// R rotation
		gchild = child->right;
		child->right = gchild->left;
		gchild->left = child;
	}
	if (key < pivot->key || pivot == base)
		pivot->left = gchild;
	else
		pivot->right = gchild;

	return gchild;
}

node* rbti_insert(int key, node* base, int* num) {
	node* t, * parent, * gparent, * ggparent;
	ggparent = gparent = parent = base;
	t = base->left;
	while (t != NULL) {
		if (key == t->key) return NULL;				// 값이 있으니까 굳이 insertion 할 필요없음
		if (t->left && t->right && t->left->red && t->right->red) {		// 양쪽 자식들이 모두 red 이면
			t->red = 1;
			t->left->red = t->right->red = 0;				// color flip
			if (parent->red) {				// red 가 연속될경우
				gparent->red = 1;
				if (key < gparent->key != key < parent->key)
					parent = rotate(key, gparent, base);
				t = rotate(key, ggparent, base);
				t->red = 0;
			}
			base->left->red = 0;
		}
		ggparent = gparent;
		gparent = parent;
		parent = t;
		if (key < t->key) t = t->left;
		else t = t->right;
	}
	t = (node*)malloc(sizeof(node));
	t->key = key;
	t->left = NULL;
	t->right = NULL;
	if (key < parent->key || parent == base)parent->left = t;
	else parent->right = t;											// 크기 비교
	(*num)++;		// 개수증가
	t->red = 1;		// red 노드 insert
	if (parent->red) {			// insert 노드의 부모가 red 이면 수행
		gparent->red = 1;
		if (key < gparent->key != key < parent->key)
			parent = rotate(key, gparent, base);
		t = rotate(key, ggparent, base);		
		t->red = 0;			// 회전 후 자식 black
	}
	base->left->red = 0;
	return t;
}

void red(node* t) {
	if (t->red == 1) printf("red입니다.\n");
	else printf("black입니다.\n");
}


void main() {
	int num = 0;
	init_tree(&head);
	rbti_insert(1, head, &num);
	rbti_insert(3, head, &num);
	rbti_insert(5, head, &num);
	rbti_insert(7, head, &num);
	rbti_insert(9, head, &num);

	printf("\n%d는 ", head->left->right->right->key);
	red(head->left->right->right);

}