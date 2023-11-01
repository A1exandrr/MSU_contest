// Шефферовость
#include <stdio.h>
#include <stdlib.h>

int CheckFile(FILE *file_check); // проверка на открытие файлов
void Shefferovost(FILE *fin, FILE *fout);
int T0();
int T1();
int Pow(int base, int exponent); // возведение в степень
void Read(FILE *fin, int *arr, int n, int size);

int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *fin = NULL;
        FILE *fout = NULL;
        // получение файлов
        fin = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(fin) == 1)
        {
            fout = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(fout) == 1)
            {
                // считываем последовательность k^2 размерность
                // при считывании мы проверяем на всех k-1 элементов в последовательности
                // двумерный массив k / k^k
                Shefferovost(fin, fout);
                // закрытие файлов после работы
                fclose(fin);
                fclose(fout);
                return 0;
            }
            else
            {
                fclose(fin);
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
        return 1;
    fprintf(stdout, "You have some problems with files. Please check for correctness\n");
    return 0;
}

void Shefferovost(FILE *fin, FILE *fout)
{
    int n; // количество переменных 2
    int k; // кол-во у функции k(значность логики) - запись в двумерный массив(длина)
    fscanf(fin, "%d%d", &k, &n);
    int size = Pow(n, k);
    int *arr = (int *)malloc(sizeof(int) * size);
    Read(fin, arr, n, size);
    for (int i = 0; i < size; i++)
    {
        fprintf(stdout, "%d\n", arr[i]);
    }
}

// Функция возведения в степень(сделано чтобы не было ошибок)
int Pow(int base, int exponent)
{
    int result = 1;
    for (exponent; exponent > 0; exponent--)
    {
        result = result * base;
    }
    if (result > 0)
        return result;
    return -1;
}

void Read(FILE *fin, int *arr, int n, int size)
{
    char tmp;
    fscanf(fin, "%c", &tmp);
    for (int i = 0; i < size; i++)
    {
        fscanf(fin, "%c", &tmp);
        arr[i] = tmp - 48;
    }
}
