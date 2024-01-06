#include <stdio.h>
#include <stdlib.h>

int CheckFile(FILE *file_check); // проверка на открытие файлов
void Sostoyania(FILE *fin, FILE *fout);
int Memory(int ***arr, int l, int step); // выделение памяти для массивов
int Pow(int base, int exponent); // возведение в степень, ибо pow может вызвать погрешность
void Read(FILE *fin, int **arr, char *arr_out, int len_1, int len_2); // считывание последовательностей из файла
char Solve(int **arr, char **arr_check, char *arr_out, int i, int j);
void Answer(FILE *fout, char **arr_check, int n, int step);
void ToBinary(FILE *fout, int i, int n); // перевод чисел относительно их индекса
// последовательности в двочиный вид
void Clean(int ***arr, int count); // чистка выделенной памяти для указателей
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
                Sostoyania(fin, fout);
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

// сам алгоритм
void Sostoyania(FILE *fin, FILE *fout)
{
    int n = 0; // длина начального состояния автомата
    fscanf(fin, "%d", &n);
    int step = Pow(2, n);
    int **arr = NULL;
    char *arr_out = (char *)malloc(sizeof(char) * (step * 2));

    if (Memory(&arr, 2, step) && arr_out)
    {
        int flag = 1;
        Read(fin, arr, arr_out, n, Pow(2, n + 1));
        // временый костыль в виде выделения памяти под двумерный массив char
        // двумернный массив char с нулями
        char **arr_check = (char **)malloc(sizeof(char *) * step);
        if (arr_check)
        {
            for (int i = 0; i < step; i++)
            {
                // если на пересечении i и j стоит 0 тогда они эквивалентны
                arr_check[i] = (char *)malloc(sizeof(char) * step);
                for (int j = 0; j < step; j++)
                {
                    arr_check[i][j] = '0';
                }
            }
            while (flag)
            {
                flag = 0;
                for (int i = 0; i < step - 1; i++)
                { // j = i + 1 чтобы не идти по диагонали матрицы, ибо это тривиальные случаи(они все равны 0)
                    for (int j = i + 1; j < step; j++)
                    {
                        if (arr_check[i][j] == '0')
                        {
                            arr_check[i][j] = Solve(arr, arr_check, arr_out, i, j);
                            arr_check[j][i] = arr_check[i][j]; // для симметрии
                            if (arr_check[i][j] == '1')
                            {
                                flag = 1;
                            }
                        }
                    }
                }
            }
            Answer(fout, arr_check, n, step);
            // очистка для arr_check
            for (int i = 0; i < step; i++)
                free(arr_check[i]);
            free(arr_check);
            arr_check = NULL;
        }
        else
            fprintf(stdout, "You have some problems with memory for arr_check");
    }
    Clean(&arr, 2);
    free(arr_out);
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

// Функция возведения в степенnullь(сделано чтобы не было ошибок)
int Pow(int base, int exponent)
{
    int result = 1;
    for (exponent; exponent > 0; exponent--)
    {
        result = result * base;
    }
    return result;
}

// считывание наборов с файла
void Read(FILE *fin, int **arr, char *arr_out, int len_1, int len_2)
{
    // 2 ^ (n + 1)
    // 2 3 10101011 : 2(значность логики) и 3 нам не нужны, записываем в мусор (tmp) 10101011 - количество символов
    // равняется 2 ^ 3
    char tmp_c;
    int tmp_i;
    fscanf(fin, "%c", &tmp_c);
    for (int i = 0; i < len_1; i++)
        fscanf(fin, "%c", &tmp_c);

    for (int i = 0; i < len_1; i++)
    {
        fscanf(fin, "%d %d%c", &tmp_i, &tmp_i, &tmp_c); // для пропуска мусора в входном файле в виде 2 3 и пробела
        for (int j = 0; j < len_2; j++)
        {
            fscanf(fin, "%c", &tmp_c);
            if (tmp_c == '1')
            {
                arr[j / 2][j % 2] = arr[j / 2][j % 2] + Pow(2, len_1 - i - 1);
            }
        }
    }
    fscanf(fin, "%d %d%c", &tmp_i, &tmp_i, &tmp_c);
    for (int i = 0; i < len_2; i++) // запись выхода 2 3 10111011(последняя строчка в входном файле)
        fscanf(fin, "%c", &arr_out[i]);
    for (int i = 0; i < len_1; i++)
    {
        for (int j = 0; j < len_2; j++)
        {
            fprintf(stdout, "%d", arr[i][j]);
        }
        fprintf(stdout, "\n");
    }
}

char Solve(int **arr, char **arr_check, char *arr_out, int i, int j)
{
    int next_1 = 0; // следующее по i
    int next_2 = 0; // следующее по j
    for (int k = 0; k < 2; k++)
    {
        next_1 = arr[i][k];
        next_2 = arr[j][k];
        if (arr_check[next_1][next_2] == '1' || arr_check[next_2][next_1] == '1')
            return '1';
        if (arr_out[2 * i + k] != arr_out[2 * j + k])
            return '1';
    }
    return '0';
}

void Answer(FILE *fout, char **arr_check, int n, int step)
{
    int flag = 1;
    for (int i = 0; i < step - 1; i++)
    {
        for (int j = i + 1; j < step; j++)
        {
            if (arr_check[i][j] == '0')
            {
                flag = 0;
                ToBinary(fout, i, n);
                fprintf(fout, " ");
                ToBinary(fout, j, n);
                fprintf(fout, " ");
            }
        }
    }
    if (flag == 1)
    {
        fprintf(fout, "-1");
    }
}

// перевод в двочиный вид единиц
void ToBinary(FILE *fout, int i, int n)
{
    for (int j = 0; j < n; j++)
        fprintf(fout, "%d", (i / Pow(2, n - j - 1)) % 2);
}

// чистка выделенной памяти для указателей
void Clean(int ***arr, int count)
{
    for (int i = 0; i < count; i++)
        free((*arr)[i]);
    free(*arr);
    arr = NULL; // снос указателя
}
