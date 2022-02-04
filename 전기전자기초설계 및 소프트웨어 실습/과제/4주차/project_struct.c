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

	printf("������ �Է��ϼ���.\n");					// ����Լ��� ���� �ȳ��޴´�.
	for (int i = 0; i < 10; i++) {					// 10���� �л��� ������ ������ ���̱� ������ for������ 10�� ����.
		printf("%d��° �л�\n�̸�:", i + 1);		// ���° �л����� ��Ÿ����.			
		scanf("%s", record[i].name);				// �̸��� Ű����� �Է��ϰ� record[i]�迭�� ����ִ� STD ����ü�� name ����� �����Ͽ� Ű���� �Է°��� �����Ѵ�.
		printf("�й�: ");		
		scanf("%d", &record[i].ID);					// �й��� Ű���� �Է��ϰ� record[i]�迭�� ����ִ� STD ����ü ID ����� �����Ͽ� Ű���� �Է°��� �����Ѵ�.
		printf("\n��������: ");
		scanf("%d", &record[i].kor);				// ���������� Ű���� �Է��ϰ� record[i]�迭�� ����ִ� STD ����ü kor ����� �����Ͽ� Ű���� �Է°��� �����Ѵ�.
		printf("��������: ");
		scanf("%d", &record[i].eng);				// ���������� Ű���� �Է��ϰ� record[i]�迭�� ����ִ� STD ����ü eng ����� �����Ͽ� Ű���� �Է°��� �����Ѵ�.
		printf("��������: ");
		scanf("%d", &record[i].math);				// ���������� Ű���� �Է��ϰ� record[i]�迭�� ����ִ� STD ����ü math ����� �����Ͽ� Ű���� �Է°��� �����Ѵ�.
		printf("\n");
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
	printf("\n---------------------------------------------\n");		
	printf("�̸�    �й� ���� ���� ���� �հ� ��� ����");
	printf("\n---------------------------------------------\n");			// ����ǥ�� �����ֱ� ���� print �Լ�
	for (int i = 0; i < 10; i++) {											// 10���̱� ������ for���� 10�� ����.
		printf("%s   %d   %d   %d  %d  %d  %2.2f  %d\n",record[i].name, record[i].ID, record[i].kor, record[i].eng,			// �̸�, �й�, ����, ����, ����, �հ�, ���, ������ ���ʷ� ��½�Ų��.
															record[i].math, record[i].sum, record[i].avg, i+1);				// �̶� ����� float������ �Ҽ���ǥ���� �ϰ� ������ �迭�ε����� 1�� ���Ͽ���.
	}
	
}

