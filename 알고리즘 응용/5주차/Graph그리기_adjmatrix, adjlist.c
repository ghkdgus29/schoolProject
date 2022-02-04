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

typedef struct node {												// adjlist 를 위한 node 구조체이다.
	int vertex;														// vertex는 꼭지점 성분을 나타내고
	struct node* next;												// next는 linked list의 다음번 노드 주소를 나타낸다.
}node;

node* GL[MAX_NODE];											// NODE구조체의 주소를 저장하는 GL 배열을 선언한다.

void input_adjlist(node* a[], int* V, int* E) {				// adjlist를 구현하는 함수를 나타낸다.
	char vertex[3];										    // AB, AC 와 같은 선분을 저장하는 배열	
	int i, j;
	node* t;												// linked list 를 만들기 위한 t를 선언한다.
	printf("\nInput number of node & edge\n");
	scanf("%d %d", V, E);									// 키보드입력을 통해 V(꼭짓점수) , E(선분수)를 입력받는다.
	for (i = 0; i < *V; i++) a[i] = NULL;					// GL의 요소들을 NULL로 초기화시킨다.
	for (j = 0; j < *E; j++) {
		printf("\nInput two node consist of edge ->");
		scanf("%s", vertex);								// 선분정보를 vertex 배열에 저장한다.
		i = name2int(vertex[0]);							// 첫번째 꼭지점을 정수화하여 i에 저장한다.
		t = (node*)malloc(sizeof(node));					// t주소에 node구조체만큼의 공간을 할당한다.
		t->vertex = name2int(vertex[1]);					// t주소의 node 구조체에 두번째 꼭지점값을 정수화하여 저장한다.
		t->next = a[i];										// next에 NULL 값을 저장한다.
		a[i] = t;											// GL의 i번째 요소에 t의 주소를 저장한다.

		i = name2int(vertex[1]);							// AB = BA 이므로 양방향으로 저장하기 위해 1과 0을 뒤집어서 한번 더 해준다.
		t = (node*)malloc(sizeof(node));
		t->vertex = name2int(vertex[0]);
		t->next = a[i];
		a[i] = t;
	}
}

void print_adjlist(int a[MAX_NODE], int V) {				// adjlist를 출력하기 위한 함수이다. GL배열을 매개변수로 받는다.
	int i, j;
	node* p, * s;											// linked list를 이용하기 위한 p와 free를 위한 s를 선언한다.

	for (i = 0; i < V; i++) {
		printf("\n%3c ", int2name(i));						// A B C D .. 와 같은 vertex 성분들을 왼쪽 시작부분에 단어화하여 출력한다.
		p = GL[i];											// p에 GL[i]에 해당하는 주소값을 저장한다. 즉 linked list에 접근하기 위함이다.
		while (p != NULL) {									// 만약 p주소가 NULL 값을 가리키면 while문을 빠져나간다.
			printf("-> %3c ", int2name(p->vertex));			// p주소의 node 구조체의 vertex값을 단어화하여 출력한다.
			s = p;											// p주소값을 s주소에 할당한다.
			p = p->next;									// p주소값을 다음 노드로 옮긴다.
			free(s);										// 출력이 끝난 노드는 제거한다.
		}
	}
}

void main() {
	int V, E;
	input_adjmatrix(GM, &V, &E);
	print_adjmatrix(GM, V);

	printf("\n\n");

	input_adjlist(GL, &V, &E);
	print_adjlist(GL, V);
	printf("\n");


}