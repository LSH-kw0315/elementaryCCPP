#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
int main(void) {
	char name1[100];
	char name2[100];
	char buf[20];
	int cnt1 = 0, cnt2 = 0;
	printf("A 파일: ");
	scanf("%s",name1);
	printf("B 파일: ");
	scanf("%s", name2);
	FILE* fp1 = fopen(name1,"rt");
	FILE* fp2 = fopen(name2,"rt");
	while (feof(fp1) != 0) {
		fgets(buf, sizeof(buf), fp1);
		cnt1++;
	}
	while (feof(fp2) != 0) {
		fgets(buf, sizeof(buf), fp2);
		cnt2++;
	}
	fclose(fp1);
	fclose(fp2);
	if (cnt1 == cnt2) {
		printf("동일한 파일입니다. \n");
	}
	Sleep(5000);
	return 0;
}