#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int data;
    int *p_data;
    char name;
} F_struct;

F_struct *pointer;
F_struct man;

int main(int argc, char const *argv[])
{
    man.data = 10;
   pointer->p_data = &man.data;


    // fprintf(stdout, "%d\n", );
    fprintf(stdout, "%d\n", man.data);
    return 0;
}
