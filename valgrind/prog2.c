#include <stdio.h>
#include <stdlib.h>
// #include <conio.h> 
int main()
{
    int N = 100;
    int *arr = (int *)malloc(N * sizeof(int));
    free(arr);
    int i = rand()%100;
    printf("%d\n",arr[i]);
}