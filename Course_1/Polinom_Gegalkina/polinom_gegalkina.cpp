// Реализация полинома Жегалкина
#include <stdio.h>
#include <stdlib.h>

int CheckFile(FILE *file_check); // проверка на открытие файлов
void Polinom(FILE *fin, FILE *fout);
unsigned int Pow(int base, int exponent); // возведение в степень, ибо pow может вызвать погрешность
int Memory(int ***triangle, unsigned int step); // выделение память под указатели
void ToBinary(FILE *fout, int *arr_bin, int i, int n); // перевод чисел относительно их индекса в
                                                       // последовательности в двочиный вид
void ReadSet(FILE *fin, int **triangle, unsigned int step); // считывания первоначальной последовательности в файле
void Pascal(int **triangle, unsigned int step); // Построение треугольника паскаля для полинома
int Counter(int **triangle, unsigned int step); // счетчик единиц в треугольнике
void Total(FILE *fout, int **triangle, int k, int n, unsigned int step, int counter);
// Перевод индекса единиц в двоичный вид в
// треугольнике + запись в файл
void Clean(int ***triangle, unsigned int step); // очистка выделенной памяти под указател
int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *fin = NULL;
        FILE *fout = NULL;
        // получение файловё
        fin = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(fin) == 1)
        {
            fout = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(fout) == 1)
            {
                Polinom(fin, fout);
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

// Работа с полином и с входящим файлом
void Polinom(FILE *fin, FILE *fout)
{
    unsigned int step = 0; // степень и необходимая длина последовательности
    int k = 0;
    int n = 0;       // количество необходимов битов
    int counter = 0; // счетчик для реализациии двумерного массива
    fscanf(fin, "%d %d", &k, &n);
    step = Pow(k, n);
    int **triangle = NULL; // указатель для треугольника
    if (Memory(&triangle, step))
    {
        char tmp; // временая переменя чтобы пропустить пробел
        fscanf(fin, "%c", &tmp);
        ReadSet(fin, triangle, step);
        Pascal(triangle, step);
        Total(fout, triangle, k, n, step, Counter(triangle, step));
        Clean(&triangle, step);
    }
    else
        fprintf(stdout, "Please, correct all problems\n");
}

// Функция возведения в степень(сделано чтобы не было ошибок)
unsigned int Pow(int base, int exponent)
{
    unsigned int result = 1;
    for (exponent; exponent > 0; exponent--)
    {
        result = result * base;
    }
    return result;
}

// выделение памяти под указатель и проверка на корректность
int Memory(int ***triangle, unsigned int step)
{
    // выделение памяти для двумерного массива
    (*triangle) = (int **)calloc(step, sizeof(int *));
    if (triangle == NULL)
    {
        fprintf(stdout, "Error with memory for triangle_arr pointer\n");
        return 0;
    }

    // выделение памяти под каждую строку + проверка корректности
    for (int i = 0; i < step; i++)
    {
        // сразу будем выделять память под строки в виде треугольника
        (*triangle)[i] = (int *)calloc(step - i, sizeof(int));
        if ((*triangle)[i] == NULL)
        {
            // очистка выделенно памяти в случае ошибки
            for (int j = 0; j < i; j++)
                free((*triangle)[j]);
            free(*triangle);
            fprintf(stdout, "Error with memory for arr[%d] pointer\n", i);
            return 0;
        }
    }
    return 1;
}

// перевод в двочиный вид единиц
void ToBinary(FILE *fout, int *arr_bin, int i, int n)
{
    unsigned int num = i;
    int c = 0;
    int tmp = n;
    while (n >= 0)
    {
        arr_bin[c] = num % 2;
        num /= 2;
        c++;
        n--;
    }
    for (int h = tmp; h >= 0; h--)
        fprintf(fout, "%d", arr_bin[h]);
}

// Построение треугольника паскаля для полинома
void Pascal(int **triangle, unsigned int step)
{
    for (int i = 1; i < step; i++)
        for (int j = 0; j < step - i; j++)
            triangle[i][j] = (triangle[i - 1][j] ^ triangle[i - 1][j + 1]);
}

// функция считывания первоначальной строчки в файле
void ReadSet(FILE *fin, int **triangle, unsigned int step)
{
    char tmp = 0;
    for (int i = 0; i < step; i++)
    {
        fscanf(fin, "%c", &tmp);
        if (tmp == '1')
            triangle[0][i] = 1;
    }
}

// счетчик единиц в треугольнике
int Counter(int **triangle, unsigned int step)
{
    int counter = 0;
    for (int i = 0; i < step; i++)
    {
        if (triangle[i][0])
            counter++;
    }
    return counter;
}

// Перевод индекса единиц в двоичный вид в треугольнике + запись в файл
void Total(FILE *fout, int **triangle, int k, int n, unsigned int step, int counter)
{
    int *arr_bin = (int *)calloc(n, sizeof(int)); // массив для перевода в двоичный вид
    fprintf(fout, "%d %d %d\n", k, n, counter);
    for (int i = 0; i < step; i++)
        if (triangle[i][0])
        {
            ToBinary(fout, arr_bin, i, n - 1);
            fprintf(fout, " 1\n");
        }
    free(arr_bin);
}

// чистка памяти, снос указателя
void Clean(int ***triangle, unsigned int step)
{
    for (int i = 0; i < step; i++)
    {
        free((*triangle)[i]);
    }
    free(*triangle);
}
