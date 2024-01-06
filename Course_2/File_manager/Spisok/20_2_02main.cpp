#include "20_2_02task.cpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

static int VALUE_MAX_LENGTH = 32;
static int ABSOLUTE_PATH_MAX_LEN = 128;

int main(int argc, char const *argv[])
{
    // char *dir_now = strdup("");
    file_manager_t fm;
    setup_file_manager(&fm);
    fprintf(stdout, "Result create: %d\n", fm.create(100));
    fprintf(stdout, "Result of creating folders %d\n", fm.create_dir("Main"));
    // fm.create_dir("/Main/test1");
    // fm.create_dir("/Main/test1/test2");
    // fm.change_dir("/Main/test1/test2/..");
    fprintf(stdout, "Result of destroing: %d\n", fm.destroy());
    // fm.create_file();
    // fm.remove();
    // fm.change_dir();
    // fm.get_cur_dir(dir_now);
    // fm.list();
    return 0;
}

// static модификатор доступа(общий доступ к переменной во всей дирректории)

/*
char str[] = "Geeks for Geeks";
    char* token;
    char* rest = str;

    while ((token = strtok_r(rest, " ", &rest)))
        printf("%s\n", token);
*/