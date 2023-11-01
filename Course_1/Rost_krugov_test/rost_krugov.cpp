#include <stdlib.h>
#include <stdio.h>

struct rost_krugov
{
    double x = 0.0;
    double y = 0.0;
};


int CheckFiles(FILE *file_input, FILE *file_output);        // проверка на открытие файлов
void SolvingEx(FILE *file_input);
void WritingSolveInFile(FILE *file_output);

int main(int argc, char const *argv[])
{
    FILE *file_input = NULL;
    FILE *file_output = NULL;
    // получение файловё
    file_input = fopen(argv[1], "r");
    file_output = fopen(argv[2], "wb");

    if (CheckFiles(file_input, file_output) == 1)
    {
        
    }
    fclose(file_input);
    fclose(file_output);
    return 0;
}

// проверка на открытие файлов
int CheckFiles(FILE *file_input, FILE *file_output)
{
    if (file_input != NULL && file_output != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void SolvingEx(FILE *file_input){

    struct rost_krugov center_1, center_2;

     if(fscanf(file_input, "%lf %lf %lf %lf", center_1.x, center_1.y, center_2.x, center_2) == 4){

     }
     else{
        fprintf(stdout, "Error");
     }
}