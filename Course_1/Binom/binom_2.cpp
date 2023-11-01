#include <stdlib.h>
#include <stdio.h>

int CheckFile(FILE *file_check);                                                // проверка на открытие файлов
void SolvingBinom(FILE *file_input, FILE *file_output);                         // работа с биномиальными коэффициентами
void SolvingKoef(unsigned long long *arrKoef, int degree);                      // вычесление самих коэффициентов.
void WritingResult(FILE *file_output, unsigned long long *arrKoef, int degree); // запись результата в файл
int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *file_input = NULL;
        FILE *file_output = NULL;
        // получение файловё
        file_input = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(file_input) == 1)
        {
            file_output = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(file_output) == 1)
            {
                SolvingBinom(file_input, file_output);
                // закрытие файлов после работы
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

// вычесления биномиальных коэффициентов
void SolvingBinom(FILE *file_input, FILE *file_output)
{ // Алгоритм с вычеслением комбинаторной формулы показал что появляется погрешность на 63 степени.
    // Поэтому будем просто складывать.
    int degree = 0;
    if (fscanf(file_input, "%d", &degree) == 1)
    {
        if (degree >= 0)
        {
            fprintf(file_output, "%d ", degree + 1); // количества коэф

            unsigned long long *arrKoef = (unsigned long long *)malloc(sizeof(unsigned long long) * (degree + 1));
            if (arrKoef != NULL)
            {
                SolvingKoef(arrKoef, degree);
                WritingResult(file_output, arrKoef, degree);
                free(arrKoef);
            }
            else
            {
                fprintf(stdout, "Error with malloc");
            }
        }
        else
        {
            fprintf(file_output, "0");
        }
    }
    else
    {
        fprintf(file_output, "0");
    }
}

// вычесление самого коэф
void SolvingKoef(unsigned long long *arrKoef, int degree)
{ // высчитывание самих коэф, но простым сложением
    int i = 1, k = 1;
    arrKoef[0] = 1;
    arrKoef[degree] = 1;
    for (i; i < degree; i++)
    {
        arrKoef[i] = 1;
        for (k = i; k >= 1; k--)
            arrKoef[k] = arrKoef[k] + arrKoef[k - 1];
    }
}

// запись результата в файл
void WritingResult(FILE *file_output, unsigned long long *arrKoef, int degree)
{
    for (int i = 0; i <= degree; i++)
    {
        fprintf(file_output, "%llu ", arrKoef[i]);
    }
}