#include "os_mem.h"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int mm_create(int size, int num_pages);
int mm_destroy();
int mm_create_object(const char *name);
int mm_destroy_object(const char *name);
int mm_set_root(const char *name);
int mm_link(const char *object1_name, const char *object2_name);
int check_obj_existance(const char *name);
int check_link_existance(link_node *obj, const char *name);
void mm_print_objects();
void mm_collect_live_objects();
void locate_obj(ObjNode *loc_node, ObjNode *node);

void setup_memory_manager(memory_manager_t *mm)
{
    mm->create = mm_create;
    mm->destroy = mm_destroy;
    mm->create_object = mm_create_object;
    mm->destroy_object = mm_destroy_object;
    mm->print_objects = mm_print_objects;
    mm->set_root = mm_set_root;
    mm->link = mm_link;
    mm->collect_live_objects = mm_collect_live_objects;
}
