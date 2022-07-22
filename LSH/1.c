#define _CRT_NO_SECURE_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int arr[10];
	int odd = 0, even = 0;
	int* ptr = arr;
	for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
		scanf("%d", ptr+i);
	}
	for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
		if (*(ptr + i) % 2 == 0) 
			even++;
		else 
			odd++;
	}
	printf("odd:%2d\neven:%2d\n", odd, even);
	return 0;
}