#include <stdio.h>

int main(void)
{
    int a = 1;
    int b = 2;
    int c = 3;

    printf("주소:%d %d\n", &a, a);
    printf("주소:%d %d\n", &b, b);
    printf("주소:%d %d\n", &c, c);

    int * x = &a;
    printf("주소:%d %d\n", x, *x);
    return 0;
}