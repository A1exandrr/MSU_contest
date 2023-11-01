#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int size;
    int data;       
}Create;


int main(int argc, char const *argv[])
{
    Create fm;
    fscanf(stdin, "%d", fm.size);
    char *result = (char*)malloc(sizeof(char) * fm.size);
    char str[] = "root/file/search/main.txt/knkasdfj";
    char token = (char)strtok(str, "/");
    while (token != NULL && token != '.')
    {
        fprintf(stdout, "%s\n", token);
        token = (char)strtok(NULL, "/");
    }
    return 0;
}
