#include <stdio.h>
#include <stdlib.h>

int Convertation(unsigned long long int num, int index)
{
    int d = 0, flag = 0;
    int size = sizeof(long long int) * 8;
    while (size > 0) {
        size--;
        d = (num & 1);
        if (d == 1)
        {
            index = flag;
        }
        flag++;
        num = num >> 1;
        if (num == 0)
        {
            break;
        }      
    }
    return index;
}

void Write_To_File(FILE* b, int index)
{
    fprintf(b, "%d", index);
}

int main(int argc, char* argv[])
{
    FILE* a, * b;
    unsigned long long int num = 0;
    int index = 0;
    if (argc == 3)
    {
        a = fopen(argv[1], "r");
        b = fopen(argv[2], "w");
        if (a != NULL && b != NULL)
        {
            fscanf(a, "%llu", &num);
            if (num >= 0)
            {
                index = Convertation(num, index);
                if (index >= 0)
                {
                    Write_To_File(b, index);
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return -1;
            }

        }
        if (a != NULL)
        {
            fclose(a);
        }
        if (b != NULL)
        {
            fclose(b);
        }
    }
    return 0;
}