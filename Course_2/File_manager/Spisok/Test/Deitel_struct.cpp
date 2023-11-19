#include <stdio.h>
#include <stdlib.h>

typedef struct First_struct
{
    int data;
    int *p_data;
    char name;
} f_struct;

f_struct *pointer;
f_struct man;

int main(int argc, char const *argv[])
{
    man.data = 10;
   pointer->p_data = &man.data;


    // fprintf(stdout, "%d\n", );
    fprintf(stdout, "%d\n", man.data);
    return 0;
}
