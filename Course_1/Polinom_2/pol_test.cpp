#include <stdio.h>

int main(int argc, char *argv[]) 
{
    if (argc == 3)
    {
        FILE *fin = fopen(argv[1], "r");
        FILE *fout = fopen(argv[2], "w");

        if(fin != NULL && fout != NULL)
        {
            double sum_1 = 0, sum_2 = 0, n, x;

            if(fscanf(fin, "%lf", &x) > 0)
            {
                sum_1 = n;
                while(fscanf(fin, "%lf", &n) > 0)
                {
                    sum_2 = sum_2 * x + sum_1;
                    sum_1 = sum_1 * x + n;
                }

                fprintf(fout, "%lf %lf", sum_1, sum_2);
            } else 
            {
                fprintf(fout, "0");
            }
            
            fclose(fin);
            fclose(fout);
        }
    }

    return 0;
}
