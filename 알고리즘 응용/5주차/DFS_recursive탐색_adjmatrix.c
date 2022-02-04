#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int GM[MAX_NODE][MAX_NODE];										// adjmatrix �� ��Ÿ���� ���� ���

int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// adjmatrix�� �����ϴ� �Լ��̴�.
	char vertex[3];													// AB, AC �� ���� ������ �����ϴ� �迭
	int i, j, k;
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);											// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM �迭�� 0���� ���� �ʱ�ȭ�Ѵ�.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 1;							// �밢 ������ ��� 1�� �ʱ�ȭ�Ѵ�.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);										// ������ ������ vertex�迭�� �����Ѵ�. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// �Է¹��� ���� ������ ����ȭ�Ѵ�.
		a[i][j] = 1;
		a[j][i] = 1;												// �Է¹��� ������ �������� �ش��ϴ� ��� ������ 1�� üũ�Ѵ�.(���� �մ´�.)
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix�� ����ϴ� �Լ��̴�.
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%3c", int2name(i));				// ��Ŀ��� ���� �ش��ϴ� ������ ���е��� ����Ѵ�. (���� ������)

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%3c", int2name(i));									// ��Ŀ��� �࿡ �ش��ϴ� ������ ���е��� ����Ѵ�. (���� ������)
		for (j = 0; j < V; j++) printf("%3d", a[i][j]);				// ��� ���е��� �ش��ϴ� 0�� 1�� ��Ÿ����.
		printf("\n");
	}
}

int check[MAX_NODE];

void DFS_recur_matrix(int a[][MAX_NODE], int V, int i) {
	int j;
	check[i] = 1;
	printf("%3c->", int2name(i));
	for (j = 0; j < V; j++) {
		if (a[i][j] != 0) {
			if (check[j] == 0) {
				DFS_recur_matrix(a, V, j);
			}
		}
	}
}

void DFS_adjmatrix(int a[][MAX_NODE], int V) {
	int i;
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) DFS_recur_matrix(a, V, i);
	}
}


void main() {
	int V, E;
	input_adjmatrix(GM, &V, &E);
	DFS_adjmatrix(GM, V);
	printf("\n");
}