#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char name[10];
	int kor;
	int eng;
	int math;
	int sum;
	float avg;
	struct node* next;
}node;

node* head, * tail;
int count = 1;

void init_list() {
	head = (node*)calloc(1, sizeof(node));
	tail = (node*)calloc(1, sizeof(node));
	head->next = tail;
	tail->next = tail;
}



void ordered_insert(char m[], int k, int e, int ma) {			// �������� 1. �����Է�
	node* s, * p, * r;
	p = head;
	s = p->next;
	while (s->sum > (k+e+ma) && s != tail) {
		p = p->next;
		s = p->next;
	}
	r = (node*)calloc(1, sizeof(node));
	strcpy(r->name, m);
	r->kor = k;
	r->eng = e;
	r->math = ma;
	r->sum = k + e + ma;
	r->avg = (k + e + ma) / 3.0;

	
	p->next = r;
	r->next = s;
}


void print_list(node* t) {		// 2. ���� Ȯ�� , head �Է�
	
	printf("\n-----------------------------------------");
	printf("\n �̸�   ����   ����  ����  ����  ���  ���");
	printf("\n-----------------------------------------");
	t = t->next;
	if (t == tail) printf("��ϵ� ������ �����ϴ�!\n");
	while (t != tail) {
		printf("\n%s   %d   %d  %d  %d  %2.2f  %d��\n", t->name, t->kor, t->eng, t->math, t->sum, t->avg, count++);
		t = t->next;
	}
}

void main() {
	init_list();
	int cnt=1, i;
	while (1) {
		printf("[Menu]\n");
		printf("1. ���� �Է�\n");
		printf("2. ���� Ȯ��\n");
		printf("3. ����\n");
		printf("---------------\n���� (1~3) : ");
		scanf("%d", &i);
		if (i == 1) {
			int kor, eng, math;
			char name[10];
			printf("\n%d ��° �л� �̸�: ", cnt++);
			scanf("%s", name);
			printf("\n���� ���� : ");
			scanf("%d", &kor);
			printf("\n���� ���� : ");
			scanf("%d", &eng);
			printf("\n���� ���� : ");
			scanf("%d", &math);
			ordered_insert(name, kor, eng, math);
		}
		else if (i == 2) {
			printf("\n");
			print_list(head);
		}
		else break;
	}
}
