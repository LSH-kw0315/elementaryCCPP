#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int num;
	char* ch = "*";
	scanf("%d", &num);
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < i+1; j++) {
			printf("%-c", '*');
		}
		printf("\n\n");
	}
	return 0;
}