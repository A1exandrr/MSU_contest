#include <stdio.h>
#include <stdlib.h>

int CheckFile(FILE *file_check);           // проверка на открытие файлов
void Ekvivalentost(FILE *fin, FILE *fout); // главная функция решения задачи
void Read(FILE *fin, int **arr, int *init, int *arrOut, int n); // функция считывания
int Pow(int base, int exponent); // возведение в степень, ибо pow может вызвать погрешность
void Solving(FILE *fout, int **arr_1, int **arr_2, int *arrOut_1, int *arrOut_2, int init_1, int init_2, int n, int m);
int Memory(int ***arr, int l, int step); // выделение памяти для массивов
void Clean(int ***arr, int count);       // чистка выделенной памяти для указателей

int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *fin = NULL;
        FILE *fout = NULL;
        // получение файлов
        fin = fopen(argv[1], "r"); // открытия файла "чтение"
        if (CheckFile(fin))
        {
            fout = fopen(argv[2], "wb"); // открытия файла "запись"
            if (CheckFile(fout))
            {
                Ekvivalentost(fin, fout);
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
    {
        return 1;
    }
    fprintf(stdout, "You have some problems with files. Please check for correctness\n");
    return 0;
}

// главная функция выполнения
void Ekvivalentost(FILE *fin, FILE *fout)
{
    int init_1 = 0; // начальное состяоние первого автомата
    int init_2 = 0; // начальное состяоние второго автомата

    int flag_1 = 0;
    int flag_2 = 0; // необходимые флаги для очистки памяти

    /*============[ Работа с первым автоматом ]============*/

    int n; // начальное состояние первого автомата
    fscanf(fin, "%d", &n);
    int size_1 = Pow(2, n);
    int **arr_1 = NULL;                                      // массив состояний
    int *arrOut_1 = (int *)malloc(sizeof(int) * 2 * size_1); // функция выхода первого автомата
    if (Memory(&arr_1, 2, size_1) && arrOut_1)
    {
        flag_1 = 1;
        Read(fin, arr_1, &init_1, arrOut_1, n);
    }
    /*============[ Конец работы с первым автоматом ]============*/

    /*============[ Работа с вторым автоматом ]============*/

    int m; // начальное состояние второго автомата
    fscanf(fin, "%d", &m);
    int size_2 = Pow(2, m);
    int **arr_2 = NULL;                                      // массив состояний
    int *arrOut_2 = (int *)malloc(sizeof(int) * 2 * size_2); // функция выхода первого автомата
    if (Memory(&arr_2, 2, size_2) && arrOut_2)
    {
        flag_2 = 1;
        Read(fin, arr_2, &init_2, arrOut_2, m);
    }
    /*============[ Конец работы с вторым автоматом ]============*/

    Solving(fout, arr_1, arr_2, arrOut_1, arrOut_2, init_1, init_2, n, m);

    if (flag_1 && flag_2)
    {
        Clean(&arr_1, size_1);
        Clean(&arr_2, size_2);
        free(arrOut_1);
        free(arrOut_2);
    }
    else
        fprintf(stdout, "Please, fix all the problems\n");
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

// выделение памяти для указателей
int Memory(int ***arr, int l, int step)
{
    // выделение памяти для двумерного массива
    (*arr) = (int **)calloc(step, sizeof(int *));
    if (arr == NULL)
    {
        fprintf(stdout, "Error with memory for arr pointer\n");
        return 0;
    }

    // выделение памяти под каждую строку + проверка корректности
    for (int i = 0; i < step; i++)
    {
        (*arr)[i] = (int *)calloc(l, sizeof(int));
        if ((*arr)[i] == NULL)
        { // чистка предыдущих строк, если произошла ошибка
            for (int j = 0; j < i; j++)
                free((*arr)[j]);
            free(*arr);
            fprintf(stdout, "Error with memory for [%d] pointer\n", i);
            return 0;
        }
    }
    return 1;
}

// чистка выделенной памяти для указателей
void Clean(int ***arr, int count)
{
    for (int i = 0; i < count; i++)
        free((*arr)[i]);
    free(*arr);
    arr = NULL; // снос указателя
}

// функция считывания входного файла
void Read(FILE *fin, int **arr, int *init, int *arrOut, int n)
{
    char tmp;
    int i_tmp;
    *init = 0;

    fscanf(fin, "%c", &tmp);
    for (int i = 0; i < n; i++)
    {
        fscanf(fin, "%c", &tmp);
        if (tmp == '1')
            (*init) += Pow(2, n - i - 1);
    }

    for (int i = 0; i < n; i++)
    {
        fscanf(fin, "%d %d ", &i_tmp, &i_tmp, &tmp);
        for (int j = 0; j < 2 * Pow(2, n); j++)
        {
            fscanf(fin, "%c", &tmp);
            if (tmp == '1')
                arr[j / 2][j % 2] = arr[j / 2][j % 2] + Pow(2, n - i - 1);
        }
    }
    fscanf(fin, "%d %d%c", &i_tmp, &i_tmp, &tmp);

    for (int i = 0; i < 2 * Pow(2, n); i++)
    {
        fscanf(fin, "%c", &tmp);
        arrOut[i] = tmp - '0';
    }
    fscanf(fin, "%c", &tmp);
}

// Основная функция решения задачи
void Solving(FILE *fout, int **arr_1, int **arr_2, int *arrOut_1, int *arrOut_2, int init_1, int init_2, int n, int m)
{
    int flag = 1;
    int size = 1;

    int **arr_vis = NULL;
    int **arr_new = NULL;

    if (Memory(&arr_vis, Pow(2, m), Pow(2, n)) && Memory(&arr_new, 2, Pow(2, n + m)))
    {
        arr_new[0][0] = init_1;
        arr_new[0][1] = init_2;

        arr_vis[init_1][init_2] = 1;

        while (flag && size)
        {
            int num_1 = arr_new[size - 1][0];
            int num_2 = arr_new[size - 1][1];
            size--;

            for (int i = 0; i < 2; i++)
            {
                if (arrOut_1[2 * num_1 + i] != arrOut_2[2 * num_2 + i])
                {
                    flag = 0;
                    break;
                }
                int next_1 = arr_1[num_1][i];
                int next_2 = arr_2[num_2][i];
                if (arr_vis[next_1][next_2] == 0)
                {
                    arr_new[size][0] = next_1;
                    arr_new[size][1] = next_2;
                    size++;

                    arr_vis[next_1][next_2] = 1;
                }
            }
        }
        fprintf(fout, "%d", flag);
        Clean(&arr_vis, Pow(2, n));
        Clean(&arr_new, Pow(2, n + m));
    }
}
