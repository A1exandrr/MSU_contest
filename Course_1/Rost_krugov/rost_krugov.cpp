#include <stdlib.h>
#include <stdio.h>

struct rost_krugov
{
    double x;
    double y;
};

typedef struct rost_krugov TPoint;

void WriteCoordinatsInArr(FILE *fin, rost_krugov *point);

int main(int argc, char const *argv[])
{
    TPoint *point = NULL; // указатель на массив структур координат

    FILE *fin = fopen(argv[1], "r");

    if (fin != NULL)
    {
        FILE *fout = fopen(argv[2], "wb");
        if (fout != NULL)
        {
            WriteCoordinatsInArr(fin, point);
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

    return 0;
}

void Write
