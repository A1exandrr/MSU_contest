#include "os_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//===========================[ Основные структуры ]===========================
typedef struct
{
    int size;
    char *name;
    char *absolute_path = NULL;
    char *path = NULL;
    char *cur_dir = NULL;
    char *change_dir = NULL;
} Files_tree;

Files_tree *fm = NULL;
Files_tree ptr;
//============================================================================

//=================[ Прототипы основных функций ]=================
int ptr_create(int disk_size);
int ptr_destroy();
int ptr_create_dir(const char *path);
int ptr_create_file(const char *path, int file_size);
int ptr_remove(const char *path, int recursive);
int ptr_change_dir(const char *path);
void ptr_get_cur_dir(char *dst);
int ptr_list(const char *path, int dir_first);
//=================================================================

//=================[ Прототипы дополнительных функций ]=================
// int CheckDir();
//======================================================================

//=================[  Работа с указателями на структуры ]=================
void setup_file_manager(file_manager_t *fm)
{
    fm->create = ptr_create;
    fm->destroy = ptr_destroy;
    fm->create_dir = ptr_create_dir;
    fm->create_file = ptr_create_file;
    fm->remove = ptr_remove;
    fm->change_dir = ptr_change_dir;
    fm->get_cur_dir = ptr_get_cur_dir;
    fm->list = ptr_list;
}
//=========================================================================

int ptr_create(int disk_size)
{
    if (disk_size >= 0)
    {
        fm = (Files_tree *)malloc(sizeof(Files_tree));
        if (fm != NULL)
        {
            ptr.size = disk_size;
            fm->absolute_path = strdup("/");
            fm->cur_dir = fm->absolute_path;
            fprintf(stdout, "%s", fm->cur_dir);
            return 1;
        }
        else
            fprintf(stdout, "Error with malloc for fm\n");
    }
    else
    {
        fprintf(stdout, "Error: Incorrect size\n");
        return 0;
    }
    return 2;
}