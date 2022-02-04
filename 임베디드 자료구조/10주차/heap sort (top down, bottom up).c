#include <stdio.h>
#include <stdlib.h>

void upheap(int* a, int k) {		// 아래서 부터 위로 올리는 heap정렬, a는 힙주소, k는 힙의 길이(밑부터 시작)
	int v;
	v = a[k];			// v에 힙의 꼬리 값 할당, k는 꼬리값을 나타내는 인덱스
	a[0] = INT_MAX;
	while (a[k / 2] <= v) {		// 만약 힙 안에서 부모값보다 자식 값이 클 경우 (v = 꼬리값)
		a[k] = a[k / 2];		// 부모값 끌어내림 (k/2 는 부모인덱스, k는 자식 인덱스)
		k /= 2;					// k인덱스 위로 올림
	}
	a[k] = v;			// 반복문을 빠져나올 때까지 올라간 k인덱스 값에 v, 즉 처음에 집어넣은 꼬리값 넣기
}

void downheap(int* a, int N, int k) {		// 위에서 부터 아래로 내려가는 heap 정렬, a는 힙주소, N은 힙길이, k는 1
	int i, v;
	v = a[k];			// v에 root값을 저장 (보통 k=1)
	while (k <= N / 2) {		// k가 internal node 개수보다 커지면 반복문 종료
		i = k << 1;		// i = 2*k, 즉 i노드는 k노드의 왼쪽 자식
		if (i < N && a[i] < a[i + 1]) i++;		// 만약 오른쪽 자식이 더 크면 오른쪽 자식을 i노드로 설정
		if (v >= a[i]) break;					// v값, 즉 root값이 자식값보다 크면 종료
		a[k] = a[i];			// 부모값에 자식값 넣음, 즉 자식값을 위로 올림
		k = i;					// k노드(부모노드)가 i노드(자식노드)로 내려가며 반복
	}
	a[k] = v;					// 반복문이 끝난 위치에 k노드에 처음 받은 root값 저장
}

void insert(int* a, int* N, int v) {		// N은 힙의 길이, a는 힙 주소, v는 데이터
	a[++(*N)] = v;						// 힙 길이를 증가시키며 해당 위치(꼬리)에 힙에 데이터를 넣는다.
	upheap(a, *N);						// 아래서부터 위로 올리는 정렬 수행
}

int extract(int* a, int* N) {				// N은 힙의 길이, a는 힙 주소
	int v = a[1];				// root값 v에 추출
	a[1] = a[(*N)--];			// 꼬리값을 root로 올림
	downheap(a, *N, 1);			// 위에서부터 아래로 내리는 정렬 수행

	return v;				// 추출한 값 반환
}

void heap_sort_top(int* a, int N) {		// N은 데이터 길이, a는 힙 주소, top down 방식
	int i;
	int hn = 0;		// 힙 길이

	for (i = 1; i <= N; i++) insert(a, &hn, a[i]);		// 연속해서 삽입하여 힙을 구성

	for (i = hn; i >= 1; i--) a[i] = extract(a, &hn);	// root값들을 추출하여 a의 뒤쪽으로 차례차례 배치, 오름차순 정렬
}

void heap_sort_bottom(int* a, int N) {		// N은 데이터 길이, a는 힙 주소, bottom up 방식
	int k, t;
	for (k = N / 2; k > 0; k--) downheap(a, N, k);		// 마지막 internal node 에서 부터 정렬 시작

	while (N > 1) {		// 데이터 길이가 1이하가 되면 종료
		t = a[1];
		a[1] = a[N];
		a[N] = t;		// root값과 꼬리값을 교체 , 오름차순 정렬
		downheap(a, --N, 1);		// 데이터 길이를 하나 줄여서 정렬 
	}
}

void main() {
	int a[15] = { INT_MAX, 4, -80, 1, 172, 13, 9, 39, -99, 2054, 10, -10, 3, 67, -28 };
	int b[15];
	int c[15];
	int i;

	for (i = 0; i < 15; i++) {
		b[i] = a[i];
		c[i] = a[i];
	}

	heap_sort_top(b, 14);			// a[0] 은 안쓰니까 데이터 길이에서 1 빼줘야됨

	for (i = 1; i < 15; i++) printf("%5d", b[i]);

	printf("\n");

	heap_sort_bottom(c, 14);		// a[0] 은 안쓰니까 데이터 길이에서 1 빼줘야됨

	for (i = 1; i < 15; i++) printf("%5d", c[i]);

	int d[15];


	for (i = 1; i < 15; i++) {
		d[i] = b[15 - i];			// 내림차순으로
	}

	printf("\n");
	for (i = 1; i < 15; i++) printf("%5d", d[i]);
}