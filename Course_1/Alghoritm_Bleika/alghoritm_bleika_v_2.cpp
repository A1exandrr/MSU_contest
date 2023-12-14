#include <stdio.h>
#include <stdlib.h>

void Reading(FILE *fin, int **arr, int amount, int length);
int Check_merging(int **arr, int length, int i, int j);
void Merging(int **arr, int length, int i, int j, int amount);
int Repetition(int **arr, int length, int amount);
void Sorting(int **arr, int length, int amount);
int Decimal(int *arr, int length);
int Pow(int k, int n);
void Write(FILE *fout, int **arr, int amount, int length);

int main(int argc, char const *argv[])
{
    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");

    if (fin != NULL)
    {
        if (fout != NULL)
        {
            int length, amount;
            fscanf(fin, "%d %d ", &length, &amount);
            int size = length * amount;
            int **arr = (int **)calloc(size, sizeof(int *));
            for (int i = 0; i < size; i++)
                arr[i] = (int *)calloc(length, sizeof(int));

            if (arr != NULL)
            {
                Reading(fin, arr, amount, length);
                for (int i = 0; i < amount; i++)
                {
                    for (int j = i + 1; j < amount; j++)
                    {
                        if (Check_merging(arr, length, i, j))
                        {
                            amount++;
                            Merging(arr, length, i, j, amount);
                            if (Repetition(arr, length, amount))
                                amount--;
                        }
                    }
                }
                Sorting(arr, length, amount);
                Write(fout, arr, amount, length);

                for (int i = 0; i < size; i++)
                    free(arr[i]);
                free(arr);
            }
            fclose(fin);
            fclose(fout);
        }
        else
            fclose(fin);
    }
    else
        fclose(fout);

    return 0;
}

void Reading(FILE *fin, int **arr, int amount, int length)
{
    char tmp;
    for (int i = 0; i < amount; i++)
    {
        for (int j = 0; j < length; j++)
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

int Check_merging(int **arr, int length, int i, int j)
{
    int diff = 0;
    for (int k = 0; k < length; k++)
        if (arr[i][k] != arr[j][k] && arr[i][k] != 2 && arr[j][k] != 2)
            diff++;

    if (diff == 1)
        return 1;
    return 0;
}

void Merging(int **arr, int length, int i, int j, int amount)
{
    int amount_index = amount - 1;
    for (int k = 0; k < length; k++)
    {
        if (arr[i][k] != arr[j][k] && arr[i][k] != 2 && arr[j][k] != 2)
            arr[amount_index][k] = 2;
        if (arr[i][k] == arr[j][k] && arr[i][k] != 2 && arr[j][k] != 2)
            arr[amount_index][k] = arr[j][k];
        if (arr[i][k] == arr[j][k] && arr[i][k] == 2 && arr[j][k] == 2)
            arr[amount_index][k] = arr[j][k];
        if (arr[i][k] != arr[j][k] && arr[i][k] == 2 && arr[j][k] != 2)
            arr[amount_index][k] = arr[j][k];
        if (arr[i][k] != arr[j][k] && arr[i][k] != 2 && arr[j][k] == 2)
            arr[amount_index][k] = arr[i][k];
    }
}

int Repetition(int **arr, int length, int amount)
{
    int counter = 0;
    int amount_index = amount - 1;
    for (int i = 0; i < amount_index; i++)
    {
        for (int k = 0; k < length; k++)
        {
            if (arr[amount_index][k] == arr[i][k])
                counter++;
        }
        if (counter == length)
        {
            for (int t = 0; t < length; t++)
                arr[amount_index][t] = 0;
            return 1;
        }
        counter = 0;
    }
    return 0;
}

void Sorting(int **arr, int length, int amount)
{
    int tmp;
    for (int i = 0; i < amount; i++)
    {
        for (int j = 0; j < amount - 1; j++)
        {
            if (Decimal(arr[j], length) > Decimal(arr[j + 1], length))
            {
                for (int k = 0; k < length; k++)
                {
                    tmp = arr[j][k];
                    arr[j][k] = arr[j + 1][k];
                    arr[j + 1][k] = tmp;
                }
            }
        }
    }
}

int Decimal(int *arr, int length)
{
    int result = 0;
    for (int i = 0; i < length; i++)
        result += arr[i] * Pow(3, length - i - 1);
    return result;
}

int Pow(int k, int n)
{
    int result = 1;
    for (int i = 0; i < n; i++)
        result *= k;
    return result;
}

void Write(FILE *fout, int **arr, int amount, int length)
{
    fprintf(fout, "%d %d\n", length, amount);
    for (int i = 0; i < amount; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (arr[i][j] != 2)
                fprintf(fout, "%d", arr[i][j]);
            else
                fprintf(fout, "*");
        }
        fprintf(fout, "\n");
    }
}