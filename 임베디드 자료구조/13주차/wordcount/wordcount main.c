#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BTS_library.h"

typedef struct wcount {
	int count;				// 단어가 몇번 나왔는지 count 저장
	char word[20];			// 단어 저장
}wcount;

node* head;					// tree base
const char delim[] = " \t\r\n,.-";			// 다음의 아스키값이 나오면 NULL을 입력하고 단어화

int wcount_cmp(const void* a, const void* b) {
	return strcmp(((wcount*)a)->word, ((wcount*)b)->word);		// wcount 주소를 입력받으면 주소에 해당하는 word에 접근하여 단어 크기 비교		
}

void print_wcount(void* a) {
	printf("%s : %d\n", ((wcount*)a)->word, ((wcount*)a)->count);	// 해당 트리에 존재하는 데이터 주소의 count와 word 출력
}

void main() {
	int i, num = 0, cnt = 0;

	char* tok;			// 추출한 단어의 주소를 저장할 포인터
	char str[256];
	wcount wc;
	wcount* wcp;

	init_tree(&head);

	FILE* fp;
	fp = fopen("wordcount.txt", "r");

	fgets(str, 256, fp);		// 파일에서 한 줄 읽어와서 str배열에 (최대 256자) 저장하기
	do {					// 256자 내의 file이 끝날 경우를 대비해 do while로 만듬
		tok = strtok(str, delim);	// str로부터 추출한 단어의 주소를 tok에 저장
		while (tok) {			// tok에 더 이상 단어가 저장되지 않을 때 까지 반복 
			strcpy(wc.word, tok);			// tok에 저장된 단어를 wc.word에 저장
			wcp = (wcount*)btv_search(&wc, head, &num, sizeof(wcount), wcount_cmp);		// wc주소를 key로 트리를 탐색, wcp는 탐색결과의 주소를 반환 받는다.
			if (wcp == NULL) {		// 찾는 값 없음, 즉 처음 찾은 단어임
				wc.count = 1;
				btv_insert(&wc, head, &num, sizeof(wcount), wcount_cmp);			// 처음 찾은 단어의 주소를 트리에 삽입
			}
			else
				(wcp->count)++;		// 찾는 값 있음, 따라서 단어 count를 증가
			tok = strtok(NULL, delim);	// 다음 단어부터 추출
		}
		fgets(str, 256, fp);	// 파일의 다음 문장을 읽어와 str배열에 저장

	} while (!feof(fp));	// 만약 file의 끝이면 반복문 종료

	printf("\n\n");

	btv_list(head->left, print_wcount);			// tree 그래프의 데이터 (단어:카운트) 출력

	printf("\n\n\n");

	btv_balance(head, &num, sizeof(wcount));		// balanced tree 그래프 형성

	btv_list(head->left, print_wcount);		// balanced tree 그래프 출력

	fclose(fp);
}