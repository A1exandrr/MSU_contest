#include "os_file.h"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main()
{
    file_manager_t fm;
    setup_file_manager(&fm);
    fm.create(10);
    // fm.destroy();
    // fm.create_dir();
    // fm.create_file();
    // fm.remove();
    // fm.change_dir();
    // fm.get_cur_dir();
    // fm.list();
    return 0;
}
