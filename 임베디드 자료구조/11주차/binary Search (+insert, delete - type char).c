#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void downheap(char* a, int N, int k) {	// 위에서 부터 아래로 정렬  (문자열데이터면 char* a, 정수데이터면 int* a)
	int i, v;
	v = a[k];				// v 에 root값을 저장 (보통 k=1)
	while (k <= N / 2) {		// k가 internal node 개수보다 커지면 반복문 종료
		i = k << 1;			// i = 2*k, 즉 i노드는 k노드의 왼쪽 자식
		if (i < N && a[i] < a[i + 1]) i++;			// 만약 오른쪽 자식이 더 크면 오른쪽 자식을 i노드로 설정
		if (v >= a[i]) break;						// v값, 즉 root값이 자식값보다 크면 종료
		a[k] = a[i];		// 부모값에 자식값 넣음, 즉 자식값을 위로 올림
		k = i;				// k노드(부모노드)가 i노드(자식노드)로 내려가며 반복
	}
	a[k] = v;				// 반복문이 끝난 위치에 k노드에 처음 받은 root값 저장
}

void heap_sort_bottom(char* a, int N) {		 // N은 데이터 길이, a는 힙 주소, bottom up 방식 (문자열데이터면 char* a, 정수데이터면 int* a)
	int k, t;
	for (k = N / 2; k > 0; k--) downheap(a, N, k);		// 모든 internal node 에 대하여 downheap 정렬

	while (N > 1) {				// 데이터 길이가 1이하가 되면 종료
		t = a[1];
		a[1] = a[N];
		a[N] = t;				// 가장 아래에 있는 노드와(작은값) 가장 위에 있는 노드(큰값)을 교체
		downheap(a, --N, 1);	//	교체가 끝난 큰값은 놔두고 다시 정렬 --> 오름차순 정렬이 된다
	}
}								

// 정렬

int bi_search(int key, char* a, int N) {				// binary search 함수 (검색) ( 문자열데이터면 char* a, 정수데이터면 int* a)
	int mid;
	int left = 0;
	int right = N - 1;
	while (right >= left) {
		mid = left + (float)(key - a[left]) * (right - left) / (a[right] - a[left]);		// 중간값 설정  mid = (right +left) / 2; 로 해도 됨
		if (key == a[mid])return mid;				// key가 중간값일 경우
		if (key < a[mid]) right = mid - 1;			// key가 중간값보다 작을 경우
		else left = mid + 1;						// key가 중간값보다 클 경우
	}
	return -1;					// 찾는 key 값이 없을 때
}

int bi_insert(int key, char* a, int* N) {			// 삽입				(문자열데이터면 char* a, 정수데이터면 int* a)
	int p = 0;
	int i;
	if (key > a[*N]) (*N)++;				// (교수님이 입력 안함) 만약 데이터에 있는 모든 값보다 큰 값이 삽입되면 데이터 길이를 증가시킨다. (heap이라서 *N이고 다른 정렬이면 *N-1임
	while (key > a[p] && p < *N) p++;				// key값이 a[p]값보다 작을 때 까지 p값 증가 (a[p]값은 key값보다 바로 큰값)
	for (i = *N; i > p; i--) a[i] = a[i - 1];		// 한칸 씩 밀려쓰기 (p+1자리까지)
	a[p] = key;										// p자리에 key값 넣기
	(*N)++;						// 데이터 길이 증가

	return p;				// key값을 넣은 위치 반환
}

int bi_delete(int key, char* a, int* N) {			// 제거       ( 문자열데이터면 char* a, 정수데이터면 int* a)
	int p;
	int i;
	if (*N > 0) {
		if ((p = bi_search(key, a, *N)) < 0)				// 찾는 값이 없으면 -1반환, p=-1
			return -1;
		for (i = p + 1; i < *N; i++) a[i - 1] = a[i];		// 한칸 씩 땡겨쓰기 (p부터 끝까지)
		a[*N-1] = '\0';						// (교수님이 넣은 값 아님) string 하려고 집어넣은거임 int면 뺄 것
		(*N)--;					// 데이터 길이 감소
		return p;			// 제거한 key값 위치반환
	}
	return -1;		
}



void main() {
	int i;
	char str[20] = "TOSORTALGORITHM";
	char cpy[20] = { 0 };					// cpy 문자열
	cpy[0] = ' ';							// 처음값은 공백
	int size = 15;							// 배열 길이
	
	for (i = 0; i < 15; i++) cpy[i + 1] = str[i];		// str값 cpy에 복사

	printf("원형\n");
	for (i = 1; i <= 15; i++) printf("%3c", cpy[i]);	// 원형
	
	heap_sort_bottom(cpy, size);						// 정렬	

	printf("\n\n정렬\n");
	for (i = 1; i <= 15; i++) {
		printf("%3c", cpy[i]);							// 정렬값 출력	
	}
	int p = bi_search('S', cpy, size);
	printf("\n\nFind your key at %d (value %c)\n", p, cpy[p]);		// S 탐색

	p = bi_insert('Z', cpy, &size);					// Z 삽입

	printf("\n\nInsert Z and string\n");
	for (i = 1; i <= 16; i++) {
		printf("%3c", cpy[i]);						// 삽입후 출력
	}

	printf("\n%s", cpy);							// 문자열로 출력

	p = bi_delete('A', cpy, &size);				// A빼기
	printf("\n%s\n", cpy);							// 제거후 문자열로 출력
}