#include "os_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NODE_VALUE_MAX_LENGTH 32
#define ABSOLUTE_PATH_MAX_LEN 128

struct node *root = NULL;
struct node *cur_dir_node = NULL;
int FREE_SIZE = 0;

struct node
{
    struct node *parent;
    struct LinkedList *child;
    char *nodeValue;
    char *absolutePath;
    int is_dir;
    int nodeSize;
} typedef node;

// Linked list struct
struct ListItem
{
    node *_node;
    struct ListItem *prev;
    struct ListItem *next;
} typedef ListItem;

struct LinkedList
{
    int size;
    ListItem *head;
    ListItem *tail;
} typedef LinkedList;

// implementation of Linked list
LinkedList *createLinkedList()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void pushBack(LinkedList *list, node *node)
{
    ListItem *item = (ListItem *)malloc(sizeof(ListItem));
    item->_node = node;
    item->next = NULL;
    item->prev = list->tail;
    if (list->tail)
    {
        list->tail->next = item;
    }
    list->tail = item;
    if (!list->head)
    {
        list->head = item;
    }
    ++list->size;
}

int deleteNode(node *_node)
{
    if (!_node)
    {
        return 0;
    }
    size_t _size = _node->child->size;
    FREE_SIZE += _node->nodeSize;
    for (size_t i = 0; i < _size; ++i)
    {
        // node *iter = popBack(_node->child);
        ListItem *item = _node->child->tail;
        _node->child->tail = _node->child->tail->prev;
        if (_node->child->tail)
        {
            deleteNode(item->_node);
            free(_node->child->tail->next);
            _node->child->tail->next = NULL;
        }
        // if (item == _node->child->head)
        else
        {
            deleteNode(item->_node);
            free(_node->child->head);
            _node->child->head = NULL;
        }
        --_node->child->size;
        // deleteNode(iter);
    }
    free(_node->absolutePath);
    free(_node->child);
    free(_node->nodeValue);
    _node->absolutePath = NULL;
    _node->child = NULL;
    _node->nodeValue = NULL;
    free(_node);
    _node = NULL;
    return 1;
}

int deleteNthNode(LinkedList *list, node *node)
{
    ListItem *iter = list->head;
    while (iter)
    {
        if (!strcmp(iter->_node->nodeValue, node->nodeValue))
        {
            if (iter->next && iter->prev)
            {
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
            }
            if (!iter->next && !iter->prev)
            {
                deleteNode(iter->_node);
                iter->_node = NULL;
                --list->size;
                free(list->head);
                list->head = NULL;
                list->tail = NULL;
                // list->tail = iter->prev;
                return 1;
            }
            if (!iter->prev)
            {
                deleteNode(iter->_node);
                iter->_node = NULL;
                --list->size;
                // free(list->head);
                list->head = iter->next;
                free(list->head->prev);
                list->head->prev = NULL;
                return 1;
            }
            if (!iter->next)
            {
                deleteNode(iter->_node);
                iter->_node = NULL;
                list->tail = iter->prev;
                --list->size;
                free(list->tail->next);
                list->tail->next = NULL;
                return 1;
            }
            --list->size;
            deleteNode(iter->_node);
            iter->_node = NULL;
            if (list->head && list->tail)
            {
                list->tail->next = NULL;
                list->head->prev = NULL;
            }
            return 1;
        }
        iter = iter->next;
    }
    return 0;
}

void printTree(const LinkedList *list, int flag)
{
    node **arr = (node **)malloc(sizeof(node *) * list->size);
    ListItem *iter = list->head;
    for (int i = 0; i != list->size; ++i)
    {
        arr[i] = iter->_node;
        iter = iter->next;
    }
    node *tmp;
    for (int i = 0; i != list->size; ++i)
    {
        for (int j = 0; j != list->size - 1; ++j)
        {
            if (strcmp(arr[j]->nodeValue, arr[j + 1]->nodeValue) == 1)
            {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    if (!flag)
    {
        for (int i = 0; i != list->size; ++i)
        {
            printf("%s\n", arr[i]->nodeValue);
        }
        free(arr);
        arr = NULL;
    }
    else
    {
        for (int i = 0; i != list->size; ++i)
        {
            if (arr[i]->is_dir)
            {
                printf("%s\n", arr[i]->nodeValue);
            }
        }
        for (int i = 0; i != list->size; ++i)
        {
            if (!arr[i]->is_dir)
            {
                printf("%s\n", arr[i]->nodeValue);
            }
        }
        free(arr);
        arr = NULL;
    }
}

node *is_contains(LinkedList *list, char *nodeValue)
{
    ListItem *item = list->head;
    while (item)
    {
        if (!strcmp(item->_node->nodeValue, nodeValue))
        {
            return item->_node;
        }
        item = item->next;
    }
    return NULL;
}

int create(int disk_size);
int destroy();
int create_dir(const char *path);
int create_file(const char *path, int file_size);
int remove(const char *path, int recursive);
int change_dir(const char *path);
void get_cur_dir(char *dst);
int list(const char *path, int dir_first);
void setup_file_manager(file_manager_t *fm);

int is_valid_path(char *path);
int createObject(const char *path, int size, int isDir);
int count(char *path);
char *get_abs_path(char *lhs, char *rhs);
char **parser(char *path, int size);
int set_dir(char *path);
int checkPath(const char *path);
int getIthNode(node **node, char **token, char *_path, int size);
int deleteObject(char *path, int recursive);

void setup_file_manager(file_manager_t *fm)
{
    fm->create = create;
    fm->destroy = destroy;
    fm->create_dir = create_dir;
    fm->create_file = create_file;
    fm->remove = remove;
    fm->change_dir = change_dir;
    fm->get_cur_dir = get_cur_dir;
    fm->list = list;
}

int is_valid_path(char *path)
{
    char arr[] = "!/\\,{}[]<>@#$%^&*()+|'\"?~`*+=";
    if (!path || strlen(path) > NODE_VALUE_MAX_LENGTH || !strcmp(path, "") ||
        (*path == '.' && path[strlen(path) - 1] == '.'))
    {
        return 0;
    }
    for (size_t i = 0; i != strlen(arr); ++i)
    {
        if (strchr(path, arr[i]))
        {
            return 0;
        }
    }
    return 1;
}

int count(char *path)
{
    int count = 0;
    for (size_t i = 0; i != strlen(path); ++i)
    {
        if (path[i] == '/')
        {
            ++count;
        }
    }
    return count;
}

char **parser(char *path, int size)
{
    char **lines = (char **)malloc(sizeof(char *) * size);
    lines[0] = strtok(path, "/");
    for (int i = 1; i < size; ++i)
    {
        lines[i] = strtok(NULL, "/");
    }
    return lines;
}

int set_dir(char *path)
{
    int flag = 0;
    int _size = (*path == '/') ? count(path) : count(path) + 1;
    char **token = parser(path, _size);
    node *tmp = cur_dir_node;
    if (getIthNode(&tmp, token, path, _size + 1))
    {
        cur_dir_node = tmp;
        flag = 1;
    }
    free(path);
    free(token);
    path = NULL;
    token = NULL;
    return flag;
}

int deleteObject(char *_path, int recursive)
{
    int flag = 0;
    int size = (*_path == '/') ? count(_path) : count(_path) + 1;
    char **token = parser(_path, size);
    node *iter = cur_dir_node;
    if (getIthNode(&iter, token, _path, size + 1) && !(iter->child->size && !recursive))
    {
        deleteNthNode(iter->parent->child, iter);
        flag = 1;
    }
    free(_path);
    free(token);
    _path = NULL;
    iter = NULL;
    token = NULL;
    return flag;
}

int checkPath(const char *path)
{
    int flag = (!root || !path) ? 0 : 1;
    return flag;
}

char *get_abs_path(char *lhs, char *rhs)
{
    int size = strlen(rhs) + strlen(lhs) + 2;
    char *abs_path = (char *)malloc(size);
    strcpy(abs_path, lhs);
    if (*(lhs + strlen(lhs) - 1) != '/')
    {
        strcat(abs_path, "/");
    }
    strcat(abs_path, rhs);
    return abs_path;
}

int getIthNode(node **iter, char **token, char *_path, int _size)
{
    if (*_path == '/')
    {
        *iter = root;
    }

    for (int i = 0; i != _size - 1; ++i)
    {
        if (!strcmp(".", token[i]))
        {
            continue;
        }
        else if (!strcmp("..", token[i]))
        {
            if (!(*iter)->parent)
            {
                return 0;
            }
            *iter = (*iter)->parent;
        }
        else
        {
            *iter = is_contains((*iter)->child, token[i]);
            if (!(*iter) || !is_valid_path(token[i]))
            {
                return 0;
            }
        }
    }
    return 1;
}

int createObject(const char *path, int size, int isDir)
{

    char *_path = strdup(path);
    int flag = 0;
    int _size = (*path == '/') ? count(_path) : count(_path) + 1;
    char **token = parser(_path, _size);
    node *iter = cur_dir_node;
    if (getIthNode(&iter, token, _path, _size) && is_valid_path(token[_size - 1]) &&
        !is_contains(iter->child, token[_size - 1]) && iter->is_dir)
    {
        node *tmp = (node *)malloc(sizeof(node));
        tmp->child = createLinkedList();
        tmp->is_dir = isDir;
        tmp->parent = iter;
        tmp->nodeValue = strdup(token[_size - 1]);
        tmp->absolutePath = get_abs_path(iter->absolutePath, token[_size - 1]);
        tmp->nodeSize = size;
        pushBack(iter->child, tmp);
        FREE_SIZE -= size;
        flag = 1;
    }
    free(_path);
    free(token);
    _path = NULL;
    token = NULL;
    return flag;
}

// implementation of main functions
int create(int disk_size)
{
    if (disk_size > 0 && !root) // root = null
    {                           // !root (Если рут не равен null)
        root = (node *)malloc(sizeof(node));
        root->child = createLinkedList();
        root->is_dir = 1;
        root->parent = NULL;
        root->absolutePath = strdup("/");
        root->nodeValue = strdup("/");
        root->nodeSize = disk_size;
        cur_dir_node = root;
        FREE_SIZE = disk_size;
        return 1;
    }
    return 0;
}

int destroy()
{
    if (root)
    {
        deleteNode(root);
        root = NULL;
        cur_dir_node = NULL;
        return 1;
    }
    return 0;
}

int create_dir(const char *path)
{
    return checkPath(path) ? createObject(path, 0, 1) : 0;
}

int create_file(const char *path, int file_size)
{
    return (checkPath(path) && (FREE_SIZE - file_size >= 0)) ? createObject(path, file_size, 0) : 0;
}

int remove(const char *path, int recursive)
{
    return checkPath(path) ? deleteObject(strdup(path), recursive) : 0;
}

int change_dir(const char *path)
{
    char *_path = strdup(path);
    return (checkPath(path) || cur_dir_node) ? set_dir(_path) : 0;
}

void get_cur_dir(char *dst)
{
    if (cur_dir_node->absolutePath)
    {
        strcpy(dst, cur_dir_node->absolutePath);
    }
}

int list(const char *path, int dir_first)
{
    if (!strcmp(path, "/"))
    {
        printTree(root->child, dir_first);
        return 1;
    }
    char *_path = strdup(path);
    int flag = 0;
    int _size = (*path == '/') ? count(_path) : count(_path) + 1;
    char **token = parser(_path, _size);
    node *iter = cur_dir_node;
    if (getIthNode(&iter, token, _path, _size + 1))
    {
        flag = 1;
        if (!iter->is_dir)
        {
            printf("%s\n", path);
        }
        else
        {
            printf("%s:\n", path);
            printTree(iter->child, dir_first);
        }
    }
    free(_path);
    free(token);
    _path = NULL;
    token = NULL;
    return flag;
}