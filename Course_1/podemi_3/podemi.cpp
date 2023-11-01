#include <stdlib.h>
#include <stdio.h>

int CheckFile(FILE *file_check);                            // проверка на открытие файлов
int PodschetUchastkov(FILE *file_input, FILE *file_output); // подсчет возрастания

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
                fprintf(file_output, "%d", PodschetUchastkov(file_input, file_output));

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

int CheckFile(FILE *file_check)
{
    if (file_check != NULL)
    {
        return 1;
    }
    fprintf(stdout, "You have some problems with files. Please check for correctness\n");
    return 0;
}

// подсчет возрастающих участков в последовательности
int PodschetUchastkov(FILE *file_input, FILE *file_output)
{
    // временые переменные которые будут хранения значений
    double number_1 = 0.0;
    double number_2 = 0.0;

    int global_counter = 0;

    int flag = 0; // данный флаг нужен для определения второго числа в файле. Если его нет, то в файле лишь
                  // одно число

    if (fscanf(file_input, "%lf", &number_1) == 1)
    {
        int counter = 0; // локальный счетчик
        flag = 1;        // в файле как минимум точно есть одно число
        while (fscanf(file_input, "%lf", &number_2) == 1)
        {
            flag = 0; // снос флага, проверка пройдена.
            if (number_1 < number_2)
            {
                counter++;
            }
            else
            {
                if (counter != 0)
                {
                    global_counter++;
                }

                counter = 0;
            }
            number_1 = number_2;
        }
        if (counter != 0)
        { // данная проверка нужна для конца последовательности.
            global_counter++;
        }
        if (flag == 1)
        { // если в файле лишь одно число, то 1
            return 0;
        }
    }
    else
    {
        global_counter = 0;
    }
    return global_counter;
}