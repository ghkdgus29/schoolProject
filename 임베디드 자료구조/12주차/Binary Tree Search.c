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

node* bti_delete1(int key, node* base, int* num) {		// �Է¹��� key���� ���� Ʈ�� ��带 �����ϴ� �Լ�
	node* parent, * son, * del, * nexth;
	parent = base;
	del = base->left;						// base�� ���ʿ� Ʈ���� �Ѹ��� �ִ�. parent - del ���� �θ� - �ڽ� �����̴�
	while (key != del->key && del != NULL) {			// �Է¹��� key���� ���� ���� ã�ų� NULL�� ���� �ٴٸ������� �ݺ�
		parent = del;					// ���� ��带 parent��忡 ����
		if (del->key > key) del = del->left;		// ���� ����� key���� �Է¹��� key������ ũ�� ���� �Ʒ��� �̵�
		else del = del->right;						// ���� ����� key���� �Է¹��� key������ ������ ������ �Ʒ��� �̵�
	}		// ������� ����� parent, del �ּҸ� ã�� �ݺ���

	if (del == NULL) return NULL;		// ������� ��带 ��ã���� NULL ��ȯ

	if (del->left == NULL && del->right == NULL)		// case 1 ) ������� ��尡 external node (Ʈ���� ����)
		son = NULL;			// son�� NULL ���� (���߿� parent node�� �����Ұ���)

	else if (del->left != NULL && del->right != NULL) {		// case 3) child node�� ���� ����
		nexth = del->right;				// ������� ����� ������ �ڽĳ�带 nexth���� ����
		if (nexth->left != NULL) {		// case 3-2) ������� node�� root ����϶� (����� ����� ���� �ڽĳ����, �����ʳ�尡 ���� �ڽ��� �� �������)
			while (nexth->left->left != NULL) nexth = nexth->left;	// ���������� , ���� ���ʿ� �ִ� ����� �θ� ��带 nexth�� ����
			son = nexth->left;			// ����������, ���� ���ʿ� �ִ� ��带 son���� ����
			nexth->left = son->right;	// nexth�� son����� ������ ���е��� ����
			son->left = del->left;
			son->right = del->right;		// root��带 son ���� ��ü (������ �ٲ�)
		}
		else {							// case 3-1) ������� ��尡 not root�϶� (����� ����� ���� �ڽĳ����, �����ʳ�尡 ���� �ڽ��� �� ���� ���)
			son = nexth;				// son ���� ����� ����� ������ �ڽĳ��
			son->left = del->left;		// ����� ��带 son ���� ��ü (������ �ٲ�)
		}
	}
	else {						// case 2) ����� ��尡 child�� �ϳ��ۿ� ���� ���
		if (del->left != NULL) son = del->left;		// ����� ����� �ڽ��� ���� �Ʒ��� ���, ���� �Ʒ��� son���� ����
		else son = del->right;						// ����� ����� �ڽ��� ������ �Ʒ��� ���, ������ �Ʒ��� son���� ����
	}
	if (key < parent->key || parent == base)		// case 1,2,3�� ���� �����ؾ� �ϴ� �۾�, parent�� son ��带 ����
		parent->left = son;					// parent���� son���� ũ�� son�� ���ʾƷ� ��ġ
	else
		parent->right = son;				// parent���� son���� ������ son�� �����ʾƷ� ��ġ


	free(del);		// del ��� �����
	(*num)--;		// ������ ���� ���̱�

	return parent;		// ���� ����� �θ��� ��ȯ


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

int main(void) {
	init_tree();
	init_queue();

	int i, len = 0;

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
	levelorder_traverse(head->left);

	printf("\n L path is ");
	bti_search(name2int('L'), head, &len);

	bti_delete1(name2int('F'), head, &len);
	printf("\nDelete F (root)\n");
	levelorder_traverse(head->left);

	printf("\nDelete M\n");
	bti_delete1(name2int('M'), head, &len);
	levelorder_traverse(head->left);

	printf("\nDelete B\n");
	bti_delete(name2int('B'), head, &len);
	levelorder_traverse(head->left);

	printf("\nDelete A\n");
	bti_delete(name2int('A'), head, &len);
	levelorder_traverse(head->left);



}