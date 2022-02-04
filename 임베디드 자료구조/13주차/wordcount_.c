#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	struct node* left;
	struct node* right;
}node;

typedef struct wcount {
	int count;
	char word[20];
}wcount;

node* head;
const char delim[] = " \t\r\n,.-";

int wcount_cmp(const void* a, const void* b) {
	return strcmp(((wcount*)a)->word, ((wcount*)b)->word);
}

void print_wcount(void* a) {
	printf("%s : %d\n", ((wcount*)a)->word, ((wcount*)a)->count);
}

typedef int(*FCMP)(const void*, const void*);

void init_tree(node** p) {
	*p = (node*)malloc(sizeof(node));
	(*p)->left = NULL;
	(*p)->right = NULL;
}

void* btv_search(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* t;
	t = base->left;
	while (t != NULL && fcmp(t + 1, key) != 0) {
		if (fcmp(key, t + 1) < 0) t = t->left;
		else t = t->right;
	}
	if (t == NULL) return NULL;
	else return t + 1;
}

void* btv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {
	node* p, * s;
	p = base;
	s = base->left;
	while (s != NULL) {
		p = s;
		if (fcmp(key, s + 1) < 0) s = s->left;
		else s = s->right;
	}
	s = (node*)malloc(sizeof(node) + width);
	memcpy(s + 1, key, width);
	s->left = NULL;
	s->right = NULL;

	if (fcmp(key, p + 1) < 0 || p == base) p->left = s;
	else p->right = s;

	(*num)++;
	return s;
}


int _index;

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

void main() {
	int i, num = 0, cnt = 0;

	char* tok;
	char str[256];
	wcount wc;
	wcount* wcp;

	init_tree(&head);

	FILE* fp;
	fp = fopen("wordcount.txt", "rt");

	fgets(str, 256, fp);
	do {
		tok = strtok(str, delim);
		while (tok) {
			strcpy(wc.word, tok);
			wcp = (wcount*)btv_search(&wc, head, &num, sizeof(wcount), wcount_cmp);
			if (wcp == NULL) {
				wc.count = 1;
				btv_insert(&wc, head, &num, sizeof(wcount), wcount_cmp);
			}
			else
				(wcp->count)++;
			tok = strtok(NULL, delim);
		}
		fgets(str, 256, fp);
	} while (!feof(fp));

	printf("\n\n");

	btv_list(head->left, print_wcount);			// tree 그래프의 데이터 (단어:카운트) 출력

	printf("\n\n\n");

	btv_balance(head, &num, sizeof(wcount));		// balanced tree 그래프 형성

	btv_list(head->left, print_wcount);		// balanced tree 그래프 출력

	fclose(fp);
}