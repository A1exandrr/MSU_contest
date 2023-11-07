#include "os_file.h"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//===========================[ Основные структуры ]===========================
typedef struct Files_tree
{
    int size;
    char *name;
    char *absolute_path;
    char *path;
    char *cur_dir;
    char *change_dir;
    char *new_dir;
} Files_tree;

Files_tree *f_tree = NULL;

typedef struct 
//============================================================================

//=================[ Прототипы основных функций ]=================
int ptr_create(int disk_size);
// int ptr_destroy();
int ptr_create_dir(const char *path);
// int ptr_create_file(const char *path, int file_size);
// int ptr_remove(const char *path, int recursive);
// int ptr_change_dir(const char *path);
void ptr_get_cur_dir(char *dst);
// int ptr_list(const char *path, int dir_first);
//=================================================================

//=================[ Прототипы дополнительных функций ]=================
int Valid_name();
void C_DoubleList();
//======================================================================

//=================[  Работа с указателями на структуры ]=================
void setup_file_manager(file_manager_t *fm)
{
    fm->create = ptr_create;
    // fm->destroy = ptr_destroy;
    fm->create_dir = ptr_create_dir;
    // fm->create_file = ptr_create_file;
    // fm->remove = ptr_remove;
    // fm->change_dir = ptr_change_dir;
    fm->get_cur_dir = ptr_get_cur_dir;
    // fm->list = ptr_list;
}
//=========================================================================

int ptr_create(int disk_size)
{
    if (disk_size >= 0)
    {
        f_tree = (Files_tree *)malloc(sizeof(Files_tree));
        if (f_tree != NULL)
        {
            f_tree->size = disk_size;
            f_tree->absolute_path = strdup("/");
            f_tree->cur_dir = strdup(f_tree->absolute_path);
            return 1;
        }
        else
            fprintf(stdout, "Error with malloc for fm\n");
    }
    else
        fprintf(stdout, "Error: Incorrect size\n");
    return 0;
}

int ptr_create_dir(const char *path)
{

    if (strcmp(f_tree->cur_dir, f_tree->absolute_path) == 0)
    {
        f_tree->new_dir = strdup(path);
        f_tree->cur_dir = strdup(strcat(f_tree->cur_dir, f_tree->new_dir));
        return 1;
    }
    else{
        if(s)
    }

    // f_tree->new_dir = strdup(path);
    // f_tree->new_dir = strdup(strcat(f_tree->new_dir, "/"));
    // f_tree->cur_dir = strdup(strcat(f_tree->cur_dir, f_tree->new_dir));
    return 0;
}

void ptr_get_cur_dir(char *dst)
{
    dst = strdup(f_tree->cur_dir);
    fprintf(stdout, "%s\n", dst);
}

void C_DoubleList()
{
}