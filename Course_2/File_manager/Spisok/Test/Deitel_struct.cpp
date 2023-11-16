#include <stdio.h>
#include <stdlib.h>

typedef struct First_struct
{
    int data;
    char name;
} f_struct;

f_struct *pointer;
f_struct man;

int main(int argc, char const *argv[])
{
    man man.data = 12;

    fprintf(stdout, "%d\n", );
    fprintf(stdout, "%d\n", man.data);
    return 0;
}
