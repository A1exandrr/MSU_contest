#include "20_2_02task.cpp"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
    file_manager_t fm;
    setup_file_manager(&fm);
    fm.create(100);
    fm.create_dir("/Main");
    fm.create_dir("/Main/test1");
    // fm.destroy();
    // fm.create_dir();
    // fm.create_file();
    // fm.remove();
    // fm.change_dir();
    // fm.get_cur_dir();
    // fm.list();
    return 0;
}
