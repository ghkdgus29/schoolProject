#include <stdio.h>
#include <stdlib.h>
#define SIZE 100
#define BASE(i) ((char*)base + (i)*width)			// (char*)base + y*width 를 매크로함수를 통해 쉽게 호출

int intcmp(const void* a, const void* b) {			// a , b 크기 비교 , 자료형에 따라 형변환 해주어야됨
	return (*(int*)a - *(int*)b);
}

typedef int (*FCMP)(const void*, const void*);		// function pointer를 쉽게 호출하기 위한 typedef

int lv_search(void* key, void* base, int* num, int width, FCMP fcmp) {	//  찾기함수
	int i = 0;
	while (fcmp(BASE(i), key) != 0 && i < *num)i++;	// while (a[i] != key && i < *num)i++;

	return(i < *num ? i : -1);						// key값 못찾으면 -1, 찾으면 key값 인덱스 반환
}

int lv_insert(void* key, void* base, int* num, int width, FCMP fcmp) {		//  삽입함수
	memcpy(BASE((*num)++), key, width);			// a[(*num)++] = key; 맨뒤에 key 삽입

	return *num - 1;				// 맨 뒤값 , 인덱스 반환
}

int lv_delete(void* key, void* base, int* num, int width, FCMP fcmp) {		//  제거함수
	int p, i;
	if (*num > 0) {
		if ((p = lv_search(key, base, num, width, fcmp)) < 0) return -1;	//if ((p = seq_search(key, a, num)) < 0) return -1;	// 만약 p가 -1, 즉 찾는 key값이 없으면 -1반환

		for (i = p + 1; i < *num; i++) {
			memcpy(BASE(i - 1), BASE(i), width);		// for (i = p + 1; i < *num; i++) a[i - 1] = a[i]; p 위치부터 끝까지 한칸씩 데이터 땡겨쓰기
		}
		(*num)--;
		return p;
	}
	return -1;
}

int lfv_search(void* key, void* base, int* num, int width, FCMP fcmp) {	// 찾은후 앞으로 정렬 함수
	int i = 0, j;
	void* v;
	while (fcmp(BASE(i), key) != 0 && i < *num) i++;		// search
	if (i >= *num) return -1;								// 찾는 값 없으면 -1 반환

	v = malloc(width);						// temp (일종의 임시 변수)
	memcpy(v, BASE(i), width);				// v 에 찾은 값 저장 (인덱스 i)
	for (j = i - 1; j >= 0; j--) memcpy(BASE(j + 1), BASE(j), width);	// for (j = i - 1; j >= 0; j--) a[j + 1] = a[j]; 1부터 i위치까지 한칸 씩 데이터 밀기

	memcpy(BASE(0), v, width);			// a[0] = key;
	return 0;
}

void main() {
	int data[SIZE] = { 1, 4, 7, 34, 23, 6, 356, 674 , 123, -54 };
	int nitem = 10;
	int index, i;

	int search = 7;
	int insert = 454;
	int delete = 356;
	int fsearch = -54;



	index = lv_search(&search, data, &nitem, sizeof(int), intcmp);						// search 찾기

	printf("7은 %d번째에 있습니다.\n", index + 1);				// 1더해줘야됨, 배열이니깐

	if (nitem < SIZE) lv_insert(&insert, data, &nitem, sizeof(int), intcmp);			// 454 삽입
	else printf("Memory is full !!\n");							// 만약 데이터 공간이 꽉찼으면 삽입실패

	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);

	printf("\n");
	lv_delete(&delete, data, &nitem, sizeof(int), intcmp);								// 356 제거


	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);

	printf("\n");
	lfv_search(&fsearch, data, &nitem, sizeof(int), intcmp);							//-54를 찾고 그 값을 맨앞으로 정렬

	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);






}