#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int dif = 'a' - 'A';
	int i = 0;
	char str[100];
	scanf("%s", str);
	while (str[i] != 0) {
		if (str[i] >= 'A' && str[i] <= 'z') {
			if (str[i] >= 'a' && str[i] <= 'z') {
				str[i] -= dif;
				printf("%c",str[i]);
			}
			else
				printf("%c", str[i]);
		}
		i++;
	}
	printf("\n");
	return 0;
}