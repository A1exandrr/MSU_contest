#include "os_mem.h"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// заливка двух файлов
// реализация через двусвязный список. Кривая реализация через массивы с изменением памяти

int main()
{
    memory_manager_t mm;
    setup_memory_manager(&mm);
    mm.create(10, 10); // максимальное количество 10 объектов
    // создание тестовых объектов
    mm.create_object("wuw");
    mm.create_object("b");
    mm.create_object("a");
    mm.create_object("wuwww");
    mm.create_object("c");
    //============
    mm.set_root("b");
    mm.link("b", "a");
    mm.link("b", "c");
    mm.link("a", "wuw");
    mm.link("wuw", "c");
    mm.destroy_object("c");
    mm.destroy();
    mm.print_objects();
    mm.collect_live_objects();
    mm.destroy();
    return 0;
}
