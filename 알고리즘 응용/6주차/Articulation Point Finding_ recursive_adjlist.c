#include <stdio.h>
#include <stdlib.h>
#define MAX_NODE 100

int name2int(char c) {					// 알파벳을 정수로 바꾸는 함수
	return c - 'A';
}

char int2name(int i) {					// 정수를 알파벳으로 바꾸는 함수
	return i + 'A';
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

int check[MAX_NODE];
int order;
int son_of_root;

int AP_recur(node* a[], int i) {
	node* t;
	int min, m;
	check[i] = min = ++order;
	for (t = a[i]; t != NULL; t = t->next) {
		if (i == 0 && check[t->vertex] == 0) son_of_root++;
		if (check[t->vertex] == 0) {
			m = AP_recur(a, t->vertex);
			if (m < min) min = m;
			if (m >= check[i] && i != 0) printf("*%c %2d : %d\n", int2name(i), check[i], m);
			else printf(" %c %2d : %d\n", int2name(i), check[i], m);	
		}
		else {
			if (check[t->vertex] < min) min = check[t->vertex];
		}
	}
	return min;
}

void AP_search(node* a[], int V) {
	int i, n = 0;
	node* t;

	for (i = 0; i < V; i++) check[i] = 0;
	order = son_of_root = 0;
	AP_recur(a, 0);

	if (son_of_root > 1) printf("*");
	else printf(" ");
	printf("%c son : %d\n", int2name(0), son_of_root);
}

void main() {
	int V, E;						// node 와 edge를 입력하기 위한 변수
	input_adjlist(GL, &V, &E);		// adjlist 형식으로 그래프 생성
	printf("\n");					
	AP_search(GL, V);				// AP_search 함수를 호출하여 DFS 탐색을 하며 Articulation point 를 찾는다.
}