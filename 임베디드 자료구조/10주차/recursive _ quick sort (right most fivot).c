#include <stdio.h>
#include <stdlib.h>

void quick_sort(int* a, int N) {		// quick sort 함수, a는 데이터주소, N은 데이터길이
	int v, t;
	int i, j;
	if (N > 1) {			// 종료 조건, 데이터 길이가 1이하가 되면 종료
		v = a[N - 1];			// v에 피벗 저장
		i = -1;					// i인덱스는 왼쪽끝부터 시작
		j = N - 1;				// j인덱스는 오른쪽끝부터 시작
		while (1) {
			while (a[++i] < v);		// 왼쪽부터 탐색해서 피벗보다 큰값이 있으면 stop
			while (a[--j] > v);		// 오른쪽부터 탐색해 피벗보다 작은값이 있으면 stop
			if (i >= j) break;		// i, j가 같아지면 반복문 빠져나감
			t = a[i];
			a[i] = a[j];
			a[j] = t;				// a[i]값과 a[j]값을 바꾼다, 즉 왼쪽엔 피벗보다 작은값, 오른쪽엔 피벗보다 큰값들이 쌓인다.
		}
		t = a[i];
		a[i] = a[N - 1];
		a[N - 1] = t;				// 반복문을 빠져나가면, i=j인 인덱스 위치 값과 피벗값을 바꾼다

		quick_sort(a, i);			// 피벗값의 좌측을 다시 퀵정렬
		quick_sort(a + i + 1, N - i - 1);		// 피벗값의 우측을 다시 퀵정렬
	}
}

void main() {
	int a[15] = { 9, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };

	quick_sort(a, 15);

	for (int i = 0; i < 15; i++) printf("%5d", a[i]);
}