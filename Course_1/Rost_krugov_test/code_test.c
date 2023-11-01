#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct Circle
{
    double x, y;
};

void swap(double *a, double *b)
{
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *ff = fopen(argv[2], "w");
    struct Circle c1, c2;
    fscanf(f, "%lf %lf %lf %lf", &c1.x, &c1.y, &c2.x, &c2.y);

    if (c1.x > c2.x)
       swap(&c1.x, &c2.x);
    if (c1.y > c2.y)
       swap(&c1.y, &c2.y);

    if (c1.x == c2.x)
    {
        double ans = abs(c2.y - c1.y) / 2; //середина отрезка
        double tmp = ans - (int)ans; //проверка на целочисленность
        int ans1 = (int)ans + (tmp != 0);
        fprintf(ff, "%d %d", ans1,ans1);
        return 0;
    }
    else if (c1.y == c2.y)
    {
        double ans = abs(c2.y - c1.y) / 2;
        double tmp = ans - (int)ans;
        int ans1 = (int)ans + (tmp != 0);
        fprintf(ff, "%d %d", ans1,ans1);
        return 0;
    }
    else
    {
        double h = sqrt(pow(abs(c2.x - c1.x), 2) + pow(abs(c2.y - c1.y), 2));
        h /= 2;
        double tmp = h - (int)h;
        // h += (int)h + (tmp != 0);
        int h1 = (int)h + (tmp != 0);
        fprintf(ff, "%d %d", h1, h1);
    }
    return 0;
}
