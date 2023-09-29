#include <stdio.h>
#include <stdlib.h>
int main()
{
    int N = 100;
    int *arr = (int *)malloc(N * sizeof(int));
    arr[N] = 0;
}