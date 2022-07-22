#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
int main(void) {
	char str[100];
	char cmp[100];
	int cnt = 0;
	scanf("%s", str);
	scanf("%s", cmp);
	for (int i = 0; i < strlen(str); i++) {
		if (!strncmp(&str[i], cmp, strlen(cmp))) {
			cnt++;
			i += strlen(cmp) - 1;
		}
	}
	printf("%d \n", cnt);
	return 0;
}