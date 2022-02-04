#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>		// 시간측정을 위한 헤더

#define SIZE 10

void select_sort(int* a, int N) {		// seletion sort 함수, a는 데이터배열, N은 데이터의 길이
	int min;
	int min_idx;
	int x, y;

	for (y = 0; y < N - 1; y++) {		// 0~ 맨끝에서 두번째까지 선택
		min_idx = y;				// 가장 작은 값의 인덱스는 y로 설정
		min = a[y];					// 가장 작은 값은 a[y] 로 설정
		for (x = y + 1; x < N; x++) {		// y번째 다음 값부터 맨끝까지 선택(검사)
			if (min > a[x]) {			// 그 다음 검사값들 (x번째 값들) 중 min값보다 작은 값이 나오면
				min = a[x];				// min값 (최소값)은 그 x번째 값으로 업데이트
				min_idx = x;			// 인덱스값 역시 업데이트
			}
		}
		a[min_idx] = a[y];				// 모든 x값 검사가 끝나면 가장 작은 x번째 값 위치에 y값 집어넣기
		a[y] = min;						// y번째 위치엔 min값 (가장 작은 x번째 값) 집어넣기
	}
}

void main() {
	int number[SIZE] = { 9,3,1,7,4,6,9,2,10,5 };			// 데이터

	LARGE_INTEGER freq, start, stop;
	double diff;

	QueryPerformanceFrequency(&freq);// computer frequency
	QueryPerformanceCounter(&start);// starting point

	select_sort(number, SIZE);							// 정렬 수행
	for (int i = 0; i < 10000000; i++); // 더미루프

	QueryPerformanceCounter(&stop); //stopping point
	diff = (double)(stop.QuadPart - start.QuadPart) / freq.QuadPart;

	printf("%lf\n", diff); // 시간출력

	printf("정렬결과 : ");
	for (int i = 0; i < 10; i++) {
		printf("%3d", number[i]);			// 정렬된 값 출력
	}
	printf("\n");


}