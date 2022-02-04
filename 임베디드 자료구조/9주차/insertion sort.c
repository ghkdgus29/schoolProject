#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

void insert_sort(int* a, int N) {			// insert sort 함수 , a는 데이터주소, N은 데이터 길이
	int i, j, t;
	for (i = 1; i < N; i++) {				// 두번째 데이터부터 맨 끝까지에 대해 반복
		t = a[i];					// t에 a[i] 값 저장
		j = i;
		while (j > 0 && a[j - 1] > t) {		// 만약 현재 j인덱스가 (맨 처음엔 i인덱스) 0보다 크고,  a[i]보다 크면
			a[j] = a[j - 1];		// 반복해서 값을 바꾼다 (앞으로 민다)
			j--;					// j인덱스는 앞으로 이동
		}
		a[j] = t;					// 반복문을 빠져나왔을 때 j인덱스가 a[i]가 있을 자리이다.
	}
}

void indirect_insert_sort(int* a, int* index, int N) {	// 간접 insert sort 함수, a는 데이터 주소, index는 인덱스 주소, N은 데이터 길이
	int i, j, t;
	for (i = 0; i < N; i++) index[i] = i;		// 인덱스 초기화 0 ~ N-1

	for (i = 1; i < N; i++) {
		t = index[i];				// t에 index[i] 저장, 즉 a[i]의 인덱스만 저장
		j = i;
		while (j > 0 && a[index[j - 1]] > a[t]) {		// 간접참조를 통해 이전값이 현재값보다 크다면 
			index[j] = index[j - 1];				// 인덱스를 바꾼다 (앞으로 민다)
			j--;									// 인덱스는 앞으로 이동
		}
		index[j] = t;		// 반복문을 빠져나왔을 때 a[i]의 인덱스는 index[j]가 있을 자리이다.
	}
}

void main() {
	int i;
	int number[SIZE] = { 9, 3, 1, 7 , 9 , 4, 5, 6, 2, 11 };

	insert_sort(number, SIZE);
	for (i = 0; i < 10; i++) {
		printf("%3d", number[i]);
	}

	int number2[SIZE] = { 9, 3, 1, 7 , 9 , 4, 5, 6, 2, 11 };
	int index[SIZE];

	printf("\n간접참조 sort\n");
	indirect_insert_sort(number2, index, SIZE);
	for (i = 0; i < 10; i++) printf("%3d", number2[index[i]]);

	printf("\n원본\n");
	for (i = 0; i < 10; i++) printf("%3d", number2[i]);
}