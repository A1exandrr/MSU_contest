#include "os_file.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DISK_SPACE = 0;

//=================[ Prototypes of main functions ]==================
int ptr_create(int disk_size);
// int ptr_destroy();
int ptr_create_dir(const char *path);
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
    fm->create_dir = ptr_create_dir;
    // fm->create_file = ptr_create_file;
    // fm->remove = ptr_remove;
    // fm->change_dir = ptr_change_dir;
    // fm->get_cur_dir = ptr_get_cur_dir;
    // fm->list = ptr_list;
}
//================================================================

// Node structure

typedef struct NestedNode
{
    char *name;
    char isDir;
    struct NestedNode *prev;
    struct NestedNode *next;
    char *absolutePath;
    char *relativePath;
    char *curDir;
} NestedNode;

typedef struct MainNode
{
    char *name;
    struct MainNode *prev;
    struct MainNode *next;
    NestedNode *nestedList;
    char *absolutePath;
    char *relativePath;
} MainNode;

typedef struct List
{
    NestedNode *head;
    NestedNode *tail;
} List;

//=================[ Struct for work with files and path ]=================

MainNode *manager_n = NULL;
//=========================Prototypes of additional functions
MainNode *createMainNode();
//===================================

int ptr_create(int disk_size)
{
    if (!manager_n && disk_size > 0)
    {
        manager_n = (MainNode *)malloc(sizeof(MainNode *));
        if (manager_n)
        {
            DISK_SPACE = disk_size;
            manager_n = createMainNode();
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

int ptr_create_dir(const char *path)
{
    
    if (path && manager_n)
    {
        if (path[0] == '/')
    }
    return 0;
}

//===============================Additional Functions

MainNode *createMainNode()
{
    MainNode *newNode = (MainNode *)malloc(sizeof(MainNode));
    if (newNode)
    {
        newNode->name = strdup("/");
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->nestedList = NULL;
        newNode->absolutePath = strdup("/");
        newNode->relativePath = strdup("/");
        return newNode;
    }
    return NULL;
}
