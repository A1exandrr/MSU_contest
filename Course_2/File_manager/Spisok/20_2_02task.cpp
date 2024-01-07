#include "os_file.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=================[  Prototypes of additional functions ]==================
void Create_node(int disk_size);
void Def_initVariables(int disk_size);
void Add_child(const char *path);
// void Garbage_collection();
int Check_operation(char *path);
int valid_path(const char *path);
int valid_name(const char *name);

//==========================================================================

//=================[ Prototypes of main functions ]==================
int ptr_create(int disk_size);
int ptr_destroy();
int ptr_create_dir(const char *path);
// int ptr_create_file(const char *path, int file_size);
// int ptr_remove(const char *path, int recursive);
// int ptr_change_dir(const char *path);
// void ptr_get_cur_dir(char *dst);
// int ptr_list(const char *path, int dir_first);
//=================================================================

//=================[ Prototypes of functions  ]==================
void setup_file_manager(file_manager_t *fm)
{
    fm->create = ptr_create;
    fm->destroy = ptr_destroy;
    fm->create_dir = ptr_create_dir;
    // fm->create_file = ptr_create_file;
    // fm->remove = ptr_remove;
    // fm->change_dir = ptr_change_dir;
    // fm->get_cur_dir = ptr_get_cur_dir;
    // fm->list = ptr_list;
}
//========================================================

//=================[ Struct for work with manager nodes  ]=================
typedef struct node_manager
{
    int c_children, disk_space;    // Reconsider if you need these
                                   //   for maintenance effort versus benefit
    struct node_manager *children; // point to children of this node
    struct node_manager *next;     // point to next node at same level
    struct node_manager *previous; // point to previos node at same level
    struct node_files *file;
    char *cur_path;
    char *child;
} node_manager;
node_manager *root = NULL;
node_manager var_m;
//========================================================

//=================[ Struct for work with files and path ]=================
typedef struct node_files
{
    char *name[32];
    char *absolute_path[128];
    char *relative_path;
} node_files;
//========================================================

//========================================================

//=================[ Realization of main functions ]=================
int ptr_create(int disk_size)
{
    if (disk_size > 0)
    {
        // Добавить проверку на корректное выделение памяти
        Create_node(disk_size);
        return 1;
    }
    else
    {
        perror("Incorrect disk size");
        return 0;
    }
}

int ptr_create_dir(const char *path)
{
    int flag = 0;
    char *rest = strdup(path);
    char *token = NULL;

    while ((token = strtok_r(rest, "/", &rest)))
        if (valid_name(token) && valid_path(rest, token))
            flag = 1;
        else
            return 0;
    var_m.disk_space--;

    fprintf(stdout, "Disk space after creating new folder: %d\n", var_m.disk_space);
    return 2;
}

int ptr_destroy()
{
    if (root)
    {
        // Incorrect memory clearing. Need to fix it
        //  free(root->file->relative_path);
        free(root->children->previous);
        free(root->children->next);
        free(root->children);
        // free(root->file->cur_path);
        free(root->file);
        free(root);
        var_m.c_children = 0;
        var_m.disk_space = 0;
        return 1;
    }
    return 0;
}
//===================================================================

//=================[ Realization of additional functions ]=================

void Create_node(int disk_size)
{
    root = (node_manager *)malloc(sizeof(node_manager));
    root->file = (node_files *)malloc(sizeof(node_files));
    root->children = (node_manager *)malloc(sizeof(node_manager));
    root->children->next = (node_manager *)malloc(sizeof(node_manager));
    root->children->previous = (node_manager *)malloc(sizeof(node_manager));
    if (root)
    {
        Def_initVariables(disk_size);
    }
    else
        perror("Error with memory");
}

void Def_initVariables(int disk_size)
{
    var_m.c_children = 0;

    // node_m->file->absolute_path = strdup("/");
    // node_m->file->relative_path = node_m->file->absolute_path;
    // node_m->file->cur_path = node_m->file->relative_path;

    root->children[var_m.c_children].next = NULL;
    root->children[var_m.c_children].previous = NULL;
    root->children[var_m.c_children].cur_path = strdup("/");

    var_m.disk_space = disk_size;

    fprintf(stdout, "%d\n", var_m.disk_space);
}

int Check_operation(char *path, )
{
}

void Add_child(const char *path)
{
}

node_manager *new_node(char name)
{
    node_manager *new_node = (node_manager *)malloc(sizeof(node_manager));

    if (new_node)
    {
        new_node->children[0].next = NULL;
        new_node->children[0].previous = root->children.new_node->data = data;
    }

    return new_node;
}

int valid_path(const char *path, const char *name)
{
    if (path[0] == '/')
    {
        for (; root != NULL; root = root->children->next)
        {                         // scan the siblings' list
            if (root->child == name) // test the current node
                return 1;      // return it if the value found

            if (root->child != NULL)
            { // scan a subtree
                valid_path(root->child->, key);
                if (result)        // the value found in a subtree
                    return result; // abandon scanning, return the node found
            }
        }
        return 0; // key not found
    }
}

int valid_name(const char *name)
{
    if ((strlen(name) > VALUE_MAX_LENGTH) || !strcmp(name, ".") || !strcmp(name, "..") || (name[0] == '.') ||
        !strcmp(name, "") || !name)
        return 0;

    char bad_chars[] = "!#$%&\'()*+,-/:;<=>?@[\\]^`{|}~";

    for (int i = 0; i < strlen(bad_chars); i++)
        if (strchr(name, bad_chars[i]) != NULL)
            return 0;

    return 1;
}

//=========================================================================

/*

int count = 0;
    var_m.c_children++;
    char *rest = strdup(path);
    char *token = strtok(rest, "/");
    // Test function. Need to rewrite
    if (path[0] == '/')
    {
        while (token != NULL)
        {
            count++;
            fprintf(stdout, "%s\n", token);
            token = strtok(NULL, "/");
        }
    }
    else
    {
        while (token != NULL)
        {
            count++;
            root->children->child[0] = (char *)malloc(sizeof(char) * strlen(token) + 1);
            root->children[0].child[0] = strdup(token);
            root.token = strtok(NULL, "/");
        }
    }

    fprintf(stdout, "%d\n", count);
    free(rest);*/
