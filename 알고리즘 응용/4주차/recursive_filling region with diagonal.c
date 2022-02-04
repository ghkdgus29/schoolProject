#include <stdio.h>

int canvas[7][7] = { 0,0,0,0,0,0,0,
					0,0,0,1,0,0,0,
					0,0,0,1,0,0,0,
					0,1,1,1,1,0,0,
					0,0,0,0,0,1,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0, };    // filling region�� ��Ÿ�� canvas�� 2�����迭�� ����

void recur_fill(int x, int y) {        // canvas ���� 1�� ����(ǥ�õ� ����) �� 2�� �ٲٴ� (��ĥ�ϴ�) �Լ��̴�.
	if (!(canvas[y][x] == 1)) return;   // ���� canvas���� 1�� �ƴ� ���� (0�� 2)�� ������ �Լ��� �����Ѵ�.
	else {								// ���������� �������� �ʴ� ��� 
		canvas[y][x] = 2;				// 1�� 2�� �ٲ۴�. ��ĥ�� �ش��Ѵ�.
		recur_fill(x - 1, y);			
		recur_fill(x + 1, y);
		recur_fill(x, y - 1);			// �Է¹��� seed�� �� �� �� �� ������ ���� recursive �Լ�ȣ��� ���� 1�� ������ 2�� �ٲٰ� 
		recur_fill(x, y + 1);			// 1�� ���ٸ� recursive �Լ��� ����������.
		recur_fill(x - 1, y - 1);
		recur_fill(x - 1, y + 1);
		recur_fill(x + 1, y - 1);		// �Է¹��� seed�� �밢�� ������ ���� recursive �Լ�ȣ��� ���� 1�� ������ 2�� �ٲٰ� 
		recur_fill(x + 1, y + 1);		// 1�� ���ٸ� recursive �Լ��� ����������.
	}
}

void main() {
	int i, j;							// for�� �ݺ��� ���� ���� ����
	recur_fill(3, 1);					// 3,1 ������ seed�� �ϴ� recur_fill ȣ��

	for (i = 0; i < 7; i++) {			// ������ canvas�� ����ϱ� ���� ���� for���̴�.
		for (j = 0; j < 7; j++) {
			printf("%-2d", canvas[i][j]);
		}
		printf("\n");
	}

}