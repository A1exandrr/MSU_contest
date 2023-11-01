#include <stdlib.h>
#include <stdio.h>

int CheckFile(FILE *file_check);                                           // проверка на открытие файлов
void WorkWithArr(FILE *file_input, FILE *file_output, double *arrNumbers); // запись чисел в массив
void Sortirovka(double *arrNumbers, int real_size);                        // двунаправленная сортировка пузырьком
void WriteArrToFile(FILE *file_output, double *arrNumbers, int real_size); // запись результата в файл
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
                double *arrNumbers = NULL;
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
void WorkWithArr(FILE *file_input, FILE *file_output, double *arrNumbers)
{
    int size = 0;
    int real_size = 0;
    // инициализация размера массива
    if (fscanf(file_input, "%d", &size) == 1)
    { // проверка размеры массива
        if (size > 0)
        {
            arrNumbers = (double *)malloc(sizeof(double) * size);
            if (arrNumbers != NULL)
            {
                for (int i = 0; (i < size) && (fscanf(file_input, "%lf", &arrNumbers[i]) == 1); i++)
                { // определение реального количества чисел в файле
                    real_size++;
                }
                if (real_size != 0)
                { // проверка на пустое множество
                    if (real_size < size)
                    { // уменьшение объема занимаемой памяти, если элементов оказалось меньше завявленных
                        arrNumbers = (double *)realloc(arrNumbers, sizeof(double) * real_size);
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

// Реализация сортировки двунаправленного пузырька
void Sortirovka(double *arrNumbers, int real_size)
{
    double number = 0.0;
    for (int i = 1; i < real_size; i++)
    {
        if (arrNumbers[i] < arrNumbers[i - 1])
        {
            number = arrNumbers[i];
            arrNumbers[i] = arrNumbers[i - 1];
            arrNumbers[i - 1] = number;
            for (int j = i; j > 0; j--)
            {
                if (arrNumbers[j - 1] > arrNumbers[j])
                {
                    number = arrNumbers[j];
                    arrNumbers[j] = arrNumbers[j - 1];
                    arrNumbers[j - 1] = number;
                }
            }
        }
    }
}

// запись отсортированного массива в файл
void WriteArrToFile(FILE *file_output, double *arrNumbers, int real_size)
{
    fprintf(file_output, "%d ", real_size);
    for (int i = 0; i < real_size; i++)
    {
        fprintf(file_output, "%lf ", arrNumbers[i]);
    }
}