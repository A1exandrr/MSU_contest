#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char *result;
    char *str_1 = strdup("asdasd");
    char *str_2 = strdup("laskdmlsak");
    result = strdup(strcat(str_1, str_2));
    fprintf(stdout, "%s", result);
    return 0;
}
