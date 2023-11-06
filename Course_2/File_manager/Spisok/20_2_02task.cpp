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

Files_tree *node = NULL;
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
int CheckDir();
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
        node = (Files_tree *)malloc(sizeof(Files_tree));
        if (node != NULL)
        {
            node->size = disk_size;
            node->absolute_path = strdup("/");
            node->cur_dir = strdup(node->absolute_path);
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
    if (strcmp(node->cur_dir, node->absolute_path) == 0)
    {
       node->new_dir = strdup(path);
       
    }

    // node->new_dir = strdup(path);
    // node->new_dir = strdup(strcat(node->new_dir, "/"));
    // node->cur_dir = strdup(strcat(node->cur_dir, node->new_dir));
    return 1;
}

void ptr_get_cur_dir(char *dst)
{
    dst = strdup(node->cur_dir);
    fprintf(stdout, "%s\n", dst);
}

int CheckDir()
{
}

void C_DoubleList()
{
}