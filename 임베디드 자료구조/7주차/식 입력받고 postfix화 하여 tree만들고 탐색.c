#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct node {
	int key;
	struct node* left;
	struct node* right;
}node;

node* head, * tail;
node* stack[MAX];
node* queue[MAX];
int top, front, rear;

void init_stack() {
	top = -1;
}

void init_queue() {
	front = rear = 0;
}

int is_stack_empty() {
	return (top <= -1);
}

int is_queue_empty() {
	return (front == rear);
}

node* push(node* t) {
	stack[++top] = t;
	return t;
}

node* pop() {
	return stack[top--];
}

node* put(node* t) {
	queue[rear] = t;
	rear = ++rear % MAX;
	return t;
}

node* get() {
	node* t;
	t = queue[front];
	front = ++front % MAX;
	return t;
}

void init_tree() {
	head = (node*)calloc(1, sizeof(node));
	tail = (node*)calloc(1, sizeof(node));

	head->left = tail;
	head->right = tail;
	tail->left = tail;
	tail->right = tail;
}

int is_operator(int k) {
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

void visit(node* t) {
	printf("%c", t->key);
}

node* make_parse_tree(char* p) {
	node* t;
	while (*p) {
		while (*p == ' ') p++;
		t = (node*)calloc(1, sizeof(node));
		t->key = *p;
		t->left = tail;
		t->right = tail;
		if (is_operator(*p)) {
			t->right = pop();
			t->left = pop();
		}
		push(t);
		p++;
	}
	return pop();
}

void preorder_traverse(node* t) {
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

void levelorder_traverse(node* t) {
	put(t);

	while (!is_queue_empty()) {
		t = get();
		visit(t);

		if (t->left != tail) put(t->left);
		if (t->right != tail) put(t->right);
	}
}

int _stack[MAX];
int _top;

void _init_stack() {
	_top = -1;
}

int _push(int t) {
	if (_top >= MAX - 1) {
		printf("\nStack overflow !!!\n");
		return -1;
	}

	_stack[++_top] = t;
	return t;
}

int _pop() {
	if (_top < 0) {
		printf("\nStack underflow !!!\n");
		return -1;
	}
	return _stack[_top--];
}

int get_stack_top() {
	return (_top < 0) ? -1 : _stack[_top];
}

int is_op(int k) {
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

int precedence(int op) {
	if (op == '(') return 0;
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/') return 2;
	else return 3;
}

int _is_stack_empty() {
	if (_top < 0) return 1;
	return 0;
}

void postfix(char* dst, char* src) {
	char c;
	_init_stack();
	while (*src) {
		if (*src == '(') {
			_push(*src);
			src++;
		}
		else if (*src == ')') {
			while (get_stack_top() != '(') {
				*dst++ = _pop();
				*dst++ = ' ';
			}
			_pop();
			src++;
		}
		else if (is_op(*src)) {
			while (!_is_stack_empty() && precedence(get_stack_top()) >= precedence(*src)) {
				*dst++ = _pop();
				*dst++ = ' ';
			}
			_push(*src);
			src++;
		}
		else if (*src >= '0' && *src <= '9') {
			do {
				*dst++ = *src++;
			} while (*src >= '0' && *src <= '9');
			*dst++ = ' ';
		}
		else src++;
	}
	while (!_is_stack_empty()) {
		*dst++ = _pop();
		*dst++ = ' ';
	}
	dst--;
	*dst = 0;
}

void main() {
	char src[256];
	char exp[256];

	init_stack();
	init_queue();
	init_tree();


	while (1) {
		printf("\nInput your equation -> ");
		gets(src);
		postfix(exp, src);

		printf("\n\npostfix expression -> %s", exp);

		if (*exp == NULL) break;
		head->right = make_parse_tree(exp);
		printf("\nPre-order traverse -> ");
		preorder_traverse(head->right);

		printf("\nLevel-order traverse -> ");
		levelorder_traverse(head->right);
	}
}