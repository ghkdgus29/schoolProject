#include <stdio.h>
#include <stdlib.h>

void downheap(int* a, int N, int k) {	// 위에서 부터 아래로 정렬
	int i, v;
	v = a[k];				// v 에 값 추출
	while (k <= N / 2) {		// internal node 개수
		i = k << 1;				// i = k*2
		if (i < N && a[i] < a[i + 1]) i++;			// 오른쪽 왼쪽 중 큰 값과 교체
		if (v >= a[i]) break;						// 만약 v값이 자식값보다 크면 종료
		a[k] = a[i];								// v값이 자식값보다 작으면 자리교체
		k = i;										// 자식값을 k에 저장
	}
	a[k] = v;					// 모든 교체가 끝나고 해당 자리에 v값 저장
}

void heap_sort_bottom(int* a, int N) {		// bottom up 방식의 heap sort
	int k, t;
	for (k = N / 2; k > 0; k--) downheap(a, N, k);		// 모든 internal node 에 대하여 downheap 정렬

	while (N > 1) {
		t = a[1];
		a[1] = a[N];
		a[N] = t;				// 가장 아래에 있는 노드와(작은값) 가장 위에 있는 노드(큰값)을 교체
		downheap(a, --N, 1);	//	교체가 끝난 큰값은 놔두고 다시 정렬 --> 오름차순 정렬이 된다
	}
}								

void main() {
	int a[15] = { INT_MAX, 4, -80, 1, 117, 13, 76, 39, -99, 2054, 10 , -10, 3, 67, -28 };

	heap_sort_bottom(a, 14);		// a[0]는 정렬에 사용하지 않으므로 실제 데이터 길이에서 1을 빼준다.

	for (int i = 1; i < 15; i++) {
		printf("%5d", a[i]);			// 출력
	}
	printf("\n");
}