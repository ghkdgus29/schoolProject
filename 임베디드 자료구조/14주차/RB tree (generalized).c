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

typedef struct wcount {
	int count;
	char word[20];
}wcount;

const char delim[] = " \t\r\n,.-";

typedef int(*FCMP)(const void*, const void*);

int wcount_cmp(const void* a, const void* b) {
	return strcmp(((wcount*)a)->word, ((wcount*)b)->word);		// wcount �ּҸ� �Է¹����� �ּҿ� �ش��ϴ� word�� �����Ͽ� �ܾ� ũ�� ��		
}

void init_tree(node** p) {
	*p = (node*)malloc(sizeof(node));
	(*p)->left = NULL;
	(*p)->right = NULL;
	(*p)->red = 0;
}

node* rotate(void* key, node* pivot, node* base, FCMP fcmp) {
	node* child, * gchild;
	if (fcmp(key, pivot + 1) < 0 || pivot == head)
		child = pivot->left;
	else
		child = pivot->right;
	if (fcmp(key, child + 1) < 0) {		// L rotation
		gchild = child->left;
		child->left = gchild->right;
		gchild->right = child;
	}
	else {						// R rotation
		gchild = child->right;
		child->right = gchild->left;
		gchild->left = child;
	}
	if (fcmp(key, pivot + 1) < 0 || pivot == base)
		pivot->left = gchild;
	else
		pivot->right = gchild;

	return gchild;
}

node* rbti_insert(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* t, * parent, * gparent, * ggparent;
	ggparent = gparent = parent = base;
	t = base->left;
	while (t != NULL) {
		if (fcmp(key, t + 1) == 0) return NULL;			// ���� �����ϱ� ���� insertion �� �ʿ����
		if (t->left && t->right && t->left->red && t->right->red) {		// ���� �ڽĵ��� ��� red �̸�
			t->red = 1;
			t->left->red = t->right->red = 0;				// color flip
			if (parent->red) {				// red �� ���ӵɰ��
				gparent->red = 1;
				if (fcmp(key, gparent + 1) < 0 != fcmp(key, parent + 1) < 0)
					parent = rotate(key, gparent, base, wcount_cmp);
				t = rotate(key, ggparent, base, wcount_cmp);
				t->red = 0;
			}
			base->left->red = 0;
		}
		ggparent = gparent;
		gparent = parent;
		parent = t;
		if (fcmp(key, t + 1) < 0) t = t->left;
		else t = t->right;
	}
	t = (node*)malloc(sizeof(node) + width);
	memcpy(t + 1, key, width);
	t->left = NULL;
	t->right = NULL;
	if (fcmp(key, parent + 1) < 0 || parent == base)parent->left = t;
	else parent->right = t;											// ũ�� ��
	(*num)++;		// ��������
	t->red = 1;		// red ��� insert
	if (parent->red) {			// insert ����� �θ� red �̸� ����
		gparent->red = 1;
		if (fcmp(key, gparent + 1) < 0 != fcmp(key, parent + 1) < 0)
			parent = rotate(key, gparent, base, fcmp);
		t = rotate(key, ggparent, base, fcmp);		
		t->red = 0;			// ȸ�� �� �ڽ� black
	}
	base->left->red = 0;
	return t;
}

void red(node* t) {								// red ���� black������ ����ϴ� �Լ�
	if (t->red == 1) printf("red�Դϴ�.\n");
	else printf("black�Դϴ�.\n");
}


void main() {
	int len = 0;
	init_tree(&head);

	char* tok;
	char str[50] = "you did a good job";
	wcount wc;
	wcount* wcp;

	tok = strtok(str, delim);
	while (tok) {
		strcpy(wc.word, tok);
		rbti_insert(&wc, head, &len, sizeof(wcount), wcount_cmp);
		tok = strtok(NULL, delim);
	}
	
	printf("\n%s �� ", ((wcount*)(head->left + 1))->word);			// wcount �����ʹ� ���+1�� ��ġ�Ѵ�
	red(head->left);

	printf("\n%s �� ", ((wcount*)(head->left->right->left+1))->word);
	red(head->left->right->left);
	
	printf("\n%s �� ", ((wcount*)(head->left->right + 1))->word);
	red(head->left->right);

	printf("\n%s �� ", ((wcount*)(head->left->left + 1))->word);
	red(head->left->left);
}