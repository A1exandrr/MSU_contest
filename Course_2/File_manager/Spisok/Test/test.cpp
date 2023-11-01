#include <stdio.h>
#include <stdlib.h>

struct Test
{
    int x;
    int y;
    int z;
};

int main(int argc, char const *argv[])
{
    int n = 10;
    int *p_n = &n;
    fprintf(stdout, "%d\n", *p_n);
    int m = 20;
    p_n = &m;
    fprintf(stdout, "%d\n", *p_n);
    return 0;
}
