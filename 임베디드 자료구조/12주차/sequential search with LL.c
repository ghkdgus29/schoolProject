#include <stdio.h>
#include <stdlib.h>
#define LEN 5
#define BASE(i) ((char*)base + (i)*width)

int intcmp(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

typedef int (*FCMP)(const void*, const void*);

typedef struct node {
	struct node* next;
}node;

void init_list(node** p) {
	*p = (node*)malloc(sizeof(node));
	(*p)->next = NULL;		// *(*p).next= NULL; 와 같은 표현
}

node* llv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// 찾기 함수 (주소 반환)
	node* t;
	t = base->next;
	while (fcmp(t + 1, key) != 0 && t != NULL) t = t->next;		// t+1에 데이터 들어있음, 만약 찾는 값없으면 t주소 계속 이동
	return (t == NULL ? NULL : t + 1);		// 찾는 값 없으면 NULL 반환, 만약 찾으면 t+1 (데이터 담긴 방) 주소 반환
}

node* llv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {	// 삽입함수
	node* t;
	t = (node*)malloc(sizeof(node) + width);			// data를 넣을만한 방을 하나 더 만들어준다
	memcpy(t + 1, key, width);							// 만들어준 방에 data를 집어넣는다.
	t->next = base->next;								
	base->next = t;							// 주소를 링크드 리스트를 이용해 연결
	(*num)++;								// 데이터 길이 증가
	return t;								// 데이터를 갖고 있는 주소 (데이터는 밑의 방에 있음) 반환
}

node* llv_delete(void* key, node* base, int* num, int width, FCMP fcmp) {		// key 값을 찾아 해당 노드를 제거하는 함수
	node* p, * s;
	p = base;
	s = p->next;

	while (fcmp(s + 1, key) != 0 && s != NULL) {			// 해당 노드 찾기
		p = p->next;					// p는 타겟노드 이전 노드 (연결을 위함)
		s = s->next;					// s가 타겟 노드
	}
	if (s == NULL) return NULL;			// 못찾으면 NULL 반환

	p->next = s->next;					// s 지우려고 연결
	free(s);
	(*num)--;							// 길이 감소

	return p;
}

node* llfv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// key 값을 찾고 해당 노드를 맨 앞으로 이동시키는 함수
	node* p, * s;
	p = base;
	s = p->next;
	while (fcmp(s + 1, key) != 0 && s != NULL) {			// 해당 노드 찾기
		p = p->next;				// p는 타겟노드 이전 노드 (연결을 위함)
		s = s->next;				// s가 타겟 노드
	}
	if (s == NULL) return NULL;			// 못찾으면 NULL 반환

	p->next = s->next;						// s건너 뛰고 p 연결
	s->next = base->next;					// s를 베이스 뒤에 값과 연결
	base->next = s;							// 베이스를 s 와 연결

	return s;
}

int count(void* key, node* base, int* num, int width, FCMP fcmp) {		// 몇 번 째인지 찾기 함수
	node* t;
	int cnt = 1;			// 순서를 셀 변수, 가장 늦게 들어온 값이 LL의 가장 앞에 저장되어 있다.
	t = base->next;
	while (fcmp(t + 1, key) != 0 && t != NULL) { // t+1에 데이터 들어있음
		t = t->next;								// 만약 찾는 값없으면 t주소 계속 이동
		cnt++;										// cnt를 증가시켜 순서 증가
	}	
	return (t == NULL ? -1 : cnt);		// 찾는 값 없으면 NULL 반환, 만약 찾으면 t+1 (데이터 담긴 방) 몇번째인지 반환
}

void main() {
	void* p;
	node* t, * s;
	int i;
	int size = 0, key = 9;
	int data[LEN] = { 3, 1, 9, 7, 5 };

	init_list(&t);

	for (i = 0; i < 5; i++) {
		p = llv_insert(data + i, t, &size, sizeof(int), intcmp);				// 데이터 삽입 (LL)

		printf("%2d", *((node*)p + 1));									// 데이터 값 출력
	}

	p = llv_search(&key, t, &size, sizeof(int), intcmp);			// p는 찾는 key값의 주소 (void*)

	i = count(&key, t, &size, sizeof(int), intcmp);					// i는 찾는 key값이 몇번째인지 나타내는 함수

	printf("\n%d위치는 starting addr로 부터 %d번째 입니다.\n", *(int*)p, i);

	llfv_search(&key, t, &size, sizeof(int), intcmp);				// 찾고 앞으로 정렬 함수

	for (s = t->next; s != NULL; s = s->next) {
		printf("%2d", *(s + 1));
	}

	llv_delete(&key, t, &size, sizeof(int), intcmp);				// 제거 함수
	printf("\n");

	for (s = t->next; s != NULL; s = s->next) {
		printf("%2d", *(s + 1));
	}


}