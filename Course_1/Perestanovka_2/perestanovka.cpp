#include <stdlib.h>
#include <stdio.h>

int CheckFiles(FILE *file_input, FILE *file_output);
void WriteNumbersInArray(FILE *file_input, FILE *file_output, double *arrNumbers);
void PerestanovkaArr(double *arrNumbers, int real_size);
void WriteArrInFIle(FILE *file_output, double *arrNumbers, int real_size);

int main(int argc, char const *argv[])
{
    FILE *file_input = NULL;
    FILE *file_output = NULL;

    file_input = fopen(argv[1], "r");
    file_output = fopen(argv[2], "wb");

    if (CheckFiles(file_input, file_output) == 1)
    {
        double *arrNumbers = NULL;
        WriteNumbersInArray(file_input, file_output, arrNumbers);
        free(arrNumbers);  // очистка памяти
        arrNumbers = NULL; // снос указателя
    }
    fclose(file_input);
    fclose(file_output);
    return 0;
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

// запись чисел из файла в массив
void WriteNumbersInArray(FILE *file_input, FILE *file_output, double *arrNumbers)
{
    int size = 0;
    int real_size = 0;
    fscanf(file_input, "%d", &size); // инициализация размера массива
    if (size > 0)
    {
        arrNumbers = (double *)malloc(sizeof(double) * size);
        for (int i = 0; (i < size) && (fscanf(file_input, "%lf", &arrNumbers[i]) == 1); i++)
        { // определение реального количества чисел в файле
            real_size++;
        }
        if (real_size < size)
        { // уменьшение объема занимаемой памяти, если элементов оказалось меньше завявленных
            arrNumbers = (double *)realloc(arrNumbers, sizeof(double) * real_size);
        }
        if (real_size == 0)
        {
            fprintf(file_output, "%d", size);
        }
        else
        {
            PerestanovkaArr(arrNumbers, real_size);
            WriteArrInFIle(file_output, arrNumbers, real_size);
        }
    }
    else
    {
        fprintf(file_output, "0");
    }
}

// перестановка элементов массива в обратном порядке
void PerestanovkaArr(double *arrNumbers, int real_size)
{
    int number = 0; // временная переменная для хранения числа
    for (int i = 0, j = 1; i < real_size / 2; i++, j++)
    {
        number = arrNumbers[i];
        arrNumbers[i] = arrNumbers[real_size - j];
        arrNumbers[real_size - j] = number;
    }
}

// запись готового массива в файл
void WriteArrInFIle(FILE *file_output, double *arrNumbers, int real_size)
{
    fprintf(file_output, "%d ", real_size);
    for (int i = 0; i < real_size; i++)
    {
        fprintf(file_output, "%lf ", arrNumbers[i]);
    }
}