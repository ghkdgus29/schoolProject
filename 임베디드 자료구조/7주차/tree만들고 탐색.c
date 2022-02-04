#include <stdio.h>
#include <stdlib.h>


#define MAX 100

typedef struct node {				// 트리를 만들기 위한 doubly linked node
	int key;
	struct node* left;
	struct node* right;
}node;

node* head, * tail;					// 트리를 구성하기 위한 head, tail 노드
node* stack[MAX];					// 트리를 구성하기 위한 스택으로 노드의 주소를 저장
node* queue[MAX];					// level order traverse 를 위한 큐로 노드의 주소 저장
int top, front, rear;				// 스택, 큐를 위한 인덱스

void init_stack() {					// 스택 초기화
	top = -1;
}

void init_queue() {					// 큐 초기화
	front = rear = 0;
}

int is_stack_empty() {				// 스택이 비었는지 검사, 비었으면 1 반환
	return (top == -1);
}

int is_queue_empty() {				// 큐가 비었는지 검사, 비었으면 1반환
	return (front == rear);
}

node* push(node* t) {				// 스택에 push 를 위한 함수, 트리주소를 집어넣는다.
	stack[++top] = t;
	return t;
}

node* pop() {						// 스택값 pop 을 위한 함수 , 트리주소를 꺼낸다.
	return stack[top--];
}

node* put(node* t) {				// 큐에 트리주소를 넣기위한 put 함수
	queue[rear] = t;
	rear = ++rear % MAX;
	return t;
}

node* get() {						// 큐에서 트리주소를 꺼내기 위한 get함수
	node* t;
	t = queue[front];
	front = ++front % MAX;
	return t;
}

void init_tree() {								// 트리 초기화 , head, tail 노드를 형성하고 tail에 모두 연결
	head = (node*)calloc(1, sizeof(node));
	tail = (node*)calloc(1, sizeof(node));

	head->left = tail;
	head->right = tail;
	tail->left = tail;
	tail->right = tail;
}

int is_operator(int k) {										// 연산자를 int 값으로 반환해 조건문을 동작하기 위한 함수
	return (k == '+' || k == '-' || k == '*' || k == '/');
}

void visit(node* t) {								// 방문한 트리의 값을 출력하는 함수
	printf("%c", t->key);
}

node* make_parse_tree(char* p) {					// 트리를 형성하는 함수, 스트링의 형태로 postfix notation 값을 받는다.
	node* t;
	while (*p) {
		while (*p == ' ') p++;						// 만약 postfix notation 값의 공백이 있으면 앞으로 칸을 옮긴다.
		t = (node*)calloc(1, sizeof(node));			// 트리 노드 형성
		t->key = *p;								// 노드에 operator, operand 등을 집어넣는다.
		t->left = tail;								
		t->right = tail;
		if (is_operator(*p)) {						// 만약 현재 postfix notation으로 부터 operator 를 받으면 
			t->right = pop();						// operator 에 피연산자를 양쪽 노드에 연결한다.
			t->left = pop();
		}
		push(t);									// postfix notation 으로부터 받은 값을 저장한 트리노드주소를 스택에 집어넣는다.
		p++;										// 앞으로 이동
	}
	return pop();									// 모든 연산이 끝나면 루트 주소를 반환한다.
}

void preorder_traverse(node* t) {					// preorder_traverse 를 위한 함수
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

void levelorder_traverse(node* t) {					// level order traverse 를 위한 함수
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
		head->right = make_parse_tree(post);				// 트리 형성,  head의 우측 노드에 루트주소를 연결
		printf("\nPre-order traverse -> ");
		preorder_traverse(head->right);						// preorder traverse

		printf("\nLevel-order traverse -> ");
		levelorder_traverse(head->right);					// level order traverse
	}

}