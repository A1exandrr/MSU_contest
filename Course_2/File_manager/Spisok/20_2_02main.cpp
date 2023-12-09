#include "20_2_02task.cpp"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
    char *dir_now = strdup("");
    file_manager_t fm;
    setup_file_manager(&fm);
    fm.create(100);
    fm.create_dir("Main");
    fm.create_dir("/Main/test1");
    fm.create_dir("/Main/test1/test2");
    fm.change_dir("/Main/test1/test2/..");
    // fm.destroy();
    // fm.create_file();
    // fm.remove();
    // fm.change_dir();
    fm.get_cur_dir(dir_now);
    // fm.list();
    return 0;
}
