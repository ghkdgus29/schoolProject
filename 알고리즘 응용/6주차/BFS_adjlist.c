#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int check[MAX_NODE];

int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

typedef struct node {												// adjlist �� ���� node ����ü�̴�.
	int vertex;														// vertex�� ������ ������ ��Ÿ����
	struct node* next;												// next�� linked list�� ������ ��� �ּҸ� ��Ÿ����.
}node;

node* GL[MAX_NODE];											// NODE����ü�� �ּҸ� �����ϴ� GL �迭�� �����Ѵ�.

void input_adjlist(node* a[], int* V, int* E) {				// adjlist�� �����ϴ� �Լ��� ��Ÿ����.
	char vertex[3];										    // AB, AC �� ���� ������ �����ϴ� �迭	
	int i, j;
	node* t;												// linked list �� ����� ���� t�� �����Ѵ�.
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);									// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL�� ��ҵ��� NULL�� �ʱ�ȭ��Ų��.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);								// ���������� vertex �迭�� �����Ѵ�.
		i = name2int(vertex[0]);							// ù��° �������� ����ȭ�Ͽ� i�� �����Ѵ�.
		t = (node*)malloc(sizeof(node));					// t�ּҿ� node����ü��ŭ�� ������ �Ҵ��Ѵ�.
		t->vertex = name2int(vertex[1]);					// t�ּ��� node ����ü�� �ι�° ���������� ����ȭ�Ͽ� �����Ѵ�.
		t->next = a[i];										// next�� NULL ���� �����Ѵ�.
		a[i] = t;											// GL�� i��° ��ҿ� t�� �ּҸ� �����Ѵ�.

		i = name2int(vertex[1]);							// AB = BA �̹Ƿ� ��������� �����ϱ� ���� 1�� 0�� ����� �ѹ� �� ���ش�.
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
	}
}

void print_adjlist(int a[MAX_NODE], int V) {				// adjlist�� ����ϱ� ���� �Լ��̴�. GL�迭�� �Ű������� �޴´�.
	int i, j;
	node* p;											// linked list�� �̿��ϱ� ���� p�� free�� ���� s�� �����Ѵ�.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. �� ���� vertex ���е��� ���� ���ۺκп� �ܾ�ȭ�Ͽ� ����Ѵ�.
		p = GL[i];											// p�� GL[i]�� �ش��ϴ� �ּҰ��� �����Ѵ�. �� linked list�� �����ϱ� �����̴�.
		while (p != NULL) {									// ���� p�ּҰ� NULL ���� ����Ű�� while���� ����������.
			printf("-> %3c ", int2name(p->vertex));			// p�ּ��� node ����ü�� vertex���� �ܾ�ȭ�Ͽ� ����Ѵ�.
													// p�ּҰ��� s�ּҿ� �Ҵ��Ѵ�.
			p = p->next;									// p�ּҰ��� ���� ���� �ű��.
													// ����� ���� ���� �����Ѵ�.
		}
	}
}


typedef struct dnode {
	int key;
	struct dnode* prev;
	struct dnode* next;
}dnode;

dnode* head, * tail;

void init_queue() {
	head = (dnode*)calloc(1, sizeof(dnode));
	tail = (dnode*)calloc(1, sizeof(dnode));
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

int put(int k) {
	dnode* t;
	if ((t = (dnode*)malloc(sizeof(dnode))) == NULL) {
		printf("Out of memory!\n");
		return -1;
	}

	t->key = k;
	tail->prev->next = t;
	t->prev = tail->prev;
	tail->prev = t;
	t->next = tail;
	return k;
}

int get() {
	dnode* t;
	int k;
	t = head->next;
	if (t == tail) {
		printf("\nQueue underflow\n");
		return -1;
	}
	k = t->key;
	head->next = t->next;
	t->next->prev = head;
	free(t);
	return k;
}

void visit(int i) {
	printf("%3c->", int2name(i));
}

int queue_empty() {
	if (head->next == tail) return 1;
	return 0;
}

void BFS_adjlist(node* a[], int V) {
	int i;
	int cnt = 0;
	node* t;
	init_queue();
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) {
			++cnt;
			put(i);
			check[i] = 1;
			while (!queue_empty()) {
				i = get();
				visit(i);
				for (t = a[i]; t != NULL; t = t->next) {
					if (check[t->vertex] == 0) {
						put(t->vertex);
						check[t->vertex] = 1;
					}
				}
			}
		}
	}
	printf("\n�׷��� ���� : %d", cnt);
}

void main() {
	int V, E;

	input_adjlist(GL, &V, &E);
	print_adjlist(GL, V);

	printf("\n\n");

	BFS_adjlist(GL, V);

	node* p, * s;								// free�� ���� �޸� ������ �����ش�.
	int i;
	for (i = 0; i < V; i++) {
		p = GL[i];
		while (p != NULL) {
			s = p;
			p = p->next;
			free(s);
		}
	}

}