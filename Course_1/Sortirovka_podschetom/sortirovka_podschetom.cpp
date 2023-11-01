#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int CheckFile(FILE *file_check);                                        // проверка на открытие файлов
void WorkWithArr(FILE *file_input, FILE *file_output, int *arrNumbers); // запись чисел в массив
void Sortirovka(int *arrNumbers, int real_size);                        // сортировка подсчетом
void WriteArrToFile(FILE *file_output, int *arrNumbers, int real_size); // запись результата в файл

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
                int *arrNumbers = NULL;
                WorkWithArr(file_input, file_output, arrNumbers);

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

// запись чисел из файла в массив
void WorkWithArr(FILE *file_input, FILE *file_output, int *arrNumbers)
{
    int size = 0;
    int real_size = 0;
    // инициализация размера массива
    if (fscanf(file_input, "%d", &size) == 1)
    { // проверка размеры массива
        if (size > 0)
        {
            arrNumbers = (int *)malloc(sizeof(int) * size);
            if (arrNumbers != NULL)
            {
                for (int i = 0; (i < size) && (fscanf(file_input, "%d", &arrNumbers[i]) == 1); i++)
                { // определение реального количества чисел в файле
                    real_size++;
                }
                if (real_size != 0)
                { // проверка когда последовательность пустая
                    if (real_size < size)
                    { // уменьшение объема занимаемой памяти, если элементов оказалось меньше завявленных
                        arrNumbers = (int *)realloc(arrNumbers, sizeof(int) * real_size);
                    }
                    if (arrNumbers != NULL)
                    {
                        Sortirovka(arrNumbers, real_size);
                        WriteArrToFile(file_output, arrNumbers, real_size);
                        free(arrNumbers);
                    }

                    else
                    {
                        fprintf(stdout, "Critical error: Failed to allocate memory for pointer.\n");
                        fprintf(stdout, "Check operation realloc");
                    }
                }
                else
                {
                    free(arrNumbers);
                    fprintf(file_output, "0");
                }
            }
            else
            {
                fprintf(stdout, "Critical error: Failed to allocate memory for pointer\n");
                fprintf(stdout, "Check operation malloc");
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

// Реализация сортировки подсчетом
void Sortirovka(int *arrNumbers, int real_size)
{
    int i, j, max = -1; // локальные перемнные(счетчики и максимальное значение)
    int *count = NULL;  // массив-счетчик количества числа в числах массива.
    for (i = 0; i < real_size; i++)
    { // процесс получение наибольшего абсолютного значения для определения размера массива
        if (abs(arrNumbers[i]) > max)
        {
            max = abs(arrNumbers[i]);
        }
    }
    // увеличение размера массива в два раза(это из-за отрицательных чисел, чтобы работать с ними)
    int length_max = max * 2 + 1;
    count = (int *)malloc(sizeof(int) * length_max);
    if (count != NULL)
    {
        for (i = 0; i < length_max; i++)
        { // зануление всех элементов в массиве
            count[i] = 0;
        }
        for (i = 0; i < real_size; i++)
        {
            count[arrNumbers[i] + max]++;
        }
        i = 0;
        j = length_max - 1;

        while (i < real_size)
        {
            if (count[j] > 0)
            {
                // переместиться назад при вводе отсортированного массива
                count[j]--;
                arrNumbers[i] = j - max;
                i++;
            }
            else
            {
                j--;
            }
        }
        free(count);
    }
    else
    {
        fprintf(stdout, "Critical error. Failed to allocate memory for pointer");
    }
}

// запись отсортированного массива в файл
void WriteArrToFile(FILE *file_output, int *arrNumbers, int real_size)
{ // из-за отрицательных чисел, происходит инверсия массива
    fprintf(file_output, "%d ", real_size);
    for (int i = real_size - 1; i >= 0; i--)
    {
        fprintf(file_output, "%d ", arrNumbers[i]);
    }
}