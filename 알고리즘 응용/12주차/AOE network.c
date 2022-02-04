#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;

int name2int(char c) {
	return c - 'A';
}

char int2name(int i) {
	return i + 'A';
}

typedef struct node {
	int vertex;
	int weight;
	struct node* next;
}node;

node* GL[MAX_NODE];

void input_directed_adjlist(node* a[], int* V, int* E) {		// directed �׷����� adj list �̿��ؼ� �׸���
	char vertex[3];
	int i, j, weight;
	node* t;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) a[i] = NULL;
	for (j = 0; j < *E; j++) {
		fscanf(fp, "%s %d", vertex, &weight);
		i = name2int(vertex[0]);
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[1]);
		t->weight = weight;
		t->next = a[i];
		a[i] = t;
	}
}

void print_directed_adjlist(node* a[MAX_NODE], int V) {			// �׷��� ���
	int i, j;
	node* p, * s;

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));
		p = a[i];
		for (s = p; s != NULL; s = s->next) printf("-> %3c:%2d ", int2name(s->vertex), s->weight);
	}
}

#define MAX 1000
int stack[MAX];
int top;
int check[MAX];

int push(int t) {
	if (top >= MAX - 1) {
		printf("stack overflow!\n");
		return -1;
	}
	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf("stack underflow! \n");
		return -1;
	}
	return stack[top--];
}

int stack_empty() {
	return (top < 0);
}

void init_stack() {
	top = -1;
}															// ���ü���

typedef struct head {
	int count;
	int weight;
	struct node* next;
}head;

head network[MAX_NODE];
int earliest[MAX_NODE];
int latest[MAX_NODE];



void set_count_indegree(head net[], int V) {
	int i, j;
	int count;
	node* t;
	for (i = 0; i < V; i++) {
		count = 0;
		for (j = 0; j < V; j++) {
			for (t = net[j].next; t; t = t->next) {
				if (t->vertex == i)count++;
			}
		}
		net[i].count = count;
	}
}

void set_count_outdegree(head net[], int V) {
	int i;
	int count;
	node* t;
	for (i = 0; i < V; i++) {
		count = 0;
		for (t = net[i].next; t; t = t->next) {
			count++;
		}
		net[i].count = count;
	}
}

void forward_state(head net[], int V) {					// earliest �� ����ϴ� �Լ�
	int i, j, k;
	node* ptr;

	init_stack();
	set_count_indegree(net, V);

	for (i = 0; i < V; i++) earliest[i] = 0;			// �� ������ earliest �ʱ�ȭ
	for (i = 0; i < V; i++)
		if (!net[i].count) push(i);						// �������� ���� ������ push
	for (i = 0; i < V; i++) {
		if (!stack_empty()) {
			j = pop();
			for (ptr = net[j].next; ptr; ptr = ptr->next) {		// j������ ���Ἲ�е� ����
				k = ptr->vertex;								// k = j������ ���� ����
				net[k].count--;									// j��带 �湮������ k����(�������) ���������� ����
				if (!net[k].count)				// ������������ ������
					push(k);					// k��� push(�湮)
				if (earliest[k] < earliest[j] + ptr->weight)		// k��� ������ earliest ����ġ���� �ٸ� ���(j��� ���� ����ġ + <j, k>) �� ũ��
					earliest[k] = earliest[j] + ptr->weight;		// earliest[k] ����, k��� ���� ���� ���� ū ���� �����
			}										// accumulation
		}
	}
}

void backward_state(head net[], int V) {
	int i, j, k, l;
	node* ptr;

	init_stack();
	set_count_outdegree(net, V);

	for (i = 0; i < V; i++) latest[i] = earliest[V - 1];		// latest�� earliest�� ���� ū ������ �ʱ�ȭ
	for (i = 0; i < V; i++)
		if (!net[i].count) push(i);								// ������������ ������ push
	for (i = 0; i < V; i++) {
		if (!stack_empty()) {
			j = pop();
			for (l = 0; l < V; l++) {				// ��� ��� ����
				for (ptr = net[l].next; ptr; ptr = ptr->next) {		// �� ����� ���Ἲ���� �����Ѵ�
					if (ptr->vertex == j) {			// ���� ������ j���(�湮���)�� �ִ� ���
						k = l;						// k���� j��� ��������̴�.
						net[k].count--;				// k����� �������� ���� ���ҽ�Ų��.
						if (!net[k].count)
							push(k);				// ���� k����� ������������ ������ k��� push
						if (latest[k] > latest[j] - ptr->weight)		// ���� k����� latest ����ġ���� �ٸ� ���(latest[j] - <k,j>) �� ������
							latest[k] = latest[j] - ptr->weight;		// latest[k] ����, k��� ���� �ڿ��� ���� ���� ���� ���� ����ȴ�
					}								// accumulation
				}
			}
		}
	}
}

char yes_no(int i) {				// �������� 0�̸� Y, �ٸ��� N�� ��ȯ
	if (i == 0) return 'Y';
	return 'N';
}

void print_critical_activity(head net[], int V) {							// critical activity�� ã�� �Լ�
	int i, j;
	printf("\nEdge     Early   Late   Late-Early   �Ӱ��� ");
	node* t;
	for (i = 0; i < V; i++) {							// ��� ��忡 ���Ͽ�
		for (t = net[i].next; t; t = t->next) {					// i����� ���� ��忡 ���Ͽ�
			printf("\n<%c %c>   %3d     %3d     %3d            %c", int2name(i), int2name(t->vertex), earliest[i],		// i���, i������, early<i, i����> ���
				latest[t->vertex]-t->weight, latest[t->vertex]-t->weight-earliest[i],	// late<i, i����>, early-late ���, late�� ��� latest[i������]- <i, i������> ���� ���, <i, i������> ����ġ�� i�����尡 �����ִ�.
				yes_no(latest[t->vertex] - t->weight - earliest[i]));					// early- late ���� �Ǻ��� ������ Y, �ٸ��� N ���
		}
	}
}

void main() {
	fp = fopen("graph_AOE.txt", "rt");

	int V, E;
	int i;

	input_directed_adjlist(GL, &V, &E);			// ���Ϸ� ���� ���� �׷��� ���
	printf("\n\nOriginal graph\n");
	print_directed_adjlist(GL, V);

	for (i = 0; i < V; i++) {
		network[i].next = GL[i];				// network�� GL �׷��� ���� ����
	}

	forward_state(network, V);					// earliest	 �����
	backward_state(network, V);					// latest	�����

	printf("\n\nearlist\n");
	for (i = 0; i < V; i++) {
		printf("%3d", earliest[i]);				// earliest ���
	}

	printf("\nlatest\n");
	for (i = 0; i < V; i++) {	
		printf("%3d", latest[i]);				// latest ���
	}
	printf("\n\n");

	print_critical_activity(network, V);		// criticial_activity ���
	fclose(fp);
}