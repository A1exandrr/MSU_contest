#include <stdlib.h>
#include <stdio.h>

int CheckFile(FILE *file_check);                          // проверка на открытие файлов
void MathOperations(FILE *file_input, FILE *file_output); // основная функция выполнения логики кода
int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *file_input = NULL;
        FILE *file_output = NULL;

        file_input = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(file_input) == 1)
        {
            file_output = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(file_output) == 1)
            {
                MathOperations(file_input, file_output);
                // закрытие файлов после  работы
                fclose(file_input);
                fclose(file_output);
                return 0;
            }
            else
            {
                fclose(file_input);
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        fprintf(stdout, "Error: Not enough arguments to work correctly\n");
        return 1;
    }
}

// проверка на открытие файлов
int CheckFile(FILE *file_check)
{
    if (file_check != NULL)
    {
        return 1;
    }
    fprintf(stdout, "You have some problems with files. Please check for correctness\n");
    return 0;
}

// основные математические операции
void MathOperations(FILE *file_input, FILE *file_output)
{
    double sum_1 = 0.0;
    double sum_2 = 0.0, n, x;

    if (fscanf(file_input, "%lf", &x) == 1)
    {
        sum_1 = n;
        while (fscanf(file_input, "%lf", &n) == 1)
        {
            sum_2 = sum_2 * x + sum_1;
            sum_1 = sum_1 * x + n;
        }
        fprintf(file_output, "%lf %lf", sum_1, sum_2);
    }
    else
    {
        fprintf(file_output, "0");
    }
}