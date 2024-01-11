#include "os_file.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=================[ Prototypes of main functions ]==================
int ptr_create(int disk_size);
// int ptr_destroy();
// int ptr_create_dir(const char *path);
// int ptr_create_file(const char *path, int file_size);
// int ptr_remove(const char *path, int recursive);
// int ptr_change_dir(const char *path);
// void ptr_get_cur_dir(char *dst);
// int ptr_list(const char *path, int dir_first);
//===================================================================

//=================[ Prototypes of functions  ]==================
void setup_file_manager(file_manager_t *fm)
{
    fm->create = ptr_create;
    // fm->destroy = ptr_destroy;
    // fm->create_dir = ptr_create_dir;
    // fm->create_file = ptr_create_file;
    // fm->remove = ptr_remove;
    // fm->change_dir = ptr_change_dir;
    // fm->get_cur_dir = ptr_get_cur_dir;
    // fm->list = ptr_list;
}
//================================================================

// Node structure

typedef struct Manager_n
{
    char *name;
    Manager_n *next;
    Manager_n *prev;
    Item_n *item_list;
    int disk_space;
    char *absolute_path;
    char *relative_path;
} Manager_n;
Manager_n *node_m = NULL;

typedef struct Item_n
{
    char *name;
    Item_n *next;
    Item_n *prev;
} Item_n;

//=================[ Struct for work with files and path ]=================

int ptr_create(int disk_size)
{
    if (!node_m)
    {
        /* code */
    }
    
}