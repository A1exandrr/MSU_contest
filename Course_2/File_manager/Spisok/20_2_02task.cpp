#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //lasndalsjndkajsnda
    char str[] = "root/fil;l,;l,e/search/main.txt/knkasdfj";
    char *token = strtok(str, "/");
    while (token != NULL)
    {
        fprintf(stdout, "%s\n", token);
        token = strtok(NULL, "/");
    }
    return 0;
}
