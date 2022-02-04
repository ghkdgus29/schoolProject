#include <stdio.h>                    // printf ���� �Լ��� ����ϱ� ���� ��� ����             
#include <string.h>					  // memcpy �Լ����� ����ϱ� ���� ��� ����

typedef struct student {			// �л��� ���������� ���������� ���� sturct student����ü�� �����ϰ� 
	char name[10];					// typedef �� �̿��Ͽ� student ����ü ������ STD�� ��������ν� �����ϰ� �����ϴ�.
	int ID;
	int kor;
	int eng;
	int math;
	int sum;
	float avg;						// student ����ü ������ �̸�, �й�, ��������, ��������, ��������, �������� �հ�, �������� ����� 
}STD;								// �����Ѵ�.

void main() {
	STD record[100];					// STD����ü�� ��ĭ���� �ϴ� ��ĭ¥�� �迭, record �迭�� �����Ѵ�.
	STD temp;						// �������Ŀ��� �迭�� ������ �� �� �ӽ÷� STD ����ü�� ������ temp�� �����Ѵ�.
	char buf[200];					// fgets �Լ��� ���� ���� ������ ������ buf �迭�� �����Ѵ�.
	FILE* fp = fopen("student.txt", "rt");		// ���������� fp�� �����ϰ� fopen�� ���� student.txt ������ �ؽ�Ʈ�б���� ����. 
	if (fp == NULL) {						// ���� fp�� NULL ���� ����Ű�� ������ ������ ���� ���̹Ƿ� �������� ����Ѵ�.
		printf("Fail to open file\n");
		return -1;
	}
	fgets(buf, sizeof(buf), fp);			// �л����� ������ ���� �ؽ�Ʈ ���Ͽ��� ù��°���� �ʿ���� ������ fgets �Լ��� ����Ͽ� fp ���������Ͱ� ù��° ���� �ǳʶٰԲ� �Ѵ�.	
					
	for (int i = 0; i < 10; i++) {					// 10���� �л��� ������ ������ ���̱� ������ for������ 10�� ����.
		fscanf(fp, "%s %d %d %d %d", record[i].name, &record[i].ID, &record[i].kor, &record[i].eng, &record[i].math);  // fscanf �Լ��� �̿��Ͽ� fp�� ����Ű�� ���Ͽ����� ������ record�迭 ��
																											           //STD����ü�� ����鿡 ���� �����Ѵ�.
		
		record[i].sum = record[i].kor + record[i].eng + record[i].math;		// record[i]�迭�� ����ִ� STD ����ü�� sum ����� �������� ���� ���� �����Ѵ�.
		record[i].avg = record[i].sum / 3;									// record[i]�迭�� ����ִ� STD ����ü�� avg ����� ��հ�(record[i].sum ���� 3���� ������)�� �����Ѵ�.
	}
	for (int i = 0; i < 9; i++) {											// ���������� �ϱ� ���� ���� for���̴�.
		for (int j = 0; j < 9 - i; j++) {									// ���������� ���Ͽ� record[10] �迭�� ���׹��� ����� �������� ��ռ���� �����Ѵ�.	
			if (record[j].avg < record[j + 1].avg) {				// ���� �ռ� �迭�� ��հ��� �޹迭�� ��հ����� ������� if���� �����Ѵ�.						
				memcpy(&temp, record + j, sizeof(STD));				// memcpy�Լ��� ����� temp�� record[j] �� STD����ü ������ �����Ѵ�.
				memcpy(record + j, record + j + 1, sizeof(STD));	// memcpy�Լ��� ����� record[j]�� record[j+1] �� STD����ü ������ �����Ѵ�.
				memcpy(record + j + 1, &temp, sizeof(STD));			// memcpy�Լ��� ����� record[j+1]�� temp �� STD����ü ������ �����Ѵ�.
			}
		}																	// ���������� ������  record�迭�� �ε����� �������� ū ��հ��� ������ �ȴ�.
	}

	fp = fopen("record.txt", "wt");				// fopen�� ���� record.txt ������ �ؽ�Ʈ ������� ���� fp�� record.txt������ ����Ų��.
	if (fp == NULL) {							// ���� fp�� NULL ���� ����Ű�� ������ ������ ���� ���̹Ƿ� �������� ����Ѵ�.
		printf("Fail to write file\n");
		return -1;
	}

	fprintf(fp, "\n---------------------------------------------\n");		
	fprintf(fp, "�̸�    �й� ���� ���� ���� �հ� ��� ����");
	fprintf(fp, "\n---------------------------------------------\n");		// fprintf�Լ��� �̿��Ͽ� fp�� ����Ű�� record.txt ���Ͽ� ����ǥ�� �Է��Ѵ�.
	for (int i = 0; i < 10; i++) {											// 10���̱� ������ for���� 10�� ����.
		fprintf(fp, "%s   %d   %d   %d  %d  %d  %2.2f  %d\n",record[i].name, record[i].ID, record[i].kor, record[i].eng,	// �̸�, �й�, ����, ����, ����, �հ�, ���, ������ ���ʷ� ���Ͽ� �Է½�Ų��.
															record[i].math, record[i].sum, record[i].avg, i+1);				// �̶� ����� float������ �Ҽ���ǥ���� �ϰ� ������ �迭�ε����� 1�� ���Ͽ���.
	}
	printf("Success to write file!\n");			// ��� ������ ���������� ��ġ�� ���������� ���� ������ ����Ѵ�.
}

