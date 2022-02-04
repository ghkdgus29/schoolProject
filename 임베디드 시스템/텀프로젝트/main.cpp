#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios init_setting, new_setting;
char seg_num[10] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90 };
char seg_dnum[10] = { 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x58, 0x00, 0x10 };

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08

using namespace cv;
using namespace std;

typedef struct _node {							// 신상정보 (key 1  경찰, key 0 민원인)
	int plate;
	int key;
	char name[20];
	char department[20];
	struct _node* next;
}node;

void init_head(node** p);
void put_police(node* base);
void put_police_file(node* base);
int delete_all(node* base);
int delete_police(node* base, const char* key);
int check_police(node* base, int number);
int check_visitor(node* base);
int pass(node* base, int number, node* blacklist);
int print_list(node* base);
int get_number();
int select_menu();
void blocker_up(int bar);
void blocker_down(int bar);
void buzzer(int buz);
void menu(node* police, node* blacklist, int buz, int bar);
void seg(int seg, int num);
int check_blacklist(node* base, int number);

int main()
{
	int plate, i, j, num, key, a;
	char name[20], tmp, prev;
	node* police, *blacklist;
	init_head(&police);
	init_head(&blacklist);

	int bar = open("/dev/my_gpio", O_RDWR); // 차단바 + 버튼
	if (bar == -1) {
		printf("Opening was not possible!\n");
		return -1;
	}
	int buz = open("/dev/my_gpio2", O_WRONLY); // buzzer
	if (buz == -1) {
		printf("Opening was not possible!\n");
		return -1;
	}
	char buff = '1';							//buzzer off
	write(buz, &buff, 1);
	int seg1 = open("/dev/my_segment", O_RDWR); // segment
	if (seg1 == -1) {
		printf("Opening was not possible!\n");
		return -1;
	}
	VideoCapture cap(0);
	if (!cap.isOpened()) { printf("cannot open camera!!\n");}
	Mat frame;
	namedWindow("webcam video", WINDOW_AUTOSIZE);

	printf("\nWELCOME TO POLICE STATION ACCESS CONTROL\n\n    s : for test car\n    q : for quit\n    b : for buzzer\n    m : for menu\n\n");

	while (1) 
	{
		key = waitKey(1);
		cap >> frame;
		imshow("camera1", frame);
		read(bar, &buff, 1);				//for button
		prev = tmp;
		tmp = buff;
		if (prev != tmp) {
			if (tmp == '1') {
				write(bar, &tmp, 1);
			}
			else if (tmp == '2') {
				write(bar, &tmp, 1);
			}
		}
		if (key == 'q')
			break;
		else if (key == 's') {
			imwrite("test.JPG", frame);
			plate = get_number();
			if (plate < 0)
			{
				printf("\nCan't read number!");
				buzzer(buz);
			}
			else {
				seg(seg1, plate);
				a = pass(police, plate, blacklist);
				if (a < 0)
					buzzer(buz);
				else if (a == 0)
				{
					blocker_up(bar);
					sleep(4);
					blocker_down(bar);
				}
			}
			printf("\nWELCOME TO POLICE STATION ACCESS CONTROL\n\n    s : for test car\n    q : for quit\n    b : for buzzer\n    m : for menu\n\n");
		}
		else if (key == 'b')
		{
			buzzer(buz);
		}
		else if (key == 'm')
		{
			menu(police, blacklist, buz, bar);
			printf("\nWELCOME TO POLICE STATION ACCESS CONTROL\n\n    s : for test car\n    q : for quit\n    b : for buzzer\n    m : for menu\n\n");
		}
	}

	close(seg1);
	close(buz);
	close(bar);
	printf("\n Program ends...\n\n");
}

void init_head(node** p) {						// head 초기화
	*p = (node*)malloc(sizeof(node));
	(*p)->next = NULL;
}

void put_police(node* base) {					// 경찰관 이름 넣기
	node* k;
	k = (node*)malloc(sizeof(node));

	printf("insert names : ");
	scanf("%s", k->name);

	printf("insert departments : ");
	scanf("%s", k->department);

	printf("insert plate number : ");
	scanf("%d%*c", &(k->plate));

	k->key = 1;
	k->next = base->next;
	base->next = k;
}

void put_police_file(node* base) {
	FILE* fp;
	node* k;

	char temp[256];
	fp = fopen("police_list.txt", "rt");
	fgets(temp, 256, fp);
	fgets(temp, 256, fp);
	while (1) {
		k = (node*)malloc(sizeof(node));
		fscanf(fp, "%s %s %d\n", k->name, k->department, &(k->plate));
		k->key = 1;
		k->next = base->next;
		base->next = k;

		if (feof(fp))
			break;
	}
}

int delete_all(node* base) {
	node* t, * d;
	t = base->next;
	while (t != NULL) {
		d = t;
		t = t->next;
		free(d);
	}
	base->next = NULL;

	printf("\ndelete success!!\n");
	return 1;
}

int delete_police(node* base, const char* key) {
	node* p, *s;
	p = base;
	s = base->next;
	while (s != NULL && (strcmp(s->name, key) != 0)) {
		p = p->next;
		s = s->next;
	}
	if (s == NULL) {
		printf("\nthere is no police!!");
		return -1;
	}
	p->next = s->next;
	printf("\n%s deleted!\n", s->name);
	free(s);
	return 0;
}

int check_police(node* base, int number) {				// 번호판이 경찰인지 아닌지 체크 (1이면 경찰, 0이면 경찰아님)
	node* t;
	t = base->next;
	if (t == NULL) {
		printf("\nempty list!!");
		return 0;
	}

	while (t != NULL && t->plate != number) {
		t = t->next;
	}
	if (t == NULL) {
		printf("\nno police!\n");
		return 0;
	}

	printf("\nhe is police!\n");
	return 1;
}

int check_visitor(node* base) {												// 잘 입력시 1반환, 잘못 입력시 0반환
	node* t;
	char key[20];
	int cnt = 3;
	printf("Name the police officer : ");
	scanf("%s", key);
	t = base->next;
	if (t == NULL) {
		printf("empty list!\n");
		return -1;
	}

	while ((t != NULL) && (strcmp(t->name, key) != 0)) {
		t = t->next;
	}
	if (t == NULL) {
		printf("There is no name!\n");
		return 0;
	}

	while (cnt > 0) {
		printf("\nName the department : ");
		scanf("%s", key);
		if (!strcmp(t->department, key)) {
			break;
		}
		cnt--;
		printf("\nWrong department name!! you have %d chances left.", cnt);
	}

	return cnt;
}

int pass(node* base, int number, node* blacklist) {			// 통과시키기
	if (check_blacklist(blacklist, number))
	{
		printf("\nPERMISSION DENIED!");
		return -1;
	}
	if (check_police(base, number)) {
		printf("\nPASS!\n");
		return 0;
	}

	if (check_visitor(base)) {
		printf("\nPASS!\n");
		return 0;
	}

	printf("\n NO PASS");

	node* t;
	t = (node*)malloc(sizeof(node));
	t->plate = number;
	t->next = blacklist->next;
	blacklist->next = t;
	t->key = 0;
	return -1;
}

int print_list(node* base) {					// list 출력
	node* t;
	t = base->next;
	if (t == NULL) {
		printf("\nempty list !\n");
		return -1;
	}


	if (t->key == 1) {
		printf("\npolice name      Department      Plate number\n");
		printf("=============================================\n");
		while (t != NULL) {
			printf("%-14s     %-15s   %d\n", t->name, t->department, t->plate);
			t = t->next;
		}
	}

	else {
		printf("\nBlack list Plate number\n");
		printf("=======================\n");
		while (t != NULL) {
			printf("%d\n", t->plate);
			t = t->next;
		}
	}
	return 0;
}

int get_number() {
	string outText;
	tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();

	ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
	ocr->SetPageSegMode(tesseract::PSM_AUTO);

	Mat im = cv::imread("test.JPG", IMREAD_COLOR);

	ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);

	outText = string(ocr->GetUTF8Text());

	char number[255];
	char temp[4];
	int cnt = 0;
	int num = 0;

	for (int i = 0; i < outText.length(); i++) {
		if (outText[i] >= '0' && outText[i] <= '9') {
			number[cnt++] = outText[i];
		}
	}

	if (cnt < 4) {
		printf("error!\n");
		return -1;
	}

	printf("\n\n");

	for (int i = 0; i < 4; i++) {
		temp[3 - i] = number[--cnt];
	}

	num = (temp[0] - '0') * 1000 + (temp[1] - '0') * 100 + (temp[2] - '0') * 10 + temp[3] - '0';
	printf("plate number : %d\n", num);

	return num;

}

int select_menu() {
	int j; char s[10];
	printf("\nPOLICE STATION ACCESS CONTROL");
	printf("\n-----------------------------");
	printf("\n1. Input New Police ID");
	printf("\n2. Delete ID");
	printf("\n3. Buzzer on");
	printf("\n4. Show list");
	printf("\n5. Parking lot blocker up");
	printf("\n6. Parking lot blocker down");
	printf("\n7. Put List");
	printf("\n8. Quit menu");
	do {
		printf("\n: select operation ->");
		scanf("%d%*c", &j);
	} while (j <= 0 || j > 8); 
	return j;
}

void blocker_up(int bar)
{
	char buff = '1';
	printf("\n***** Blocker up *****");
	write(bar, &buff, 1);
}

void blocker_down(int bar)
{
	char buff = '2';
	printf("\n***** Blocker down *****");
	write(bar, &buff, 1);
}

void buzzer(int buz)
{
	char buff = '0';
	printf("\n***** Buzzer *****");
	write(buz, &buff, 1);
	sleep(2);
	buff = '1';
	write(buz, &buff, 1);
}

void menu(node* police, node* blacklist, int buz, int bar) 
{
	int i, j;
	char name[20];
	int num;
	while (((i = select_menu()) != 8))
	{
		switch (i) {
		case 1:
			put_police(police);
			break;
		case 2:
			printf("\n  1. Delete Police ID\n  2. Delete All Police\n  3. Delete All Blacklist: ");
			printf("\n----> ");
			scanf("%d%*c", &j);
			if (j == 1)
			{
				printf("Put Name :");
				scanf("%s", &name);
				delete_police(police, name);
			}
			else if (j == 2)
			{
				delete_all(police);
			}
			else if (j == 3)
			{
				delete_all(blacklist);
			}
			break;
		case 3:
			buzzer(buz);
			break;
		case 4:
			printf("\n  1. Show Police List\n  2. Show Blacklist");
			printf("\n----> ");
			scanf("%d%*c", &j);
			if (j == 1)
			{
				print_list(police);
			}
			else if (j == 2)
			{
				print_list(blacklist);
			}

			break;
		case 5:
			blocker_up(bar);
			break;
		case 6:
			blocker_down(bar);
			break;
		case 7:
			put_police_file(police);
			break;
		}
	}
}

void seg(int seg, int num)
{
	int timer = 0;
	short data[4];
	int tmp_n = 0;
	int delay_time = 1000;
	int idx1000;
	int idx100;
	int idx10;
	int idx1;

	idx1000 = num / 1000;
	idx100 = num % 1000 / 100;
	idx10 = num % 100 / 10;
	idx1 = num % 10;

	data[0] = (seg_num[idx1000] << 4) | D1;
	data[1] = (seg_num[idx100] << 4) | D2;
	data[2] = (seg_num[idx10] << 4) | D3;
	data[3] = (seg_num[idx1] << 4) | D4;

	while (1) {
		write(seg, &data[tmp_n], 2);
		usleep(delay_time);

		tmp_n++;
		timer++;
		if (tmp_n > 3) {
			tmp_n = 0;
		}

		if (timer > 3000)
			break;
	}
	write(seg, 0x0000, 2);
}

int check_blacklist(node* base, int number)
{
	node* t;
	t = base->next;
	if (t == NULL) {
		printf("\nempty list!!");
		return 0;
	}

	while (t != NULL && t->plate != number) {
		t = t->next;
	}
	if (t == NULL) {
		printf("\nno blacklist!\n");
		return 0;
	}

	printf("\nhe is blacklist!\n");
	return 1;
}