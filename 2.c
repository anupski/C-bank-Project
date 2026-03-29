#include <stdio.h>

void function()
{
    int a = 20;
    int b = 30;
    int c = 44;
    int avg;

    avg = a + b + c / 3;
    printf("The average of a, b, c is: %d", avg);
}
int main()
{
    function();
    return 0;
}