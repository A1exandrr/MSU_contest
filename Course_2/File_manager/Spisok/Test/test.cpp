#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NestedNode
{
    int data;
    struct NestedNode *prev;
    struct NestedNode *next;
    char *absolutePath; // Абсолютный путь
    char *relativePath; // Относительный путь
} NestedNode;

typedef struct MainNode
{
    int data;
    struct MainNode *prev;
    struct MainNode *next;
    NestedNode *nestedList;
    char *absolutePath; // Абсолютный путь
    char *relativePath; // Относительный путь
} MainNode;

NestedNode *createNestedNode(int data, const char *absolutePath, const char *relativePath)
{
    NestedNode *newNode = (NestedNode *)malloc(sizeof(NestedNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Ошибка выделения памяти для вложенного узла\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->absolutePath = strdup(absolutePath); // Копирование абсолютного пути
    newNode->relativePath = strdup(relativePath); // Копирование относительного пути
    return newNode;
}

MainNode *createMainNode(int data, const char *absolutePath, const char *relativePath)
{
    MainNode *newNode = (MainNode *)malloc(sizeof(MainNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Ошибка выделения памяти для основного узла\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->nestedList = NULL;
    newNode->absolutePath = strdup(absolutePath); // Копирование абсолютного пути
    newNode->relativePath = strdup(relativePath); // Копирование относительного пути
    return newNode;
}

void addToNestedList(NestedNode **head, int data, const char *absolutePath, const char *relativePath)
{
    NestedNode *newNode = createNestedNode(data, absolutePath, relativePath);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
}

void addToMainList(MainNode **head, int data, const char *absolutePath, const char *relativePath)
{
    MainNode *newNode = createMainNode(data, absolutePath, relativePath);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
}

void printNestedList(NestedNode *head)
{
    NestedNode *current = head;
    while (current != NULL)
    {
        printf("%d - Absolute Path: %s, Relative Path: %s\n", current->data, current->absolutePath,
               current->relativePath);
        current = current->next;
    }
    printf("\n");
}

void printMainList(MainNode *head)
{
    MainNode *current = head;
    while (current != NULL)
    {
        printf("Main Node: %d - Absolute Path: %s, Relative Path: %s\n", current->data, current->absolutePath,
               current->relativePath);
        printf("Associated Nested List: ");
        printNestedList(current->nestedList);
        printf("\n");
        current = current->next;
    }
}

int main()
{
    MainNode *mainList = NULL;

    addToMainList(&mainList, 1, "/abs_path_1", "rel_path_1");
    addToMainList(&mainList, 2, "/abs_path_2", "rel_path_2");
    addToMainList(&mainList, 3, "/abs_path_3", "rel_path_3");

    addToNestedList(&(mainList->nestedList), 11, "/abs_path_11", "rel_path_11");
    addToNestedList(&(mainList->nestedList), 12, "/abs_path_12", "rel_path_12");
    addToNestedList(&(mainList->next->nestedList), 21, "/abs_path_21", "rel_path_21");
    addToNestedList(&(mainList->next->nestedList), 22, "/abs_path_22", "rel_path_22");
    addToNestedList(&(mainList->next->next->nestedList), 31, "/abs_path_31", "rel_path_31");

    printMainList(mainList);

    return 0;
}
