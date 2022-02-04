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
	char buf[200];					// fgets 함수를 통해 얻은 값들을 저장할 buf 배열을 선언한다.
	FILE* fp = fopen("student.txt", "rt");		// 파일포인터 fp를 선언하고 fopen을 통해 student.txt 파일을 텍스트읽기모드로 연다. 
	if (fp == NULL) {						// 만약 fp가 NULL 값을 가리키면 파일이 열리지 않은 것이므로 오류임을 출력한다.
		printf("Fail to open file\n");
		return -1;
	}
	fgets(buf, sizeof(buf), fp);			// 학생들의 정보를 얻어올 텍스트 파일에서 첫번째줄은 필요없기 때문에 fgets 함수를 사용하여 fp 파일포인터가 첫번째 줄을 건너뛰게끔 한다.	
					
	for (int i = 0; i < 10; i++) {					// 10명의 학생의 정보를 저장할 것이기 때문에 for루프는 10번 돈다.
		fscanf(fp, "%s %d %d %d %d", record[i].name, &record[i].ID, &record[i].kor, &record[i].eng, &record[i].math);  // fscanf 함수를 이용하여 fp가 가리키는 파일에서의 값들을 record배열 내
																											           //STD구조체의 멤버들에 각각 저장한다.
		
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

	fp = fopen("record.txt", "wt");				// fopen을 통해 record.txt 파일을 텍스트 쓰기모드로 열고 fp는 record.txt파일을 가리킨다.
	if (fp == NULL) {							// 만약 fp가 NULL 값을 가리키면 파일이 열리지 않은 것이므로 오류임을 출력한다.
		printf("Fail to write file\n");
		return -1;
	}

	fprintf(fp, "\n---------------------------------------------\n");		
	fprintf(fp, "이름    학번 국어 영어 수학 합계 평균 석차");
	fprintf(fp, "\n---------------------------------------------\n");		// fprintf함수를 이용하여 fp가 가리키는 record.txt 파일에 성적표를 입력한다.
	for (int i = 0; i < 10; i++) {											// 10명이기 때문에 for문은 10번 돈다.
		fprintf(fp, "%s   %d   %d   %d  %d  %d  %2.2f  %d\n",record[i].name, record[i].ID, record[i].kor, record[i].eng,	// 이름, 학번, 국어, 영어, 수학, 합계, 평균, 석차를 차례로 파일에 입력시킨다.
															record[i].math, record[i].sum, record[i].avg, i+1);				// 이때 평균은 float값으로 소수점표현을 하고 석차는 배열인덱스에 1을 더하였다.
	}
	printf("Success to write file!\n");			// 모든 동작을 성공적으로 마치면 성공적으로 수행 했음을 출력한다.
}

