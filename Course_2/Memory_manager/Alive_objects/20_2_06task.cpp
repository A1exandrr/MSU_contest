#include "os_mem.h"
#include <stdlib.h>
#include <string.h>
#define MAX_LINKS_NUM 16
#define MAX_LENGTH_NAME 32

struct ListItem
{
    char *name;
    ListItem *next;
    ListItem *prev;
} typedef ListItem;

struct LinkedList
{
    int size;
    ListItem *head;
    ListItem *tail;
} typedef LinkedList;

struct Object
{
    char *name;
    int8_t is_free;
    int8_t is_root;
    LinkedList *links;
} typedef Object;

Object **manager_n = NULL;
int manager_size = 0;

LinkedList *createLinkedList();
void deleteLinkedLinst(LinkedList **list);
void pushBack(LinkedList *list, const char *name);
ListItem *popBack(LinkedList *list);

// main functions prototype
int create(int size, int num_pages);
int destroy();
int create_object(const char *name);
int destroy_object(const char *name);
void print_objects();
int set_root(const char *name);
int link(const char *object1_name, const char *object2_name);
void collect_live_objects(void);
void setup_memory_manager(memory_manager_t *mm);

// Helper functions prototype
int8_t is_contains(char *name);
Object **sorting();
int num_objects();
// Implementation of helper functions
int8_t is_contains(char *name)
{
    int flag = 0;
    for (int i = 0; i < manager_size; i++)
    {
        if (!manager_n[i]->is_free && !strcmp(manager_n[i]->name, name))
        {
            flag = 1;
        }
    }
    free(name);
    return flag;
}

// Implementation of main functions
void setup_memory_manager(memory_manager_t *mm)
{
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->destroy_object = destroy_object;
    mm->print_objects = print_objects;
    mm->set_root = set_root;
    mm->link = link;
    mm->collect_live_objects = collect_live_objects;
}

int create(int size, int num_pages)
{
    if (!manager_n && size > 0)
    {
        manager_n = (Object **)malloc(sizeof(Object *) * size);
        manager_size = size;
        for (int i = 0; i != size; ++i)
        {
            manager_n[i] = (Object *)malloc(sizeof(Object));
            manager_n[i]->is_free = 1;
            manager_n[i]->is_root = 0;
            manager_n[i]->name = NULL;
            manager_n[i]->links = NULL;
        }
        return 1;
    }
    return 0;
}

int destroy()
{
    if (manager_n)
    {
        for (int i = 0; i != manager_size; ++i)
        {
            if (!manager_n[i]->is_free)
            {
                deleteLinkedLinst(&manager_n[i]->links);
                free(manager_n[i]->name);
            }
            free(manager_n[i]);
            manager_n[i] = NULL;
        }
        free(manager_n);
        manager_n = NULL;
        return 1;
    }
    return 0;
}

int create_object(const char *name)
{
    if (manager_n && name)
    {
        for (int i = 0; i < manager_size; i++)
        {
            if (manager_n[i]->is_free && !is_contains(strdup(name)) && strlen(name) <= MAX_LENGTH_NAME)
            {
                manager_n[i]->name = strdup(name);
                manager_n[i]->links = createLinkedList();
                manager_n[i]->is_free = 0;
                return 1;
            }
        }
    }
    return 0;
}

int destroy_object(const char *name)
{
    if (manager_n && name)
    {
        for (int i = 0; i < manager_size; i++)
        {
            if (!manager_n[i]->is_free && !strcmp(name, manager_n[i]->name))
            {
                deleteLinkedLinst(&manager_n[i]->links);
                free(manager_n[i]->name);
                manager_n[i]->name = NULL;
                manager_n[i]->is_free = 1;
                return 1;
            }
        }
    }
    return 0;
}

void print_objects()
{
    Object **s_arr = sorting();
    for (int i = 0; i < num_objects(); ++i)
    {
        fprintf(stdout, "%s\n", s_arr[i]->name);
    }
    fprintf(stdout, "\n");
    free(s_arr);
    s_arr = NULL;
}

int set_root(const char *name)
{
    if (manager_n && name)
    {
        for (int i = 0; i < manager_size; i++)
        {
            if (!manager_n[i]->is_free && !strcmp(name, manager_n[i]->name))
            {
                manager_n[i]->is_root = 1;
                return 1;
            }
        }
    }
    return 0;
}

int link(const char *object1_name, const char *object2_name)
{
    if (manager_n && object1_name && object2_name)
    {
        for (int i = 0; i < manager_size; i++)
        {
            if (!manager_n[i]->is_free && !strcmp(object1_name, manager_n[i]->name) &&
                is_contains(strdup(object2_name)))
            {
                pushBack(manager_n[i]->links, object2_name);
                return 1;
            }
        }
    }
    return 0;
}

void collect_live_objects(void)
{
    for (int i = 0; i < manager_size; i++)
    {
        if (!manager_n[i]->is_free && manager_n[i]->is_root)
        {
            fprintf(stdout, "%s\n", manager_n[i]->name);
            ListItem *iter = manager_n[i]->links->head;
            while (iter)
            {
                fprintf(stdout, "%s\n", iter->name);
                iter = iter->next;
            }
        }
    }
    fprintf(stdout, "\n");
}

LinkedList *createLinkedList()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void deleteLinkedLinst(LinkedList **list)
{
    ListItem *head = (*list)->head;
    ListItem *next = NULL;
    while (head)
    {
        next = head->next;
        free(head->name);
        head->name = NULL;
        free(head);
        head = next;
    }
    free(*list);
    *list = NULL;
}

void pushBack(LinkedList *list, const char *name)
{
    ListItem *item = (ListItem *)malloc(sizeof(ListItem));
    item->name = strdup(name);
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

ListItem *popBack(LinkedList *list)
{
    ListItem *item = list->tail;
    list->tail = list->tail->prev;
    if (list->tail)
    {
        list->tail->next = NULL;
    }
    if (item == list->head)
    {
        list->head = NULL;
    }
    return item;
}

Object **sorting()
{
    int count = num_objects();
    Object **arr = (Object **)malloc(sizeof(Object *) * count);
    int j = 0;
    for (int i = 0; i < manager_size; i++)
    {
        if (!manager_n[i]->is_free)
        {
            arr[j] = manager_n[i];
            j++;
        }
    }
    Object *tmp;
    for (int i = 0; i < count; i++)
    {
        for (int k = 1; k < count; k++)
        {
            if (strcmp(arr[k - 1]->name, arr[k]->name) == 1)
            {
                tmp = arr[k - 1];
                arr[k - 1] = arr[k];
                arr[k] = tmp;
            }
        }
    }

    return arr;
}

int num_objects()
{
    int count = 0;
    for (int i = 0; i < manager_size; i++)
    {
        if (!manager_n[i]->is_free)
            count++;
    }
    return count;
}