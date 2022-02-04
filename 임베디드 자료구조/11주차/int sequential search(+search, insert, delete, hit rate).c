#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

int seq_search(int key, int* a, int* num) {		// key는 찾는 값, a는 데이터, num은 데이터 길이
	int i = 0;
	while (a[i] != key && i < *num)i++;			// key값을 찾을 때 까지 인덱스 증가

	return (i < *num ? i : -1);					// 만약 key값을 못찾으면 i = *num 이니까 -1반환
}												//  key값을 찾으면 해당 key값 인덱스 반환

int seq_insert(int key, int* a, int* num) {		// key값을 데이터(a) 맨뒤에 삽입
	a[(*num)++] = key;			// 맨 뒤에 삽입

	return *num - 1;			// 맨 뒤값, key값 인덱스 반환
}

int seq_delete(int key, int* a, int* num) {		// key값을 a데이터에서 찾아 제거
	int p, i;
	if (*num > 0) {		// 데이터 길이가 0보다 커야됨
		if ((p = seq_search(key, a, num)) < 0) return -1;		// 만약 p가 -1, 즉 찾는 key값이 없으면 -1반환

		for (i = p + 1; i < *num; i++) a[i - 1] = a[i];			// key값을 찾으면 p(key 위치)위치 부터 끝까지 한칸씩 데이터 땡겨쓰기
		(*num)--;			// 데이터 길이 한칸 줄이기
		return p;			// 삭제한 데이터 위치 반환
	}
	return -1;			// 데이터 길이 0보다 작으면 -1반환
}

int seq_f_search(int key, int* a, int* num) {		// key값을 a데이터에서 찾고, 찾은 key값은 맨 앞으로 정렬
	int p, i;
	if ((p = seq_search(key, a, num)) < 0) return -1;			// 만약 p가 -1,즉 찾는 key값이 없으면 -1반환

	for (i = p - 1; i >= 0; i--) a[i + 1] = a[i];				//key값을 찾으면 1부터 p위치까지 한칸 씩 데이터 밀기(key 위치)
	a[0] = key;			// key값 맨 앞으로
	return 0;
}


void main() {
	int data[SIZE] = { 1, 4, 7, 34, 23, 6, 356, 674 , 123, -54 };
	int nitem = 10;
	int index, i;


	index = seq_search(34, data, &nitem);						// search 찾기

	printf("34는 %d번째에 있습니다.\n", index+1);				// 1더해줘야됨, 배열이니깐

	if (nitem < SIZE) seq_insert(453, data, &nitem);			// 453 삽입
	else printf("Memory is full !!\n");							// 만약 데이터 공간이 꽉찼으면 삽입실패

	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);

	printf("\n");
	seq_delete(23, data, &nitem);								// 23 제거


	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);

	printf("\n");
	seq_f_search(-54, data, &nitem);							//-54를 찾고 그 값을 맨앞으로 정렬

	for (i = 0; i < nitem; i++)									// 메모리 값들 출력
		printf("%4d", data[i]);
	


	

	
}