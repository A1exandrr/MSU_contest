// Реализация алгоритма Блейка
// произвести склеивание наборов
#include <stdio.h>
#include <stdlib.h>

int CheckFile(FILE *file_check);                // проверка на открытие файлов
void Bleik(FILE *fin, FILE *fout);              // Реализация блейка
void Input(FILE *fin, int **arr, int l, int c); // считывание последовательностей из файла
void ShortenedDNF(int **arr, int *p_c, int l); // процесс склевания наборов(обобщенная функция)
int PossibilityGluing(int **arr, int l, int i, int j); // проверка на возможность склеивания
int Duplicate(int **arr, int index, int l);            // проверка на наличие монома
void Gluing(int **arr, int index, int l, int i, int j); // процесс склевания
void Sort(int **arr, int *p_c, int l);                  // Сортировка исходного массива
int Convert(int *arr, int l); // перевод в троичный вид для сортировки
int Pow(int base, int exponent); // возведение в степень, ибо pow может вызвать погрешность
void Output(FILE *fout, int **arr, int *p_c, int l); // запись ответа
int Memory(int ***arr, int l, int amount);           // выделение памяти для массивов
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
                Bleik(fin, fout);
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
            return 1;
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

// Алгоритм Блейка
void Bleik(FILE *fin, FILE *fout)
{
    int l = 0;                    // длина конъюнктов
    int c = 0;                    // количество конъюнктов
    fscanf(fin, "%d %d", &l, &c); // считывание первых двух чисел в файле
    int amount = c * l;           // размерность памяти необходмиой для массива
    int **arr = NULL; // массив для записи последовательности из файла
    int *p_c = &c;
    if (Memory(&arr, l, amount))
    {
        Input(fin, arr, l, c);
        ShortenedDNF(arr, p_c, l);
        Sort(arr, p_c, l);
        Output(fout, arr, p_c, l);
        Clean(&arr, amount);
    }
    else
        fprintf(stdout, "Please, fix all problems\n");
}

// выделение памяти для указателей
int Memory(int ***arr, int l, int amount)
{
    // выделение памяти для двумерного массива
    (*arr) = (int **)calloc(amount, sizeof(int *));
    if (arr == NULL)
    {
        fprintf(stdout, "Error with memory for arr pointer\n");
        return 0;
    }

    // выделение памяти под каждую строку + проверка корректности
    for (int i = 0; i < amount; i++)
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

// считывание наборов с файла
void Input(FILE *fin, int **arr, int l, int c)
{
    char tmp;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            fscanf(fin, "%c", &tmp);
            if (tmp == '0' || tmp == '1' || tmp == '*')
            {
                if (tmp == '1')
                    arr[i][j] = 1;
                if (tmp == '*')
                    arr[i][j] = 2;
            }
            else
                j--;
        }
    }
}
// склеивание(обощенная функция)
void ShortenedDNF(int **arr, int *p_c, int l)
{
    int index = *p_c - 1;
    for (int i = 0; i < *p_c; i++)
    {
        for (int j = i + 1; j < *p_c; j++)
        {
            if (PossibilityGluing(arr, l, i, j))
            {
                index++; // увеличение индекса для записи склейки
                Gluing(arr, index, l, i, j);
                (*p_c)++;
                if (Duplicate(arr, index, l))
                {
                    index--;
                    (*p_c)--;
                }
            }
        }
    }
}

// проверка на возможность склеивания
int PossibilityGluing(int **arr, int l, int i, int j)
{
    int gap = 0;
    // m индекс будет локальным для перебора всех возможных вариаций
    for (int m = 0; m < l; m++)
    {
        if (arr[i][m] != arr[j][m] && arr[i][m] != 2 && arr[j][m] != 2)
            gap++;
    }
    if (gap == 1)
        return 1;
    return 0;
}
// процесс склевания
void Gluing(int **arr, int index, int l, int i, int j)
{
    for (int m = 0; m < l; m++)
    {
        if (arr[i][m] != arr[j][m] && arr[i][m] != 2 && arr[j][m] != 2)
            arr[index][m] = 2; // склеивания с * в случае разных цифрах
        if (arr[i][m] == arr[j][m] && arr[i][m] != 2 && arr[j][m] != 2)
            arr[index][m] = arr[j][m];
        if (arr[i][m] == arr[j][m] && arr[i][m] == 2 && arr[j][m] == 2)
            arr[index][m] = arr[j][m];
        if (arr[i][m] != arr[j][m] && arr[i][m] == 2 && arr[j][m] != 2)
            arr[index][m] = arr[j][m];
        if (arr[i][m] != arr[j][m] && arr[i][m] != 2 && arr[j][m] == 2)
            arr[index][m] = arr[i][m];
    }
}

// проверка на наличие монома
int Duplicate(int **arr, int index, int l)
{
    int counter = 0;
    for (int i = 0; i < index; i++)
    {
        for (int m = 0; m < l; m++)
        {
            if (arr[index][m] == arr[i][m])
                counter++;
        }
        // если найдется совпадение, то снос этого монома
        if (counter == l)
        {
            for (int h = 0; h < l; h++)
                arr[index][h] = 0;
            return 1;
        }
        counter = 0;
    }
    return 0;
}
// Сортировка массива пузырьком
void Sort(int **arr, int *p_c, int l)
{
    int *tmp = (int *)calloc(l, sizeof(int));
    for (int i = 0; i < *p_c; i++)
    {
        for (int j = 0; j < *p_c - 1; j++)
        {
            if (Convert(arr[j], l) > Convert(arr[j + 1], l))
            {
                for (int k = 0; k < l; k++)
                {
                    tmp[k] = arr[j][k];
                    arr[j][k] = arr[j + 1][k];
                    arr[j + 1][k] = tmp[k];
                }
            }
        }
    }
    free(tmp);
}

// перевод в троичное представление строк
int Convert(int *arr, int l)
{

    int sum = 0;
    for (int i = 0; i < l; i++)
    {
        sum += arr[i] * Pow(3, l - i - 1);
    }
    return sum;
}

// возведение в степень с локальной реализацией
int Pow(int base, int exponent)
{
    int result = 1;
    for (exponent; exponent > 0; exponent--)
    {
        result = result * base;
    }
    return result;
}

// Запись результата в выходной файл
void Output(FILE *fout, int **arr, int *p_c, int l)
{
    fprintf(fout, "%d %d\n", l, *p_c);
    for (int i = 0; i < *p_c; i++)
    {
        for (int j = 0; j < l; j++)
        {
            if (arr[i][j] == 2)
            {
                fprintf(fout, "*");
            }
            else
                fprintf(fout, "%d", arr[i][j]);
        }
        fprintf(fout, "\n");
    }
}

// чистка выделенной памяти для указателей
void Clean(int ***arr, int count)
{
    for (int i = 0; i < count; i++)
        free((*arr)[i]);
    free(*arr);
    arr = NULL; // снос указателя
}
