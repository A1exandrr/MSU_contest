#include "os_file.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
typedef struct Node Node;
typedef struct LinkedList LinkedList;
typedef struct PathInfo PathInfo;

LinkedList *createLinkedList();
PathInfo *createPathInfo(const char *absolutePath, const char *relativePath);
void deleteNode(Node *node);
void freePathInfo(PathInfo *pathInfo);
void freeLinkedList(LinkedList *list);
void freeNode(Node *node);
Node *popFront(LinkedList *list);
int is_valid_path(char *path);
char *get_last_component(const char *path);
Node *find_child_by_name(Node *parent, const char *name);
int is_contains(LinkedList *children, const char *name);
Node *createNode(const char *name, Node *parent, int is_dir);
Node *popFront(LinkedList *list);
void deleteNode(Node *node);
void freePathInfo(PathInfo *pathInfo);
void freeLinkedList(LinkedList *list);
void freeNode(Node *node);
char *concatPaths(const char *path1, const char *path2);
Node *createNode(const char *name, Node *parent, int is_dir);
void pushBack(LinkedList *list, Node *node);
//=================[ Prototypes of main functions ]==================
int ptr_create(int disk_size);
int ptr_destroy();
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
    fm->destroy = ptr_destroy;
    // fm->create_dir = ptr_create_dir;
    // fm->create_file = ptr_create_file;
    // fm->remove = ptr_remove;
    // fm->change_dir = ptr_change_dir;
    // fm->get_cur_dir = ptr_get_cur_dir;
    // fm->list = ptr_list;
}
//================================================================

// Node structure
struct Node
{
    size_t disk_space;
    Node *parent;
    LinkedList *children;
    Node *next;
    PathInfo *pathInfo;
    char *name;
    int is_dir;
};

// LinkedList structure
struct LinkedList
{
    int size;
    Node *head;
    Node *tail;
};

// PathInfo structure
struct PathInfo
{
    char *absolutePath;
    char *relativePath;
};

// Global variables
Node *root = NULL;
Node *cur_dir = NULL;

//=================[ Struct for work with files and path ]=================

int ptr_create(int disk_size)
{
    if (disk_size > 0 && !root)
    {
        root = (Node *)malloc(sizeof(Node));
        if (!root)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        root->children = createLinkedList();
        root->is_dir = 1;
        root->parent = NULL;
        root->pathInfo = createPathInfo("/", ""); // корневая директория
        cur_dir = root;
        root->name = root->pathInfo->absolutePath;
        root->disk_space = disk_size;

        fprintf(stdout, "Root: %s\n", root->name);
        fprintf(stdout, "Relative path: %s\n", root->pathInfo->relativePath);
        fprintf(stdout, "Disk space: %zu\n", root->disk_space);
        return 1;
    }
    return 0;
}

LinkedList *createLinkedList()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    if (!list)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

PathInfo *createPathInfo(const char *absolutePath, const char *relativePath)
{
    PathInfo *pathInfo = (PathInfo *)malloc(sizeof(PathInfo));
    if (!pathInfo)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    pathInfo->absolutePath = strdup(absolutePath);
    pathInfo->relativePath = strdup(relativePath);

    return pathInfo;
}

int ptr_create_dir(const char *path)
{
    if (!root)
    {
        fprintf(stderr, "File manager is not created.\n");
        return 0;
    }

    char *token = strdup(path);

    // Проверяем, что путь является абсолютным или относительным
    if (!is_valid_path(token))
    {
        fprintf(stderr, "Invalid path: %s\n", path);
        return 0;
    }

    // Если необходимо создать корневую директорию
    if (strcmp(path, "/") == 0)
    {
        fprintf(stderr, "Root directory already exists.\n");
        return 0;
    }

    // Проверяем, что директория существует
    char *parent_path = strdup(path);
    char *dir_name = get_last_component(parent_path);

    if (!parent_path || !dir_name)
    {
        free(parent_path);
        fprintf(stderr, "Memory allocation error.\n");
        return 0;
    }

    // Проверяем, что родительская директория существует
    Node *parent;
    if (!navigate_path(path, &parent) || !parent->is_dir)
    {
        fprintf(stderr, "Parent directory does not exist.\n");
        return 0;
    }

    // Проверяем, что директория с указанным именем еще не существует
    if (is_contains(parent->children, dir_name))
    {
        free(parent_path);
        free(dir_name);
        fprintf(stderr, "Directory already exists.\n");
        return 0;
    }

    // Создаем новую директорию
    Node *new_dir = createNode(dir_name, parent, 1); // is_dir = 1, так как создаем директорию
    pushBack(parent->children, new_dir);

    // Очищаем временные строки
    free(parent_path);
    free(dir_name);

    return 1;
}

int is_valid_path(char *path)
{
    char arr[] = "!/\\,{}[]<>@#$%^&*()+|'\"?~`*+=";
    if (!path || strlen(path) > 32 || !strcmp(path, "") || (*path == '.' && path[strlen(path) - 1] == '.'))
        return 0;
    for (size_t i = 0; i != strlen(arr); ++i)
    {
        if (strchr(path, arr[i]))
            return 0;
    }
    return 1;
}

char *get_last_component(const char *path)
{
    char *token = strdup(path);
    if (!path || !is_valid_path(token))
    {
        return NULL;
    }

    size_t path_len = strlen(path);

    if (path_len == 0)
    {
        return NULL;
    }

    // Ищем последний слеш в пути
    const char *last_slash = strrchr(path, '/');

    if (!last_slash) // Если слеш не найден, возвращаем копию всего пути
    {
        return strdup(path);
    }

    // Индекс начала последнего компонента
    size_t start_index = last_slash - path + 1;

    // Выделяем память под последний компонент
    char *last_component = (char *)malloc(path_len - start_index + 1);

    if (!last_component)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Копируем последний компонент в выделенную память
    strcpy(last_component, path + start_index);

    return last_component;
}

int navigate_path(const char *path, Node **result_node)
{
    char *token = strdup(path);
    if (!token || !is_valid_path(token))
    {
        free(token);
        return 0;
    }

    char *last_component = get_last_component(token);
    if (!last_component)
    {
        free(token);
        return 0;
    }

    Node *child = find_child_by_name(cur_dir, last_component);

    // Освобождаем память, выделенную для last_component и token
    free(last_component);
    free(token);

    if (child && result_node)
    {
        *result_node = child;
        return 1;
    }

    return 0;
}

Node *find_child_by_name(Node *parent, const char *name)
{
    if (!parent || !name)
    {
        return NULL;
    }

    Node *current = parent->children->head;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int is_contains(LinkedList *children, const char *name)
{
    if (!children || !name)
    {
        return 0;
    }

    Node *iter = children->head;
    while (iter)
    {
        if (strcmp(iter->name, name) == 0)
        {
            return 1; // Узел с указанным именем найден
        }
        iter = iter->next;
    }

    return 0; // Узел с указанным именем не найден
}

Node *createNode(const char *name, Node *parent, int is_dir)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        perror("Memory allocation error");
        return NULL;
    }

    node->name = strdup(name);
    if (!node->name)
    {
        perror("Memory allocation error");
        free(node);
        return NULL;
    }

    node->is_dir = is_dir;
    node->parent = parent;
    node->children = createLinkedList();
    node->next = NULL;

    // Создание пути для нового узла
    if (parent)
    {
        const char *parent_path = parent->pathInfo->absolutePath;
        const char *relative_path = parent->pathInfo->relativePath;
        node->pathInfo = createPathInfo(concatPaths(parent_path, name), concatPaths(relative_path, name));
    }
    else
    {
        // Для корневого узла
        node->pathInfo = createPathInfo(strdup(name), strdup(name));
    }

    return node;
}

char *concatPaths(const char *path1, const char *path2)
{
    size_t len1 = strlen(path1);
    size_t len2 = strlen(path2);

    // Выделяем память для объединенного пути
    char *result = (char *)malloc(len1 + len2 + 2); // +2 для разделителя и завершающего нуля

    if (!result)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Копируем первую часть пути
    strcpy(result, path1);

    // Добавляем разделитель, если нужно
    if (len1 > 0 && path1[len1 - 1] != '/')
    {
        strcat(result, "/");
    }

    // Добавляем вторую часть пути
    strcat(result, path2);

    return result;
}

void pushBack(LinkedList *list, Node *node)
{
    if (!list || !node)
    {
        return;
    }

    // Если список пуст, новый элемент становится и головой, и хвостом
    if (list->size == 0)
    {
        list->head = node;
        list->tail = node;
        node->next = NULL;
    }
    else
    {
        // Иначе добавляем элемент в конец списка
        list->tail->next = node;
        list->tail = node;
        node->next = NULL;
    }

    // Увеличиваем размер списка
    list->size++;
}

int ptr_destroy()
{
    if (root)
    {
        deleteNode(root); // Удаляем все узлы и подчищаем память
        root = NULL;
        cur_dir = NULL;
        return 1;
    }
    return 0;
}

void deleteNode(Node *node)
{
    if (!node)
    {
        return;
    }

    while (node->children->size > 0)
    {
        Node *child = popFront(node->children);
        deleteNode(child); // Рекурсивно удаляем потомков
    }

    freePathInfo(node->pathInfo);
    freeLinkedList(node->children);
    free(node->name);
    free(node);
}

void freePathInfo(PathInfo *pathInfo)
{
    if (pathInfo)
    {
        free(pathInfo->absolutePath);
        free(pathInfo->relativePath);
        free(pathInfo);
    }
}

void freeLinkedList(LinkedList *list)
{
    Node *current = list->head;
    while (current)
    {
        Node *next = current->next;
        freeNode(current);
        current = next;
    }
    free(list);
}

void freeNode(Node *node)
{
    freePathInfo(node->pathInfo);
    free(node->name);
    free(node);
}

Node *popFront(LinkedList *list)
{
    if (list->size == 0)
    {
        return NULL;
    }

    Node *node = list->head;

    list->head = node->next;
    if (list->head == NULL)
    {
        // Если убрали последний элемент, обновляем tail
        list->tail = NULL;
    }

    list->size--;

    return node;
}