// #pragma warning(disable:4996)
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n;
    char z;
    int pr;
    pr = 0;
    clock_t start, endd;
    start = clock();
    double total;
    FILE* in = fopen("input.txt", "r");
    FILE* out = fopen("output.txt", "w");
    fscanf(in, "%d ", &n);
    for (int i = 0; i < n; i++)
    {
        fscanf(in, "%c", &z);
    }
    if (pr == 1)
        printf("%c", z);
    int rows = pow(2, n + 1);
    int col = n + 1;
    int** arr = (int**)malloc(sizeof(int*) * rows);
    for (int i = 0; i < rows; i++)
    {
        arr[i] = (int*)malloc(sizeof(int) * (2 * col));
    }
    int m = 0;
    for (int i = 0; i < col; i++)
    {
        arr[0][i] = 0;
    }
    for (m = 1; m < rows; m++)
    {
        for (int j = 0; j < col; j++)
        {
            arr[m][j] = arr[m - 1][j];
        }
        n = col - 1;
        while (arr[m][n] == 1)
        {
            n--;
        }
        arr[m][n] = 1;

        for (int j = n + 1; j < col; j++)
        {
            arr[m][j] = 0;
        }
    }

    if (pr == 1)
    {
        for (int i = 0; i < rows; i++)
        {
            printf("\n");
            for (int j = 0; j < col; j++)
            {
                printf("%d", arr[i][j]);
            }
        }
    }

    char tm;
    /*char tab;
    for (int j = col; j < 2 * col; j++) {
        for (int i = 0; i < rows; i++) {
            fscanf(in, "%c", &tm);
            if ((tm == '0') || (tm == '1'))
                arr[i][j] = tm - '0';
            else {
                j--; break;
            }
        }
    }*/
    int tmp;
    char tmp_c;
    for (int j = col; j < 2 * col; j++)
    {
        fscanf(in, "%d %d ", &tmp, &tmp);
        for (int i = 0; i < rows; i++)
        {
            // fscanf(in, "%d", &arr[i][j]);
            fscanf(in, "%c", &tm);
            if ((tm == '0') || (tm == '1'))
            {
                arr[i][j] = tm - '0';
            }
            else
            {
                i--;
                break;
            }
        }
    }

    if (pr == 1)
    {
        printf("\n");
        for (int i = 0; i < rows; i++)
        {
            printf("\n");
            for (int j = 0; j < 2 * col; j++)
            {
                printf("%d", arr[i][j]);
            }
        }
    }

    // printf("\n");
    int index = 0;
    int end = 2 * col;
    int vis_count1 = 0;
    int vis_count2 = 0;
    int* arr_st1 = (int*)malloc(sizeof(int) * col);
    int* arr_st2 = (int*)malloc(sizeof(int) * col);
    int* arr_st3 = (int*)malloc(sizeof(int) * col);
    int* arr_st4 = (int*)malloc(sizeof(int) * col);
    //int* arr_visit1 = (int*)malloc(sizeof(int) * 2 * col);
    //int* arr_visit2 = (int*)malloc(sizeof(int) * 2 * col);
    int p1 = 0;
    int p2 = 0;
    int p3 = 0;
    int p4 = 0;
    int d = 0;
    int L = 0;
    int a = 0, b = 0;
    int count_d = 0;
    int count_l = 0;
    int count_l1 = 0;
    int Dead_end = 0;
    int j;
    int diff = 0;

    for (int i = 0; i < rows - 2; i += 2)
    {
        p4 = 0;
        for (j = i + 2; j < rows - 1; j += 2)
        {
            p1 = 0;
            for (int z = 0; z < 2; z++)
            {
                p2 = 0;
                for (int k = 0; k < col; k++)
                {
                    arr_st1[k] = arr[i + z][k];
                    arr_st2[k] = arr[j + z][k];
                    arr_st3[k] = arr_st1[k];
                    arr_st4[k] = arr_st2[k];

                } //  for (int k = 0; k < col; k++) {
                if (pr == 1)
                {
                    printf("a1=");
                    for (int k = 0; k < col; k++)
                    {
                        printf("%d", arr_st3[k]);
                    }
                    printf("\n");
                    printf("a2=");
                    for (int k = 0; k < col; k++)
                    {
                        printf("%d", arr_st4[k]);
                    }
                    printf("\n");
                }

                while (p2 == 0)
                {
                    count_d = 0;
                    count_l = 0;
                    count_l1 = 0;
                    diff = 0;
                    int m1 = -1;

                    for (int mk = 0; mk < rows - 1; mk++)
                    {
                        int p3 = 0;

                        for (int k = 0; k < col; k++)
                        {
                            if (arr_st3[k] != arr[mk + z][k])
                            {
                                p3 = 1;
                            }
                        }
                        if (p3 == 0)
                        {
                            m1 = mk + z;
                            // printf("m1=%d", m1);
                            // arr_visit1[vis_count1] = m1;
                            // vis_count1++;
                            break;
                        }

                    } // for (int mk = 0; mk < rows; mk++) {

                    int m2 = -1;
                    

                    for (int mk2 = 0; mk2 < rows - 1; mk2++)
                    {
                        int p3 = 0;
                        for (int k = 0; k < col; k++)
                        {
                            if (arr_st4[k] != arr[mk2 + z][k])
                            {
                                p3 = 1;
                            }
                        }
                        if (p3 == 0)
                        {
                            m2 = mk2 + z;
                            // printf("m2=%d", m2);
                            // rr_visit2[vis_count2] = m2;
                            // vis_count2++;
                            break;
                        }
                    } // for (int mk2 = 0; mk2 < rows - 1; mk2++) {
                    // printf("\n");
                    // printf("\n");
                    

                    if (m1 < 0) break;
                    if (m2 < 0) break;

                    /*  for (int l = 0; l < vis_count1; l++) {
                          printf("%d", arr_visit1[l]);
                      }
                      printf("\n");
                      for (int l = 0; l < vis_count2; l++) {
                          printf("%d", arr_visit2[l]);
                      }*/

                    for (int k = 0; k < col; k++)
                    {
                        arr_st3[k] = arr[m1 + z - z][k + col];
                        arr_st4[k] = arr[m2 + z - z][k + col];
                        if (arr_st3[col - 1] != arr_st4[col - 1])
                        {   
                            if (pr == 1) {
                                printf("non eq");
                            }
                           

                            p2 = 1;
                            p1 = 1;
                            break;
                        }

                        if (arr_st3[col - 1] == 1 && arr_st4[col - 1] == 1)
                        {
                            arr_st3[col - 1] = 0;
                            arr_st4[col - 1] = 0;
                        }

                    } // for k

                    // printf("\n");
                    if (p2 == 0)
                    {
                        if (pr == 1)
                        {
                            printf("a3=");
                            for (int k = 0; k < col; k++)
                            {
                                printf("%d", arr_st3[k]);
                            }
                            printf("\n");
                            printf("a4=");
                            for (int k = 0; k < col; k++)
                            {
                                printf("%d", arr_st4[k]);
                            }
                            printf("\n");
                        }

                        for (int k = 0; k < col - 1; k++)
                        {
                            if (arr_st3[k] == arr_st2[k] && arr_st4[k] == arr_st1[k])
                            {
                                count_d++;
                            }
                            if (arr_st3[k] == arr_st1[k])
                            {
                                count_l++;
                            }
                            if (arr_st4[k] == arr_st2[k])
                            {
                                count_l1++;
                            }
                            if (arr_st3[k] == arr_st4[k])
                            {
                                diff++;
                            }
                        }
                        if ((diff == col - 1) && (z == 1 || z == 0))
                        {
                            Dead_end++;
                            fprintf(out, " ");
                            printf("\n *** ");
                            for (int m = 0; m < n; m++)
                            {
                                fprintf(out, "%d", arr[i][m]);

                                //printf("%d", arr[i][m]);
                            }
                            fprintf(out, " ");
                            printf(" ");
                            for (int m = 0; m < n; m++)
                            {
                                fprintf(out, "%d", arr[j][m]);
                                printf("%d", arr[j][m]);
                            }
                            p2 = 1;
                            p1 = 1;
                            break;
                        }

                        if (count_d == col - 1)
                        {
                            d = 1;
                            a = d;
                        }



                        if (count_l == col - 1 && count_l1 == col - 1 && z == 0)
                        {
                            break;
                        }
                        // else
                         //{
                           //  count_l = 0;
                             //count_d = 0;
                             //diff = 0;
                             //count_l1 = 0;
                             //continue;
                         //}

                        if (count_l == col - 1 && count_l1 == col - 1 && z == 1)
                        {
                            p2 = 1;
                            p1 = 1;
                            break;
                        }

                        if ((a == 1 || b == 1) && z == 0)
                        {

                            break;
                        }
                        else if ((a == 1 || b == 1) && z == 1)
                        {
                           // printf("\n");
                            Dead_end++;
                            for (int m = 0; m < n; m++)
                            {
                                fprintf(out, "%d", arr[i][m]);
                            }
                            fprintf(out, " ");
                            for (int m = 0; m < n; m++)
                            {
                                fprintf(out, "%d", arr[j][m]);
                            }
                            p2 = 1;
                            p1 = 1;
                            break;
                        }

                    } // if p2==0

                    /*for (int k = 0; k < col; k++) {
                        printf("%d", arr_st3[k]);
                    }
                    printf("\n");
                    for (int k = 0; k < col; k++) {
                        printf("%d", arr_st4[k]);
                    }
                    */

                } //  while (p2 == 0) {

                count_l = 0;
                count_d = 0;
                diff = 0;
                count_l1 = 0;
                if (p2 == 1)
                    break;

            } //  for (int z = 0; z < 2; z++) {
            /*if (p1 == 1) {
                p4 = 1;
                continue;
            }*/
            printf("\n");
            /* if (Dead_end == ((pow(2, n)) * (pow(2, n) - 1) / 2)) {
                 p2 = 1; p1 = 1; p4 = 1; fprintf(out,"-1"); break;
             }*/
        } //  for (int j = i + 2; j < rows - 1; j = +2) {
        if (p4 == 1)
        {
            break;
        }
    } // for i

    if (Dead_end == 0)
    {
        fprintf(out, "-1");
    }
    endd = clock();
    total = (double)(endd - start) / CLOCKS_PER_SEC;
    printf("time=%.2f", total);
}
