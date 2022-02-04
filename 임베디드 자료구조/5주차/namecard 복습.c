#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SIZE 20
#define COL_SIZE 30
#define TEL_SIZE 16
#define REC_SIZE (NAME_SIZE + COL_SIZE + TEL_SIZE)

typedef struct _card {
	char name[NAME_SIZE];
	char col[COL_SIZE];
	char tel[TEL_SIZE];
	struct _card* next;
}card;

card* head, * tail;

void init_card() {
	head = (card*)malloc(sizeof(card));
	tail = (card*)malloc(sizeof(card));
	head->next = tail;
	tail->next = tail;
}

void input_card() {
	card* s;

	s = (card*)malloc(sizeof(card));
	printf("\nInput name : ");
	gets(s->name);
	printf("\nInput college : ");
	gets(s->col);
	printf("\nInput telephone number : ");
	gets(s->tel);

	s->next = head->next;
	head->next = s;
}

int delete_card(char* key) {
	card* p, * s;
	p = head;
	s = p->next;

	while (strcmp(s->name, key) != 0 && s != tail) {
		p = p->next;
		s = p->next;
	}
	if (s == tail)
		return 0;

	p->next = s->next;
	free(s);
	return 1;
}

card* search_card(char* key) {
	card* s;
	s = head->next;

	while (strcmp(s->name, key) != 0 && s != tail) {
		s = s->next;
	}

	if (s == tail) {
		printf("\nwrong name!!");
		return NULL;
	}

	printf("\nsuccessfully find name!!\n");
	return s;
}

void save_card(char* file) {
	card* t;
	FILE* fp;
	fp = fopen(file, "wb");
	if (fp == NULL) {
		printf("\nfail to open file!!");
		return -1;
	}

	t = head->next;
	if (t == tail) {
		printf("\nthere is no name card!");
		return -1;
	}

	while (t != tail) {
		fwrite(t, REC_SIZE, 1, fp);
		t = t->next;
	}
	printf("\nsuccessfully save!!");
	fclose(fp);
}

void load_card(char* file) {
	card* d, * s;
	FILE* fp;
	fp = fopen(file, "rb");
	if (fp == NULL) {
		printf("\nfail to load file!!");
		return -1;
	}

	s = head->next;
	while (s != tail) {
		d = s;
		s = s->next;
		free(d);
	}
	head->next = tail;

	while (1) {
		s = (card*)malloc(sizeof(card));
		if (!fread(s, REC_SIZE, 1, fp)) {
			free(s);
			break;
		}
		s->next = head->next;
		head->next = s;
	}

	printf("\nload success!!");
	fclose(fp);
}


void print_header(FILE* f) {
	fprintf(f, "\nName                College                       Tel             ");
	fprintf(f, "\n-------------------------------------------------------------------");
}

void print_card(card* t, FILE* f) {
	fprintf(f, "\n%-20s %-30s %-15s", t->name, t->col, t->tel);
}

int select_menu() {
	char s[10];
	int j;
	printf("\nName Card Manager");
	printf("\n--------------------");
	printf("\n1. Input name card");
	printf("\n2. Delete name card");
	printf("\n3. Search name card");
	printf("\n4. Load name card");
	printf("\n5. Save name card");
	printf("\n6. Show list");
	printf("\n7. Program end...");
	do {
		printf("\n select operation --> ");
		scanf("%s%*c", s);
		j = atoi(s);
	} while (j < 1 || j > 7);

	return j;
}


int main() {
	int choice;
	char temp[NAME_SIZE];
	card* t;
	char* fname = "Namecardtest.dat";
	init_card();

	while ((choice = select_menu()) != 7) {
		switch (choice) {
		case 1:
			input_card();
			break;
		case 2:
			printf("\nInput name what you try to delete : ");
			gets(temp);
			if (!delete_card(temp))
				printf("\nwrong name!");
			else
				printf("\ndelete succesful!");
			printf("\n");
			break;
		case 3:
			printf("\ninput name what you try to find : ");
			gets(temp);
			t = search_card(temp);
			if (t != NULL) {
				print_header(stdout);
				print_card(t, stdout);
			}
			printf("\n");
			break;
		case 4:
			load_card(fname);
			printf("\n");
			break;
		case 5:
			save_card(fname);
			printf("\n");
			break;
		case 6:
			print_header(stdout);
			t = head->next;
			while (t != tail) {
				print_card(t, stdout);
				t = t->next;
			}
			printf("\n");
			break;
		}

	}


}