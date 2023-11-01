#include <stdlib.h>
#include <stdio.h>

int position(FILE *file_input);  // определение позиции самой старшей единицы
int CheckFile(FILE *file_check); // проверка на открытие файлов

int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *file_input = NULL;
        FILE *file_output = NULL;
        // открытие файловё
        file_input = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(file_input) == 1)
        {
            file_output = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(file_output) == 1)
            {
                fprintf(file_output, "%d", position(file_input));
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
// выполенение основной логики
int position(FILE *file_input)
{
    unsigned long long int number = 0, pow = 1;
    if (fscanf(file_input, "%llu", &number) == 1)
    {
        if (number > 0)
        {

            for (int i = 63; i >= 0; i--)
            {
                if (number & (pow << i))
                {
                    return i;
                }
            }
        }
        else
        {
            return 0;
        }
    }
    return 0;
}