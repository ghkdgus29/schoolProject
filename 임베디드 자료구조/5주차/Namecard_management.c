#define NAME_SIZE 21
#define CORP_SIZE 31
#define TEL_SIZE 16
#define REC_SIZE (NAME_SIZE+CORP_SIZE+TEL_SIZE)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct card {
	char name[NAME_SIZE];
	char corp[CORP_SIZE];
	char tel[TEL_SIZE];
	struct card* next;
}card;

card* head, * tail;

void init_card() {
	head = (card*)calloc(1, sizeof(card));
	tail = (card*)calloc(1, sizeof(card));
	head->next = tail;
	tail->next = tail;
}

void input_card() {
	card* t;
	t = (card*)calloc(1, sizeof(card));

	printf("\nInput namecard menu");
	printf("\n\nInput name :");
	gets(t->name);
	printf("\nInput corporation :");
	gets(t->corp);
	printf("\nInput telephone number:");
	gets(t->tel);

	t->next = head->next;
	head->next = t;
}

int delete_card(char* s) {			// 성공적으로 지우면 1, 아니면 0 반환
	card* t, * p;
	p = head;
	t = p->next;
	while (strcmp(s, t->name) != 0 && t != tail) {
		p = p->next;
		t = p->next;
	}
	if (t == tail) return 0;
	p->next = t->next;
	free(t);
	return 1;
}

card* search_card(char* s) {
	card* t;
	t = head->next;
	while (strcmp(s, t->name) != 0 && t != tail) {
		t = t->next;
	}
	if (t == tail) return NULL;
	else return t;
}

void save_card(char* s) {
	FILE* fp;
	card* t;
	fp = fopen(s, "wb");
	t = head->next;
	while (t != tail) {
		fwrite(t, REC_SIZE, 1, fp);
		t = t->next;
	}
	fclose(fp);
}

void load_card(char* s) {
	FILE* fp = NULL;
	card* t, * u;
	fp = fopen(s, "rb");

	t = head->next;
	while (t != tail) {
		u = t;
		t = t->next;
		free(u);
	}
	head->next = tail;
	while (1) {
		t = (card*)calloc(1, sizeof(card));
		if (!fread(t, REC_SIZE, 1, fp)) {			//만약 fread가 실패하여 NULL값을 리턴할경우 (오류가 발생할 경우)
			free(t);
			break;
		}
		t->next = head->next;
		head->next = t;
	}
	fclose(fp);
}

void print_header(FILE* f) {
	fprintf(f, "\nName                  "
		"Corprotation                    "
		"Telephone number");
	fprintf(f, "\n----------------------"
		"--------------------------------"
		"-----------------");
}

void print_card(card* t, FILE* f) {
	fprintf(f, "\n%-20s   %-30s   %-15s", t->name, t->corp, t->tel);
}

int select_menu() {
	int j;
	char s[10];
	printf("\nName Card Manager");
	printf("\n------------------");
	printf("\n1. Input name card");
	printf("\n2. Delete name card");
	printf("\n3. Search name card");
	printf("\n4. Load name card");
	printf("\n5. Save name card");
	printf("\n6. Show list");
	printf("\n7. Program end....");
	do {
		printf("\n: selection operation ->");
		gets(s);
		j = atoi(s);
	} while (j < 0 || j>7);

	return j;
}

void main() {
	char* fname = "NameCard.dat";
	char name[NAME_SIZE];
	int i;
	card* t;

	init_card();

	while ((i = select_menu()) != 7) {
		switch (i) {
		case 1:
			input_card();
			break;
		case 2:
			printf("\nInput name to delete -> ");
			gets(name);
			if (!delete_card(name)) printf("\nCan't find the name...");
			break;
		case 3:
			printf("\nInput name to search -> ");
			gets(name);
			t = search_card(name);
			if (t == NULL) break;
			print_header(stdout);
			print_card(t, stdout);
			break;
		case 4:
			load_card(fname);
			break;
		case 5:
			save_card(fname);
			break;
		case 6:
			t = head->next;
			print_header(stdout);
			while (t != tail) {
				print_card(t, stdout);
				t = t->next;
			}
			printf("\n");
			break;
		}
	}
}