#include <stdio.h>
#include <stdlib.h>

typedef struct student {
	char grade;
	int mid_term;
	int final_term;
	int attendance;
}STD;

int main() {
	FILE* fp;
	STD st;

	fp = fopen("student.txt", "wt");
	if (fp == NULL) {
		printf("Fail to open file\n");
	}

	st.grade = 'B';
	st.mid_term = 56;
	st.final_term = 72;
	st.attendance = 10;

	fprintf(fp, "grade is %c\n", st.grade);
	fprintf(fp, "mid_term is %d\n", st.mid_term);
	fprintf(fp, "final_term is %d\n", st.final_term);
	fprintf(fp, "attendance is %d\n", st.attendance);

	fclose(fp);

	return 0;
}