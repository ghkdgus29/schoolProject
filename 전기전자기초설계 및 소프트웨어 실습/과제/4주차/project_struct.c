#include <stdio.h>                    // printf 등의 함수를 사용하기 위한 헤더 선언             
#include <string.h>					  // memcpy 함수등을 사용하기 위한 헤더 선언

typedef struct student {			// 학생의 개인정보와 성적정보를 담을 sturct student구조체를 선언하고 
	char name[10];					// typedef 를 이용하여 student 구조체 선언을 STD를 사용함으로써 간단하게 가능하다.
	int ID;
	int kor;
	int eng;
	int math;
	int sum;
	float avg;						// student 구조체 내에는 이름, 학번, 국어점수, 영어점수, 수학점수, 점수들의 합계, 점수들의 평균을 
}STD;								// 저장한다.

void main() {
	STD record[100];					// STD구조체를 한칸으로 하는 백칸짜리 배열, record 배열을 선언한다.
	STD temp;						// 버블정렬에서 배열의 정렬을 할 때 임시로 STD 구조체를 저장할 temp를 선언한다.

	printf("다음을 입력하세요.\n");					// 출력함수를 통해 안내받는다.
	for (int i = 0; i < 10; i++) {					// 10명의 학생의 정보를 저장할 것이기 때문에 for루프는 10번 돈다.
		printf("%d번째 학생\n이름:", i + 1);		// 몇번째 학생인지 나타낸다.			
		scanf("%s", record[i].name);				// 이름을 키보드로 입력하고 record[i]배열에 들어있는 STD 구조체에 name 멤버에 접근하여 키보드 입력값을 저장한다.
		printf("학번: ");		
		scanf("%d", &record[i].ID);					// 학번을 키보드 입력하고 record[i]배열에 들어있는 STD 구조체 ID 멤버에 접근하여 키보드 입력값을 저장한다.
		printf("\n국어점수: ");
		scanf("%d", &record[i].kor);				// 국어점수를 키보드 입력하고 record[i]배열에 들어있는 STD 구조체 kor 멤버에 접근하여 키보드 입력값을 저장한다.
		printf("영어점수: ");
		scanf("%d", &record[i].eng);				// 영어점수를 키보드 입력하고 record[i]배열에 들어있는 STD 구조체 eng 멤버에 접근하여 키보드 입력값을 저장한다.
		printf("수학점수: ");
		scanf("%d", &record[i].math);				// 수학점수를 키보드 입력하고 record[i]배열에 들어있는 STD 구조체 math 멤버에 접근하여 키보드 입력값을 저장한다.
		printf("\n");
		record[i].sum = record[i].kor + record[i].eng + record[i].math;		// record[i]배열에 들어있는 STD 구조체의 sum 멤버에 점수들을 합한 값을 저장한다.
		record[i].avg = record[i].sum / 3;									// record[i]배열에 들어있는 STD 구조체의 avg 멤버에 평균값(record[i].sum 값을 3으로 나눈값)을 저장한다.
	}
	for (int i = 0; i < 9; i++) {											// 버블정렬을 하기 위한 이중 for문이다.
		for (int j = 0; j < 9 - i; j++) {									// 버블정렬을 통하여 record[10] 배열에 뒤죽박죽 저장된 정보들을 평균순대로 정렬한다.	
			if (record[j].avg < record[j + 1].avg) {				// 만약 앞선 배열의 평균값이 뒷배열의 평균값보다 작을경우 if문을 실행한다.						
				memcpy(&temp, record + j, sizeof(STD));				// memcpy함수를 사용해 temp에 record[j] 의 STD구조체 정보를 저장한다.
				memcpy(record + j, record + j + 1, sizeof(STD));	// memcpy함수를 사용해 record[j]에 record[j+1] 의 STD구조체 정보를 저장한다.
				memcpy(record + j + 1, &temp, sizeof(STD));			// memcpy함수를 사용해 record[j+1]에 temp 의 STD구조체 정보를 저장한다.
			}
		}																	// 버블정렬이 끝나면  record배열의 인덱스가 작을수록 큰 평균값을 가지게 된다.
	}
	printf("\n---------------------------------------------\n");		
	printf("이름    학번 국어 영어 수학 합계 평균 석차");
	printf("\n---------------------------------------------\n");			// 성적표를 보여주기 위한 print 함수
	for (int i = 0; i < 10; i++) {											// 10명이기 때문에 for문은 10번 돈다.
		printf("%s   %d   %d   %d  %d  %d  %2.2f  %d\n",record[i].name, record[i].ID, record[i].kor, record[i].eng,			// 이름, 학번, 국어, 영어, 수학, 합계, 평균, 석차를 차례로 출력시킨다.
															record[i].math, record[i].sum, record[i].avg, i+1);				// 이때 평균은 float값으로 소수점표현을 하고 석차는 배열인덱스에 1을 더하였다.
	}
	
}

