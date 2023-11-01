#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *file = fopen(argv[1], "r");

    int number = 0;
    fseek(file, 6, SEEK_CUR);
    fscanf(file, "%d", &number);
    fprintf(stdout, "%d ", number);
    fscanf(file, "%d", &number);
    fprintf(stdout, "%d ", number);
    fscanf(file, "%d", &number);
    fprintf(stdout, "%d ", number);
    fscanf(file, "%d", &number);
    fprintf(stdout, "%d ", number);
    fscanf(file, "%d", &number);
    fprintf(stdout, "%d ", number);
    return 0;
}
