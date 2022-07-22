#define _CRT_NO_SECURE_WARNINGS
#define PI 3.14
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
	int x;
	int y;
}Point;
void rectangle(Point* p1, Point* p2) {
	double area = (abs(p2->x - p1->x))*(abs(p2->y - p1->y));
	printf("rectangle area:%2g\n", area);
}
void circle(Point* p1, Point* p2) {
	double length = sqrt(((p2->x - p1->x)*(p2->x - p1->x)) + ((p2->y - p1->y)*(p2->y - p1->y)));
	double area = length*length*PI;
	printf("circle area:%2g\n", area);
}
int main(void) {
	Point p[2];
	printf("P1: ");
	scanf("%d %d", &p[0].x, &p[0].y);
	printf("P2: ");
	scanf("%d %d", &p[1].x, &p[1].y);
	rectangle(&p[0], &p[1]);
	circle(&p[0], &p[1]);
	return 0;
}