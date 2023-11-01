#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int size = sizeof(int) * 8;

void sym_bayt(FILE *fin, FILE *fout);

int main(int argc, char *argv[]) {
    if (argc == 3) {
        FILE *fin = fopen(argv[1], "r");
        FILE *fout = fopen(argv[2], "w");

        if (fin && fout) {
            sym_bayt(fin, fout);

            fclose(fin);
            fclose(fout);
        }
    }

    return 0;
}

void sym_bayt(FILE *fin, FILE *fout) {
    int n, flag = 1, k = 383, count = 0;
    fscanf(fin, "%d", &n);
    n = abs(n);

    while (count < n) {
        for (int i = 0; i < size; i++) {
            if (((k >> i) & 1) != ((k >> (15 - i)) & 1)) {
                flag = 0;
                break;
            }
        }

        if (flag) {
            fprintf(fout, "%d ", k);
            count++;
        }
        flag = 1;
        k++;
    }
}