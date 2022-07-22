#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct person{
	char name[100];
	int age;
	char place[100];
}Person;
int main(void) {
	Person member[3];
	for (int i = 0; i < 3; i++) {
		scanf("%s %d %s", member[i].name, &member[i].age, member[i].place);
	}
	int cmp1 = member[0].age > member[1].age;
	int cmp2 = member[1].age > member[2].age;
	if (cmp1 == 1 && cmp2 == 0) {
		//첫번째 출력
		printf("name:%2s\nage:%2d\naddr:%2s\n", member[0].name, member[0].age, member[0].place);
	}
	else if (cmp1 == 0 && cmp2 == 1) {
		//두번재 출력
		printf("name:%2s\nage:%2d\naddr:%2s\n", member[1].name, member[1].age, member[1].place);
	}
	else {
		//세번째 출력
		printf("name:%2s\nage:%2d\naddr:%2s\n", member[2].name, member[2].age, member[2].place);
	}
	return 0;
}