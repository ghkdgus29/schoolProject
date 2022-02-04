#include <stdio.h>
#include <stdlib.h>

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

typedef struct node {
	int key;
	struct node* left;
	struct node* right;
}node;

node* head;

void init_tree() {
	head = (node*)calloc(1, sizeof(node));
}

node* bti_search(int key, node* base, int* num) {			// ���ϴ� key���� ���� ��带 Ž���ϴ� �Լ�,num�� �����ͱ�����
	node* s;
	s = base->left;		// ���� ���� base�� ���ʿ� ��ġ
	printf("\nbase->left");			// ��ġ ����� ����
	while (key != s->key && s != NULL) {
		if (key < s->key) {
			s = s->left;	// ���� key���� ���� ��庸�� ������ �������� Ʈ��Ž��
			printf("->left");			// ��ġ ����� ����
		}
		else {
			s = s->right;					// ���� key���� ���� ��庸�� ũ�� ���������� Ʈ��Ž��
			printf("->right");			// ��ġ ����� ����
		}
	}
	if (s == NULL) return NULL;		// ���� ã�� key���� ��尡 ������ NULL ��ȯ
	else return s;						// ã���� key���� ����ּ� ��ȯ
}

node* bti_insert(int key, node* base, int* num) {
	node* p, * s;
	p = base;			// parent node			
	s = base->left;					// son node, Ʈ���� �Ѹ��� base�� left���� �����Ѵ�
	while (s != NULL) {					// NULL�� ������ �� ���� �ݺ�, while���� ���� �Է¹��� key���� �� s����� �θ� p�ּҸ� ã�´�.
		p = s;								// ���� s��带 p��忡 ����		
		if (key < s->key) s = s->left;		// ����ִ� key���� �������� key������ ������ s����ּҸ� ���ʾƷ��� �̵�
		else s = s->right;					// ����ִ� key���� �������� key������ ũ�� s����ּҸ� �����ʾƷ��� �̵�
	}
	s = (node*)malloc(sizeof(node));		// s��� �޸� ������ ���� (�Է��� key���� �� s���)
	s->key = key;							// s��� �ȿ� �Է¹��� key���� ����
	s->left = NULL;
	s->right = NULL;						// ������� s����� ���� ������ NULL�� �׶����Ŵ
	if (key < p->key || p == base) p->left = s;		// ���� parent node �� base�ų� key���� p ����� key������ ������ ���� �Ʒ��� Ʈ������
	else p->right = s;								// ���� parent node �� base�ų� key���� p ����� key������ ũ�� ������ �Ʒ��� Ʈ������
	(*num)++;			// ������ ���� ���� (Ʈ�� ����)
	return s;			// �ش� ��� �ּ� ��ȯ (key���� ����ִ�)
}

node* bti_delete(int key, node* base, int* num) {	// delete �Լ��� condition�� ���� ���� ������ delete �Լ�
	node* parent, * son, * del, * nexth;
	parent = base;
	del = base->left;						// base�� ���ʿ� Ʈ���� �Ѹ��� �ִ�. parent - del ���� �θ� - �ڽ� �����̴�
	while (key != del->key && del != NULL) {			// �Է¹��� key���� ���� ���� ã�ų� NULL�� ���� �ٴٸ������� �ݺ�
		parent = del;					// ���� ��带 parent��忡 ����
		if (del->key > key) del = del->left;		// ���� ����� key���� �Է¹��� key������ ũ�� ���� �Ʒ��� �̵�
		else del = del->right;						// ���� ����� key���� �Է¹��� key������ ������ ������ �Ʒ��� �̵�
	}		// ������� ����� parent, del �ּҸ� ã�� �ݺ���

	if (del == NULL) return NULL;		// ������� ��带 ��ã���� NULL ��ȯ

	//---------������� bti_delete1�Լ��� ����  (del node�� ã�� ����)

	if (del->right == NULL) son = del->left;	// del node�� ������ �ڽ��� ������ ���� �ڽĳ�尡 son��尡 �ȴ�
	else if (del->right->left == NULL) {		// del node�� ������ �ڽĳ��� ������ , ������ �ڽĳ���� �����ڽĳ��� ���ٸ� 
		son = del->right;					// ������ �ڽĳ�尡 son node�� �ȴ�.
		son->left = del->left;				// ������ �ڽĳ��(son node) �� del node�� ��ü�Ѵ�.
	}
	else {							// del node�� root�� ���
		nexth = del->right;
		while (nexth->left->left != NULL) nexth = nexth->left;	// ���������� , ���� ���ʿ� �ִ� ����� �θ� ��带 nexth�� ����
		son = nexth->left;			// ����������, ���� ���ʿ� �ִ� ��带 son���� ����
		nexth->left = son->right;	// nexth�� son����� ������ ���е��� ����
		son->left = del->left;
		son->right = del->right;		// root��带 son ���� ��ü (������ �ٲ�)
	}

	//----------�Ʒ� �κп��� bti_delete1�Լ��� ���� (parent -son ��� ����)

	if (key < parent->key || parent == base)		// case 1,2,3�� ���� �����ؾ� �ϴ� �۾�, parent�� son ��带 ����
		parent->left = son;					// parent���� son���� ũ�� son�� ���ʾƷ� ��ġ
	else
		parent->right = son;				// parent���� son���� ������ son�� �����ʾƷ� ��ġ


	free(del);		// del ��� �����
	(*num)--;		// ������ ���� ���̱�

	return parent;		// ���� ����� �θ��� ��ȯ
}

#define MAX 100
node* queue[MAX];				// level order traverse �� ���� ť�� ����� �ּ� ����
int front, rear;				//ť�� ���� �ε���

void init_queue() {					// ť �ʱ�ȭ
	front = rear = 0;
}

int is_queue_empty() {				// ť�� ������� �˻�, ������� 1��ȯ
	return (front == rear);
}

node* put(node* t) {				// ť�� Ʈ���ּҸ� �ֱ����� put �Լ�
	queue[rear] = t;
	rear = ++rear % MAX;
	return t;
}

node* get() {						// ť���� Ʈ���ּҸ� ������ ���� get�Լ�
	node* t;
	t = queue[front];
	front = ++front % MAX;
	return t;
}

void visit(node* t) {								// �湮�� Ʈ���� ���� ����ϴ� �Լ�
	printf("%2c", int2name(t->key));
}

void levelorder_traverse(node* t) {					// level order traverse �� ���� �Լ�
	put(t);

	while (!is_queue_empty()) {
		t = get();
		visit(t);

		if (t->left != NULL) put(t->left);
		if (t->right != NULL) put(t->right);
	}
}

void bti_sort(node* p, int* a, int* index) {			// �־��� binary tree�� �迭�� �̾Ƴ��� �Լ� (Inorder traverse�� ���)
	if (p != NULL) {		// ��������: tree��尡 NULL�� �ٴٸ��� ����
		bti_sort(p->left, a, index);			// ���ʳ�� �湮 (���� ���)
		a[(*index)++] = p->key;					// �ش� ��� �湮�Ͽ� �迭�� tree data�� �Է� 
		bti_sort(p->right, a, index);			// �����ʳ�� �湮 (ū ���)
	}
}

node* balance(int N, int* a, int* index) {				// �̾Ƴ� �迭�� ���� �������� binary tree ����
	int nl, nr;									
	node* p;
	if (N > 0) {		//  ��������: ������ ���̰� 0�̵Ǹ� ����
		nl = (N - 1) / 2;						// nl�� �迭 �߰���(root)�� ������ ���� �迭 ����
		nr = N - nl - 1;						// nr�� �迭 �߰���(root)�� ������ ������ �迭 ����
		p = (node*)malloc(sizeof(node));
		p->left = balance(nl, a, index);		// ���ʳ�� ���� (���� ���)
		p->key = a[(*index)++];					// ���� �湮 ��忡 �迭���� ����ִ´�.
		p->right = balance(nr, a, index);		// ������ ��� ���� (ū ���)

		return p;
	}
	else return NULL;
}

int main(void) {
	init_tree();
	init_queue();

	int i, len = 0, index = 0;
	int sort[MAX];
	node* base = calloc(1, sizeof(node));
	

	bti_insert(name2int('F'), head, &len);
	bti_insert(name2int('B'), head, &len);
	bti_insert(name2int('O'), head, &len);
	bti_insert(name2int('A'), head, &len);
	bti_insert(name2int('D'), head, &len);
	bti_insert(name2int('L'), head, &len);
	bti_insert(name2int('C'), head, &len);
	bti_insert(name2int('G'), head, &len);
	bti_insert(name2int('M'), head, &len);
	bti_insert(name2int('H'), head, &len);
	bti_insert(name2int('N'), head, &len);
	bti_insert(name2int('K'), head, &len);			// ���� �ڷῡ �־��� Ʈ�� �׸���

	printf("\nOrigianl Tree\n");
	levelorder_traverse(head->left);				// levelorder traverse �� Ʈ�� ǥ��

	printf("\n\nL path is ");
	bti_search(name2int('L'), head, &len);			// L ��尡 ����ִ��� search

	bti_sort(head->left, sort, &index);				// sort�Լ��� ���� binary tree ���� sort�迭�� �����Ѵ�.

	index = 0;			// �ε��� �ʱ�ȭ

	base->left = balance(len, sort, &index);				// base�� balanced binary tree�� root��� �ּҸ� ��ȯ


	printf("\n\nBalanced Tree\n");
	levelorder_traverse(base->left);						// balance tree ���

	printf("\n\nL path is ");
	bti_search(name2int('L'), base, &len);			// L ��尡 ����ִ��� search
	printf("\n");

}