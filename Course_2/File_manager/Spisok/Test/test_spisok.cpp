#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int value;        // значение элементов списка
    struct Node *ptr; // указатель на элементы списка
} Spisok; // Spisok является название структруры. По нему мы и ссылаемся

Spisok *create(int data)
{
    // выделения памяти под корень списка
    Spisok *tmp = (Spisok *)malloc(sizeof(Spisok));
    tmp->value = data; // инициализация значения
    tmp->ptr = NULL;   // указатель на следующий элемент
    return tmp;
}

Spisok *add_element_start(int data, Spisok *head)
{
    // выделения памяти под узел списка
    Spisok *tmp = (Spisok *)malloc(sizeof(Spisok));
    tmp->value = data;
    tmp->ptr = head;
    return tmp;
}

void add_element_end(int data, Spisok *head){

}
int main(int argc, char const *argv[])
{

    return 0;
}
