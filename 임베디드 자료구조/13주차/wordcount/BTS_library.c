#include "BTS_library.h"

int _index;

void btv_list(node* p, void(*fptr)(void*)) {			// Ʈ�� ����Լ�
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

void btv_balance(node* base, int* num, int width) {		// Ʈ���� �뷱�� ��� �Լ�
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

void init_tree(node** p) {						// Ʈ�� �ʱ�ȭ
	*p = (node*)malloc(sizeof(node));
	(*p)->left = NULL;
	(*p)->right = NULL;
}

void* btv_search(void* key, node* base, int* num, int width, FCMP fcmp) {		// �Է¹��� key(data�ּ�)�� Ʈ���� �����ϴ� ����� data���� ���� ���� ã�� �Լ�
	node* t;
	t = base->left;
	while (t != NULL && fcmp(key, t + 1) != 0) {		// ���� ���Ϸ��� data�� t�ּ� ��, t+1�� ����, ���� ���� �ٸ���
		if (fcmp(key, t + 1) < 0) t = t->left;			// key->word������ t����� data�� ũ�� ���� �Ʒ� �̵�
		else t = t->right;								// key->word�� > t��� data�� �̸� ������ �Ʒ� �̵�
	}
	if (t == NULL) return NULL;		// �� ã���� NULL ��ȯ
	else return t + 1;				// ã���� data�� �����ϴ� �ּ� ��ȯ		
}

void* btv_insert(void* key, node* base, int* num, int width, FCMP fcmp) {		// key(data�ּ�)�� �����ϴ� Ʈ�� ��带 �����Ѵ�.
	node* p, * s;
	p = base;
	s = base->left;
	while (s != NULL) {								// p��带 ã�� ���� �ݺ���, s�� NULL�� ã�� �� ���� �ݺ�
		p = s;
		if (fcmp(key, s + 1) < 0) s = s->left;		// key->word < s��� �����Ͱ� , s ����� ���ʾƷ��� �̵�
		else s = s->right;							// key->word > s��� �����Ͱ�, s ����� �����ʾƷ��� �̵�
	}
	s = (node*)malloc(sizeof(node) + width);		// s��� �޸� ���� ����, �޸𸮰����� node�� wcount�� ���� ũ���̴�.
	memcpy(s + 1, key, width);						// s����� data�ּҸ� �����ϴ� ����, �� s+1�� key(data�ּ�)�� �����Ѵ�.
	s->left = NULL;
	s->right = NULL;				// s��� �� ���� NULL�� ����

	if (fcmp(key, p + 1) < 0 || p == base) p->left = s;		// ���� key->word���� �θ��� data������ �۰ų� �θ� base��� p����� �������� s��� ����
	else p->right = s;				// key->word���� �θ��� data������ ũ�ٸ� p����� ���������� s��� ����

	(*num)++;			// Ʈ�� ���� ����
	return s;			// s��� (������� ���+data�ּ�) �ּ� ��ȯ
}

void _sort(node* p, void* a, int width) {				// tree ���е��� �迭�� �̾Ƴ��� �Լ�, a�� �迭�ּ�, width�� �迭 ��ĭ ũ����
	if (p != NULL) {			// ��������: NULL�� �ٴٸ��� ����
		_sort(p->left, a, width);								// ���� ��� �湮 (���� ���)							
		memcpy((char*)a + (_index++) * width, p + 1, width);	// ���� ��尪 �迭�� ����, �������ּҴ� p+1�� �����Ѵ�.
		_sort(p->right, a, width);								// ������ ��� �湮 (ū ���)
	}
}

node* _balance(int N, void* a, int width) {				// �̾Ƴ� �迭�� ���� �������� binary tree ����
	int nl, nr;
	node* p;
	if (N > 0) {		//  ��������: ������ ���̰� 0�̵Ǹ� ����
		nl = (N - 1) / 2;						// nl�� �迭 �߰���(root)�� ������ ���� �迭 ����
		nr = N - nl - 1;						// nr�� �迭 �߰���(root)�� ������ ������ �迭 ����
		p = (node*)malloc(sizeof(node) + width);		// �޸𸮰����� node�� wcount�� ���� ũ��
		p->left = _balance(nl, a, width);		// ���ʳ�� ���� (���� ���)
		memcpy(p + 1, (char*)a + (_index++) * width, width);		// ���� �湮 ��忡 �迭���� ����ִ´�., �������ּҴ� p+1�� ����
		p->right = _balance(nr, a, width);		// ������ ��� ���� (ū ���)

		return p;
	}
	else return NULL;
}


