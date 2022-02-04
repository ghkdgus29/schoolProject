#include "BTS_library.h"

int _index;

void btv_list(node* p, void(*fptr)(void*)) {			// 트리 출력함수
	int i;
	static int x = 0;

	if (p != NULL) {
		x += 2;

		btv_list(p->right, fptr);
		for (i = 2; i < x; i++) printf(" ");
		fptr(p + 1);
		btv_list(p->left, fptr);
		x -= 2;
	}
}

void btv_balance(node* base, int* num, int width) {		// 트리의 밸런스 잡는 함수
	void* tmp;
	int ntmp = *num;
	tmp = malloc(width * ntmp);
	_index = 0;
	_sort(base->left, tmp, width);
	ntmp = *num;
	btv_deleteall(base, num);
	_index = 0;
	base->left = _balance(ntmp, tmp, width);
	*num = ntmp;
	free(tmp);
}

void _deleteall(node* t) {
	if (t != NULL) {
		_deleteall(t->left); 
		_deleteall(t->right); 
		free(t);
	}
}

void* btv_deleteall(node* base, int* num) {
	node* t;
	t = base->left;
	_deleteall(t);
	base->left = NULL;
	base->right = NULL;
	*num = 0;
	return base;
}

void init_tree(node** p) {						// 트리 초기화
	*p = (node*)malloc(sizeof(node));
	(*p)->left = NULL;
	(*p)->right = NULL;
}

void* btv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// 입력받은 key(data주소)와 트리내 존재하는 노드의 data값이 같은 것을 찾는 함수
	node* t;
	t = base->left;
	while (t != NULL && fcmp(key, t + 1) != 0) {		// 실제 비교하려는 data는 t주소 밑, t+1에 있음, 만약 값이 다르면
		if (fcmp(key, t + 1) < 0) t = t->left;			// key->word값보다 t노드의 data가 크면 왼쪽 아래 이동
		else t = t->right;								// key->word값 > t노드 data값 이면 오른쪽 아래 이동
	}
	if (t == NULL) return NULL;		// 못 찾으면 NULL 반환
	else return t + 1;				// 찾으면 data가 존재하는 주소 반환		
}

void* btv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {		// key(data주소)를 저장하는 트리 노드를 삽입한다.
	node* p, * s;
	p = base;
	s = base->left;
	while (s != NULL) {								// p노드를 찾기 위한 반복문, s가 NULL을 찾을 때 까지 반복
		p = s;
		if (fcmp(key, s + 1) < 0) s = s->left;		// key->word < s노드 데이터값 , s 노드의 왼쪽아래로 이동
		else s = s->right;							// key->word > s노드 데이터값, s 노드의 오른쪽아래로 이동
	}
	s = (node*)malloc(sizeof(node) + width);		// s노드 메모리 공간 생성, 메모리공간은 node와 wcount를 합한 크기이다.
	memcpy(s + 1, key, width);						// s노드의 data주소를 저장하는 공간, 즉 s+1에 key(data주소)를 저장한다.
	s->left = NULL;
	s->right = NULL;				// s노드 양 끝단 NULL로 접지

	if (fcmp(key, p + 1) < 0 || p == base) p->left = s;		// 만약 key->word값이 부모노드 data값보다 작거나 부모가 base라면 p노드의 왼쪽으로 s노드 연결
	else p->right = s;				// key->word값이 부모노드 data값보다 크다면 p노드의 오른쪽으로 s노드 연결

	(*num)++;			// 트리 길이 증가
	return s;			// s노드 (집어넣은 노드+data주소) 주소 반환
}

void _sort(node* p, void* a, int width) {				// tree 성분들을 배열로 뽑아내는 함수, a는 배열주소, width는 배열 한칸 크기임
	if (p != NULL) {			// 종료조건: NULL에 다다르면 종료
		_sort(p->left, a, width);								// 왼쪽 노드 방문 (작은 노드)							
		memcpy((char*)a + (_index++) * width, p + 1, width);	// 현재 노드값 배열로 추출, 데이터주소는 p+1에 존재한다.
		_sort(p->right, a, width);								// 오른쪽 노드 방문 (큰 노드)
	}
}

node* _balance(int N, void* a, int width) {				// 뽑아낸 배열을 통해 균현잡힌 binary tree 형성
	int nl, nr;
	node* p;
	if (N > 0) {		//  종료조건: 데이터 길이가 0이되면 종료
		nl = (N - 1) / 2;						// nl은 배열 중간값(root)를 제외한 왼쪽 배열 길이
		nr = N - nl - 1;						// nr은 배열 중간값(root)를 제외한 오른쪽 배열 길이
		p = (node*)malloc(sizeof(node) + width);		// 메모리공간은 node와 wcount를 합한 크기
		p->left = _balance(nl, a, width);		// 왼쪽노드 형성 (작은 노드)
		memcpy(p + 1, (char*)a + (_index++) * width, width);		// 현재 방문 노드에 배열값을 집어넣는다., 데이터주소는 p+1에 존재
		p->right = _balance(nr, a, width);		// 오른쪽 노드 형성 (큰 노드)

		return p;
	}
	else return NULL;
}


