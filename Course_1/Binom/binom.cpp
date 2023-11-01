#include <stdlib.h>
#include <stdio.h>

int CheckFiles(FILE *file_input, FILE *file_output);    // проверка на открытие файлов
void SolvingBinom(FILE *file_input, FILE *file_output); // вычесления биномиальных коэффициентов
unsigned long long SolvingKoef(int n, int k);
void WritingResult(FILE *file_output, unsigned long long *arrKoef, int degree);
int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *file_input = NULL;
        FILE *file_output = NULL;
        // получение файловё
        file_input = fopen(argv[1], "r");
        file_output = fopen(argv[2], "wb");

        if (CheckFiles(file_input, file_output) == 1)
        { // запись ответа в файл
            SolvingBinom(file_input, file_output);
            // закрытие файлов после работы
            fclose(file_input);
            fclose(file_output);
        }

        return 0;
    }
    else
    {
        fprintf(stdout, "Error: Not enough arguments to work correctly\n");
        return 1;
    }
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

// вычесления биномиальных коэффициентов
void SolvingBinom(FILE *file_input, FILE *file_output)
{ // сложность алгоритма от степени,для которой идет расчет. Может происходить классическое переполнение
    // Если степень будет больше 70, то работа алгоритма значительно замедляется. Все что выше 90 будет
    // вычесляться несколько минут. Так же, может происходить легкое отклонение. До 63 степени считает точно
    // Можно использовать тип double и вычеслять хоть 100, 1000 степени, но там придется использовать
    // округление, в следствение чего резко падает точность вычеслений. До 57 степени расчет
    // будет точным.
    int degree = 0;
    if (fscanf(file_input, "%d", &degree) == 1)
    {
        if (degree >= 0)
        {
            fprintf(file_output, "%d ", degree + 1); // количества коэф

            unsigned long long *arrKoef = (unsigned long long *)malloc(sizeof(unsigned long long) * (degree + 1));
            if (arrKoef != NULL)
            {
                for (int i = 0; i <= degree; i++)
                {
                    arrKoef[i] = SolvingKoef(degree, i);
                }
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
unsigned long long SolvingKoef(int n, int k)
{ // высчитывание базовых коэф
    if (k > n / 2)
        k = n - k;
    if (k == 1)
        return n;
    if (k == 0)
        return 1;
        //точность
    unsigned long long koef; // основнная переменная, куда будут записываться значения
        koef = 1;
        for (int i = 1; i <= k; ++i)
        {
            koef *= n - k + i;
            koef /= i;
        }
    return koef;
}

// запись результата в файл
void WritingResult(FILE *file_output, unsigned long long *arrKoef, int degree)
{
    for (int i = 0; i <= degree; i++)
    {
        fprintf(file_output, "%llu ", arrKoef[i]);
    }
}