#include <stdlib.h>
#include <stdio.h>

const int size = sizeof(int) * 8; // отвечает за 8 битные числа

int CheckFile(FILE *file_check);                         // проверка на открытие файлов
void BitOperations(FILE *file_input, FILE *file_output); // основная функция выполнения логики кода
int main(int argc, char const *argv[])
{
    if (argc == 3)
    { // проверка на подачу аргументов в консоль
        FILE *file_input = NULL;
        FILE *file_output = NULL;

        file_input = fopen(argv[1], "r"); // попытка открытия файла "чтение"

        if (CheckFile(file_input) == 1)
        {
            file_output = fopen(argv[2], "wb"); // попытка открытия файла "запись"
            if (CheckFile(file_output) == 1)
            {
                BitOperations(file_input, file_output);
                // закрытие файлов после  работы
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

// основные битовые операции
void BitOperations(FILE *file_input, FILE *file_output)
{
    int number, flag = 1, curent_number = 383, count = 0;
    if (fscanf(file_input, "%d", &number) == 1)
    {
        while (count < number)
        {
            for (int i = 0; i < size; i++)
            {
                if (((curent_number >> i) & 1) != ((curent_number >> (15 - i)) & 1))
                {
                    flag = 0;
                    break;
                }
            }

            if (flag)
            {
                fprintf(file_output, "%d ", curent_number);
                count++;
            }
            flag = 1;
            curent_number++;
        }
    }
    else
    {
        fprintf(file_output, "0");
    }
}