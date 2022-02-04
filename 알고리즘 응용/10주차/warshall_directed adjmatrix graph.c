#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100
FILE* fp;


int name2int(char c) {					// ���ĺ��� ������ �ٲٴ� �Լ�
	return c - 'A';
}

char int2name(int i) {					// ������ ���ĺ����� �ٲٴ� �Լ�
	return i + 'A';
}

int GM[MAX_NODE][MAX_NODE];										// adjmatrix �� ��Ÿ���� ���� ���

void input_directed_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// directed adjmatrix�� �����ϴ� �Լ��̴�.
	char vertex[3];													// AB, AC �� ���� ������ �����ϴ� �迭
	int i, j, k;
	printf("\nInput number of node & edge\n");
	fscanf(fp, "%d %d", V, E);											// Ű�����Է��� ���� V(��������) , E(���м�)�� �Է¹޴´�.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM �迭�� 0���� ���� �ʱ�ȭ�Ѵ�.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 0;							// �밢 ������ ��� 0���� �ʱ�ȭ�Ѵ�.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		fscanf(fp, "%s", vertex);										// ������ ������ vertex�迭�� �����Ѵ�. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// �Է¹��� ���� ������ ����ȭ�Ѵ�.
		a[i][j] = 1;

	}
}

void print_directed_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix�� ����ϴ� �Լ��̴�.
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

void warshall(int a[][MAX_NODE], int V) {		// �׷����� reachability �� ã�� �Լ�
	int x, y, k;

	for (y = 0; y < V; y++) {		// ��� ��忡 ���� �ݺ�
		for (x = 0; x < V; x++) {		// ��� ��忡 ���� �ݺ�
			if (a[x][y] == 1) {				// x y ��尡 ����Ǿ��ְ�
				for (k = 0; k < V; k++) {		// ��� k��忡 ���� ���� 
					if (a[y][k] == 1) a[x][k] = 1;		// y k ��尡 ����Ǿ��ִٸ�, x k ���� ���� �����ϴ�.
				}
			}
		}
	}
}

void main() {
	fp = fopen("directed_graph.txt", "rt");
	int V, E;

	input_directed_adjmatrix(GM, &V, &E);			// �׷��� �����

	printf("\n\nOriginal graph\n");
	print_directed_adjmatrix(GM, V);				// �׷��� ���

	printf("\n\n\nReachability graph\n");
	warshall(GM, V);								// reachability ����
	print_directed_adjmatrix(GM, V);

	fclose(fp);
}