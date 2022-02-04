#include <stdio.h>
#include <stdlib.h>

FILE* fp;
#define MAX_NODE 100
#define SOURCE 'S'
#define SINK 'T'

int Capacity[MAX_NODE][MAX_NODE];		// �뷮�� ��Ÿ���� matrix
int Flow[MAX_NODE][MAX_NODE];			// ������ ��Ÿ���� matrix
int Residual[MAX_NODE][MAX_NODE];		// ������ ��Ÿ���� matrix (�뷮 - ����)
int check[MAX_NODE];					// �湮�� ��Ÿ���� �迭
int parent[MAX_NODE];					// �θ� ��Ÿ���� �迭
int path[MAX_NODE];						// augmenting path�� ��Ÿ���� �迭

int name2int(char c) {
	if (c == SOURCE) return 0;
	if (c == SINK) return 1;
	return c - 'A' + 2;			// 'A' ���� 2���� ����
}

char int2name(int i) {
	if (i == 0) return SOURCE;
	if (i == 1) return SINK;
	return i + 'A' - 2;			// 2 �� 'A'�� �ǹ�
}

int queue[MAX_NODE];			// queue�� ����ϱ� ���� �迭
int front, rear;

void init_queue() {
	front = rear = 0;
}

int queue_empty() {
	if (front == rear) return 1;
	else return 0;
}

void put(int k) {
	queue[rear] = k;
	rear = ++rear % MAX_NODE;
}

int get() {
	int i;
	i = queue[front];
	front = ++front % MAX_NODE;
	return i;
}

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// ����ġ�� ���⼺�� �����ϴ� adj matrix �׸���
	char vertex[3];
	int weight;
	int i, j, k;
	fscanf(fp, "%d %d", V, E);
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {
			a[i][j] = 0;
		}
	}

	for (i = 0; i < *V; i++) a[i][i] = 0;			// �ڱ��ڽ��� ����ġ�� 0�̴�.
	for (k = 0; k < *E; k++) {
		fscanf(fp, "%s %d", vertex, &weight);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = weight;
	}
}

void print_directed_adjmatrix(int a[][MAX_NODE], int V) {			// adj matrix ����ϱ�
	int i, j;
	printf("    ");
	for (i = 0; i < V; i++) printf("%4c", int2name(i));
	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%4c", int2name(i));
		for (j = 0; j < V; j++) printf("%4d", a[i][j]);
		printf("\n");
	}
}

void set_path() {				// augmenting path �� ����� �Լ�
	int* temp;
	int i, count = 0;
	temp = (int*)calloc(MAX_NODE, sizeof(int));		// temp �迭 ����
	i = name2int(SINK);			// SINK ������ ����
	while (i >= 0) {
		temp[count] = i;
		i = parent[i];	
		count++;				// temp �迭�� ù ���� SINK, ���� ���� �θ���� �Ž��� �ö󰡸� ����, �� augmenting path�� ����
	}
	for (i = 0; i < count; i++)
		path[i] = temp[count - i - 1];		// path�� augmenting path �� ������ �ٽ� ������ ����, �� augmenting path ����
	path[i] = -1;				// �������� -1 ���� (-1 �������� ��� ������, �� ��ħǥ�� �ǹ�)
	free(temp);
}

int get_augment_path(int a[][MAX_NODE], int V, int S, int T) {		// augmenting path ���ϱ� (BFS ����)
	int i, j;
	init_queue();
	for (i = 0; i < V; i++) {
		check[i] = 0;				// �湮 �ʱ�ȭ
		parent[i] = -1;				// �θ� �ʱ�ȭ  (0�� Source�̹Ƿ� -1�� �ʱ�ȭ)
	}
	i = name2int(S);					// Source���� ����
	if (check[i] == 0) {			// ���� i��带 �湮���� �ʾҴٸ� 
		put(i);				// i put
		check[i] = 1;				// i��� �湮 üũ
		while (!queue_empty()) {				// queue�� ���� ������ �ݺ�
			i = get();				// i�� ��������
			if (i == name2int(T)) break;		// ���� �����°� Sink�� �ݺ��� ���� (path �� ã����)
			for (j = 0; j < V; j++) {			// ��� ���(j)�� ���� ����
				if (a[i][j] != 0) {				// ���� j���� ������ ������
					if (check[j] == 0) {		// ���� j���� �湮���� �ʾ����� 
						put(j);					// j��� put (�湮)
						check[j] = 1;			// j��� �湮 üũ
						parent[j] = i;			// i��尡 �θ����� �Է�
					}
				}
			}
		}
	}
	set_path();					// ������ ���� parent �迭�� ���� augmenting path�� ã�� �Լ�
	if (i == name2int(T)) return 1;		// �ݺ����� �������� ��찡 i��尡 SINK, �� ��θ� ã�Ƽ� �������� ��� 1��ȯ
	else return 0;						// queue�� ��, �� ���̻� ��ΰ� ��� �������� ��� 0��ȯ
}

void construct_residual(int c[][MAX_NODE], int f[][MAX_NODE], int r[][MAX_NODE], int V) {		// residual network�� ���ϴ� �Լ�, V�� ��尳��
	int i, j;
	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			r[i][j] = c[i][j] - f[i][j];		// residual network = capacity - flow
}

void clear_matrix(int m[][MAX_NODE], int V) {		// matrix �ʱ�ȭ (0����)
	int i, j;
	for (i = 0; i < V; i++)
		for (j = 0; j < V; j++)
			m[i][j] = 0;
}

void network_flow(int c[][MAX_NODE], int f[][MAX_NODE], int r[][MAX_NODE], int V, int S, int T) {			// flow , residual graph�� ����� �Լ�
	int i, min;
	clear_matrix(f, V);
	clear_matrix(r, V);					// flow, residual network �ʱ�ȭ
	construct_residual(c, f, r, V);		// �켱������ residual network ���� (�̿ϼ�)
	while (get_augment_path(r, V, S, T)) {		// 1�̸� ��ΰ� �� �����ϱ� �ݺ��� �ݺ�, 0�̸� ��ΰ� ���ٴ� ���̴� �ݺ��� Ż��
		min = INT_MAX;					// �ּҰ� �ʱ�ȭ (���� ū������)
		for (i = 1; path[i] >= 0; i++)		// path�� SOURCE���� ������ SINK�� �ٴٸ��� ������ �ݺ�
			if (min > r[path[i - 1]][path[i]])
				min = r[path[i - 1]][path[i]];		// augment ��� �� ���� �뷮�� �������� ã�´�
		for (i = 1; path[i] >= 0; i++) {			// path�� SOURCE���� ������ SINK�� �ٴٸ��� ������ �ݺ�
			f[path[i - 1]][path[i]] = f[path[i - 1]][path[i]] + min;	// ���� = ���� ������ + min �뷮��
			f[path[i]][path[i - 1]] = -f[path[i - 1]][path[i]];			// ������ ������ = - ����
		}
		construct_residual(c, f, r, V);				// ���� flow, residual�� �ݺ�, augmenting path�� ���� �� ����
	}
}

int main(void) {
	int i;
	int V, E;
	fp = fopen("capacity.txt", "rt");
	input_directed_adjmatrix(Capacity, &V, &E);					// capacity graph �����

	printf("Capacity graph\n");
	print_directed_adjmatrix(Capacity, V);						// capacity graph ���

	printf("\nFlow graph\n");
	network_flow(Capacity, Flow, Residual, V, SOURCE, SINK);
	print_directed_adjmatrix(Flow, V);							// Flow graph ���


	printf("\nResidual graph\n");
	print_directed_adjmatrix(Residual, V);						// Residual graph ���
	printf("\n\n");

	fclose(fp);
}
