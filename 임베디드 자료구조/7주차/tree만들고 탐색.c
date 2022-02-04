#include <stdio.h>
#include <stdlib.h>


#define MAX 100

typedef struct node {				// Ʈ���� ����� ���� doubly linked node
	int key;
	struct node* left;
	struct node* right;
}node;

node* head, * tail;					// Ʈ���� �����ϱ� ���� head, tail ���
node* stack[MAX];					// Ʈ���� �����ϱ� ���� �������� ����� �ּҸ� ����
node* queue[MAX];					// level order traverse �� ���� ť�� ����� �ּ� ����
int top, front, rear;				// ����, ť�� ���� �ε���

void init_stack() {					// ���� �ʱ�ȭ
	top = -1;
}

void init_queue() {					// ť �ʱ�ȭ
	front = rear = 0;
}

int is_stack_empty() {				// ������ ������� �˻�, ������� 1 ��ȯ
	return (top == -1);
}

int is_queue_empty() {				// ť�� ������� �˻�, ������� 1��ȯ
	return (front == rear);
}

node* push(node* t) {				// ���ÿ� push �� ���� �Լ�, Ʈ���ּҸ� ����ִ´�.
	stack[++top] = t;
	return t;
}

node* pop() {						// ���ð� pop �� ���� �Լ� , Ʈ���ּҸ� ������.
	return stack[top--];
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

void init_tree() {								// Ʈ�� �ʱ�ȭ , head, tail ��带 �����ϰ� tail�� ��� ����
	head = (node*)calloc(1, sizeof(node));
	tail = (node*)calloc(1, sizeof(node));

	head->left = tail;
	head->right = tail;
	tail->left = tail;
	tail->right = tail;
}

int is_operator(int k) {										// �����ڸ� int ������ ��ȯ�� ���ǹ��� �����ϱ� ���� �Լ�
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

void visit(node* t) {								// �湮�� Ʈ���� ���� ����ϴ� �Լ�
	printf("%c", t->key);
}

node* make_parse_tree(char* p) {					// Ʈ���� �����ϴ� �Լ�, ��Ʈ���� ���·� postfix notation ���� �޴´�.
	node* t;
	while (*p) {
		while (*p == ' ') p++;						// ���� postfix notation ���� ������ ������ ������ ĭ�� �ű��.
		t = (node*)calloc(1, sizeof(node));			// Ʈ�� ��� ����
		t->key = *p;								// ��忡 operator, operand ���� ����ִ´�.
		t->left = tail;								
		t->right = tail;
		if (is_operator(*p)) {						// ���� ���� postfix notation���� ���� operator �� ������ 
			t->right = pop();						// operator �� �ǿ����ڸ� ���� ��忡 �����Ѵ�.
			t->left = pop();
		}
		push(t);									// postfix notation ���κ��� ���� ���� ������ Ʈ������ּҸ� ���ÿ� ����ִ´�.
		p++;										// ������ �̵�
	}
	return pop();									// ��� ������ ������ ��Ʈ �ּҸ� ��ȯ�Ѵ�.
}

void preorder_traverse(node* t) {					// preorder_traverse �� ���� �Լ�
	if (t != tail) {
		visit(t);
		preorder_traverse(t->left);
		preorder_traverse(t->right);
	}
}

void inorder_traverse(node* t) {
	if (t != tail) {
		inorder_traverse(t->left);
		visit(t);
		inorder_traverse(t->right);
	}
}

void postorder_traverse(node* t) {
	if (t != tail) {
		postorder_traverse(t->left);
		postorder_traverse(t->right);
		visit(t);
	}
}

void levelorder_traverse(node* t) {					// level order traverse �� ���� �Լ�
	put(t);

	while (!is_queue_empty()) {
		t = get();
		visit(t);

		if (t->left != tail) put(t->left);
		if (t->right != tail) put(t->right);
	}
}

void main() {
	char post[256];
	init_stack();
	init_queue();
	init_tree();

	while (1) {
		printf("\n\nInput postfix expression -> ");
		gets(post);

		if (*post == NULL) break;
		head->right = make_parse_tree(post);				// Ʈ�� ����,  head�� ���� ��忡 ��Ʈ�ּҸ� ����
		printf("\nPre-order traverse -> ");
		preorder_traverse(head->right);						// preorder traverse

		printf("\nLevel-order traverse -> ");
		levelorder_traverse(head->right);					// level order traverse
	}

}