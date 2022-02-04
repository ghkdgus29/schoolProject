#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int GM[MAX_NODE][MAX_NODE];										// adjmatrix 를 나타내기 위한 행렬

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
}

void input_adjmatrix(int a[][MAX_NODE], int* V, int* E) {			// adjmatrix를 구현하는 함수이다.
	char vertex[3];													// AB, AC 와 같은 선분을 저장하는 배열
	int i, j, k;
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);											// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) {
		for (j = 0; j < *V; j++) {									// GM 배열을 0으로 전부 초기화한다.
			a[i][j] = 0;
		}
	}
	for (i = 0; i < *V; i++) a[i][i] = 1;							// 대각 성분을 모두 1로 초기화한다.
	for (k = 0; k < *E; k++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);										// 선분의 정보를 vertex배열에 저장한다. 
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);									// 입력받은 선분 정보를 정수화한다.
		a[i][j] = 1;
		a[j][i] = 1;												// 입력받은 정보를 바탕으로 해당하는 행렬 성분을 1로 체크한다.(선을 잇는다.)
	}
}

void print_adjmatrix(int a[][MAX_NODE], int V) {					// adjmatrix를 출력하는 함수이다.
	int i, j;
	printf("   ");
	for (i = 0; i < V; i++) printf("%3c", int2name(i));				// 행렬에서 열에 해당하는 꼭지점 성분들을 출력한다. (맨위 가로줄)

	printf("\n");

	for (i = 0; i < V; i++) {
		printf("%3c", int2name(i));									// 행렬에서 행에 해당하는 꼭지점 성분들을 출력한다. (왼쪽 세로줄)
		for (j = 0; j < V; j++) printf("%3d", a[i][j]);				// 행렬 성분들을 해당하는 0과 1로 나타낸다.
		printf("\n");
	}
}

int check[MAX_NODE];

#define MAX 100
int stack[MAX];
int top;

int push(int t) {
	if (top >= MAX - 1) {
		printf(" stack overflow! \n");
		return -1;
	}
	stack[++top] = t;
	return t;
}

int pop() {
	if (top < 0) {
		printf(" stack underflow! \n");
		return -1;
	}
	return stack[top--];
}

int stack_empty() {
	if (top < 0) return 1;
	return 0;
}

void nrDFS_adjmatrix(int a[][MAX_NODE], int V) {
	int i, j;
	top = -1; // init_stack();
	for (i = 0; i < V; i++) check[i] = 0;
	for (i = 0; i < V; i++) {
		if (check[i] == 0) {
			push(i);
			check[i] = 1;
			while (!stack_empty()) {
				i = pop();
				printf("%3c->", int2name(i));
				for (j = 0; j < V; j++) {
					if (a[i][j] != 0) {
						if (check[j] == 0) {
							push(j);
							check[j] = 1;
						}
					}
				}
			}
		}
	}
}



void main() {
	int V, E;
	input_adjmatrix(GM, &V, &E);
	nrDFS_adjmatrix(GM, V);
	printf("\n");
}