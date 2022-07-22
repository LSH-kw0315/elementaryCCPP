#include <stdio.h>
int main(void){
    char* a="abcd";
    char* b="abcd";
    //a[0]='c';
    printf("%p %p \n",a,b);
    printf("%c %c \n",*a,*b);
    return 0;
}