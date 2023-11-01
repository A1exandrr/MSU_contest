#include <stdlib.h>
#include <stdio.h>

int old_counter = 0;                                         // глобальная переменая, отвечающая за максимальную последовательность в файле
int CheckFiles(FILE *file_input, FILE *file_output);         // проверка на открытие файлов
void PodschetUchastkov(FILE *file_input, FILE *file_output); // подсчет постоянных участков
void SearchTheBiggestAmount(int counter);                    // поиска максимальной последовательности
void WriteResultInFile(FILE *file_output, int counter);      // запись результата в файл
int main(int argc, char const *argv[])
{
    FILE *file_input = NULL;
    FILE *file_output = NULL;
    // получение файловё
    file_input = fopen(argv[1], "r");
    file_output = fopen(argv[2], "wb");

    if (CheckFiles(file_input, file_output) == 1)
    {
        PodschetUchastkov(file_input, file_output);

        // закрытие файлов после работы
        fclose(file_input);
        fclose(file_output);    
    }
    
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

// подсчет возрастающих участков в последовательности
void PodschetUchastkov(FILE *file_input, FILE *file_output)
{
    // временые переменные которые будут хранения значений
    double number_1 = 0.0;
    double number_2 = 0.0;

    int flag = 0; // флаг нужен для проверки на то, что в файле более двух файлов
                  // для сравнения
                  /*Надо будет переписать без флагов*/
    if (fscanf(file_input, "%lf", &number_1) == 1)
    {
        int counter = 1;
        for (;;)
        {
            if (fscanf(file_input, "%lf", &number_2) == 1)
            {
                flag = 1; // проверка на то, что в файле более одного числа
                if (number_1 == number_2)
                {
                    counter++;
                }
                else
                { /*Здесь проверки нужны чтобы не записывать в файл нули, когда в
                 файле только два числа, когда в файле не происходит выполнения по условию*/
                    flag = 0;
                    if (counter != 1)
                    {
                        flag = 1;
                        SearchTheBiggestAmount(counter);
                    }
                    counter = 1;
                }
                number_1 = number_2; // поэтапный сдвиг
            }
            else
            {
                if (flag == 0)
                { // проверка на то, если в файле только одно число
                    SearchTheBiggestAmount(1);
                }
                break;
            }
        }
        if (counter != 1)
        { // проверка когда последовательность закончилась, и происходит
            // выполнение по условию задачи
            SearchTheBiggestAmount(counter);
        }
    }
    else
    { // проврка на то, что если файл просто пустой
        SearchTheBiggestAmount(0);
    }
    WriteResultInFile(file_output, old_counter);
}

// запись результата в файл
void SearchTheBiggestAmount(int counter)
{
    if (old_counter < counter || (old_counter == 0))
    {
        old_counter = counter;
    }
}

// запись результата в файл
void WriteResultInFile(FILE *file_output, int counter)
{
    fprintf(file_output, "%d", counter);
}
// Author: Alexander