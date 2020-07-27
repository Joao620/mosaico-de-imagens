#include <stdlib.h>
#include <stdio.h>

int main()
{
    // int *x = malloc(sizeof(int) * 2);
    // x[0] = 420;
    // x[1] = 69;

    int* x = (int *)0x5563388902a0;

    //printf("agora a giripoca vai piar %p\n", x);

    ReadProcessMemory(x);

    return 0;
}