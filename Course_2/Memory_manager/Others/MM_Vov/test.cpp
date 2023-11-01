#include "os_mem.h"
#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

typedef struct link_node link_node;

typedef struct ObjNode {
  short int is_root;
  short int is_pended;
  struct ObjNode *left_neigh;
  struct ObjNode *right_neigh;
  char *obj_name;
  struct link_node *links;
} ObjNode;

struct link_node {
  struct link_node *next_link;
  struct ObjNode *linked_obj;
  struct link_node *prev_link;
};

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

int OBJECTS_COUNT = 0, INITIAl_OBJECTS_COUNT = 0;
ObjNode *MM_HEAD = NULL, *loop_node = NULL;

void setup_memory_manager(memory_manager_t *mm) {
  mm->create = mm_create;
  mm->destroy = mm_destroy;
  mm->create_object = mm_create_object;
  mm->destroy_object = mm_destroy_object;
  mm->print_objects = mm_print_objects;
  mm->set_root = mm_set_root;
  mm->link = mm_link;
  mm->collect_live_objects = mm_collect_live_objects;
}

int check_obj_existance(const char *name) {
  loop_node = MM_HEAD;
  while (loop_node && (loop_node->right_neigh != NULL) &&
         strcmp(loop_node->obj_name, name)) {
    loop_node = loop_node->right_neigh;
  }
  if ((loop_node == NULL) || loop_node && strcmp(loop_node->obj_name, name))
    return 0;
  return 1;
}

int check_link_existance(link_node *obj, const char *name) {
  while (obj && obj->next_link && strcmp(obj->linked_obj->obj_name, name)) {
    obj = obj->next_link;
  }
  if ((obj == NULL) || obj && strcmp(obj->linked_obj->obj_name, name))
    return 0;
  return 1;
}

int mm_create(int size, int num_pages) {
  if (OBJECTS_COUNT || size < 0)
    return 0;
  OBJECTS_COUNT = size;
  INITIAl_OBJECTS_COUNT = OBJECTS_COUNT;
  return 1;
}

int mm_destroy() {
  if (!OBJECTS_COUNT && (MM_HEAD == NULL))
    return 0;
  ObjNode **tmp = &MM_HEAD;
  loop_node = *tmp;
  while (loop_node && (loop_node->right_neigh != NULL)) {
    link_node *l_tmp = loop_node->links;
    while (l_tmp && (l_tmp->next_link != NULL)) {
      l_tmp = l_tmp->next_link;
      free(l_tmp->prev_link);
    }
    free(l_tmp);
    loop_node = loop_node->right_neigh;
    free(MM_HEAD->obj_name);
    free(MM_HEAD);
    MM_HEAD = loop_node;
  }
  if (loop_node && loop_node->obj_name)
    free(loop_node->obj_name);
  if (loop_node)
    free(loop_node);
  OBJECTS_COUNT = 0;
  INITIAl_OBJECTS_COUNT = 0;
  MM_HEAD = NULL;
  return 1;
}

void locate_obj(ObjNode *loc_node, ObjNode *node) {
  if ((loc_node == NULL) || (loc_node->left_neigh == NULL)) {
    node->right_neigh = MM_HEAD;
    node->left_neigh = NULL;
    if (MM_HEAD != NULL)
      MM_HEAD->left_neigh = node;
    MM_HEAD = node;
  } else {
    node->right_neigh = loc_node->right_neigh;
    loc_node->right_neigh = node;
    node->left_neigh = loc_node;
    if (node->right_neigh != NULL)
      node->right_neigh->left_neigh = node;
  }
}

int mm_create_object(const char *name) { // +-
  if (!OBJECTS_COUNT || check_obj_existance(name))
    return 0;
  loop_node == MM_HEAD;
  char *tmp = strdup(name);
  struct ObjNode *newNode = (struct ObjNode *)malloc(sizeof(struct ObjNode));
  newNode->is_root = 0;
  newNode->obj_name = tmp;
  newNode->links = NULL;
  locate_obj(loop_node, newNode);
  OBJECTS_COUNT--;
  return 1;
}

int mm_destroy_object(const char *name) {
  if (!OBJECTS_COUNT && (MM_HEAD == NULL) || !check_obj_existance(name))
    return 0;
  if (MM_HEAD == loop_node)
    MM_HEAD = loop_node->right_neigh;
  if (loop_node->right_neigh != NULL)
    loop_node->right_neigh->left_neigh = loop_node->left_neigh;
  if (loop_node->left_neigh != NULL)
    loop_node->left_neigh->right_neigh = loop_node->right_neigh;
  link_node *l_tmp = loop_node->links;
  while (l_tmp && (l_tmp->next_link != NULL)) {
    l_tmp = l_tmp->next_link;
    free(l_tmp->prev_link);
  }
  free(l_tmp);
  ObjNode *tmplnk = MM_HEAD;
  while (tmplnk) {
    l_tmp = tmplnk->links;
    while (l_tmp && l_tmp->next_link &&
           strcmp(l_tmp->linked_obj->obj_name, name)) {
      l_tmp = l_tmp->next_link;
    }
    if (l_tmp && !strcmp(l_tmp->linked_obj->obj_name, name)) {
      if (l_tmp->prev_link == NULL) {
        tmplnk->links = l_tmp->next_link;
      } else {
        l_tmp->prev_link->next_link = l_tmp->next_link;
        if (l_tmp->next_link)
          l_tmp->next_link->prev_link = l_tmp->prev_link;
      }
      free(l_tmp);
    }
    tmplnk = tmplnk->right_neigh;
  }
  free(loop_node->obj_name);
  free(loop_node);
  OBJECTS_COUNT++;
  return 1;
}

int mm_set_root(const char *name) {
  if (!OBJECTS_COUNT && (MM_HEAD == NULL) || !check_obj_existance(name))
    return 0;
  loop_node->is_root = 1;
  return 1;
}

int mm_link(const char *object1_name, const char *object2_name) {
  if (!OBJECTS_COUNT && (MM_HEAD == NULL) || !check_obj_existance(object1_name))
    return 0;
  ObjNode *tmp = loop_node;
  if (!check_obj_existance(object2_name) ||
      check_link_existance(tmp->links, loop_node->obj_name)) {
    return 0;
  }
  struct link_node *new_link =
      (struct link_node *)malloc(sizeof(struct link_node));
  new_link->linked_obj = loop_node;
  new_link->prev_link = NULL;
  new_link->next_link = NULL;
  if (tmp->links == NULL) {
    ObjNode **_ = &tmp;
    (*_)->links = new_link;
  } else {
    link_node *l_tmp = tmp->links;
    while (l_tmp && l_tmp->next_link) {
      l_tmp = l_tmp->next_link;
    }
    l_tmp->next_link = new_link;
    new_link->prev_link = l_tmp;
  }
  return 1;
}

void mm_print_objects() { //+
  if (!OBJECTS_COUNT && (MM_HEAD == NULL))
    return;
  loop_node = MM_HEAD;
  int buff_size = INITIAl_OBJECTS_COUNT - OBJECTS_COUNT;
  struct ObjNode **collector =
      (struct ObjNode **)malloc(buff_size * sizeof(ObjNode *));
  for (int i = 0; i < buff_size; i++) {
    collector[i] = NULL;
    if (loop_node) {
      collector[i] = loop_node;
      loop_node = loop_node->right_neigh;
    }
  }
  // bubble sort - bad!!! better use ready c++ qsort!
  for (int step = 0; step < buff_size - 1; ++step) {
    for (int _i = 0; _i < buff_size - 1; ++_i) {
      if (strcmp(collector[_i]->obj_name, collector[_i + 1]->obj_name) > 0) {
        ObjNode *temp = collector[_i];
        collector[_i] = collector[_i + 1];
        collector[_i + 1] = temp;
      }
    }
  }
  //
  for (int j1 = 0; j1 < buff_size; j1++) {
    if (collector[j1])
      printf("%s\n", collector[j1]->obj_name);
  }
  free(collector);
  return;
}

void mm_collect_live_objects() {
  if (!OBJECTS_COUNT && (MM_HEAD == NULL))
    return;
  loop_node = MM_HEAD;
  int buff_size = INITIAl_OBJECTS_COUNT - OBJECTS_COUNT;
  struct ObjNode **collector =
      (struct ObjNode **)malloc(buff_size * sizeof(ObjNode *));
  int i = 0;
  for (int _i = 0; _i < buff_size; _i++) {
    collector[_i] = NULL;
    if (loop_node && loop_node->is_root) {
      collector[_i] = loop_node;
      loop_node = loop_node->right_neigh;
      i++;
    }
  }
  int j = 0;
  while ((j < buff_size) && collector[j]) {
    link_node *l_tmp = collector[j]->links;
    while (l_tmp) {
      int k = 0;
      while ((k < i) &&
             strcmp(collector[k]->obj_name, l_tmp->linked_obj->obj_name)) {
        k++;
      }
      if (k == i) {
        collector[i] = l_tmp->linked_obj;
        i++;
      }
      l_tmp = l_tmp->next_link;
    }
    j++;
  }
  // bubble sort - bad!!! better use ready c++ qsort!
  for (int step = 0; step < i - 1; ++step) {
    for (int _i = 0; _i < i - 1; ++_i) {
      if (strcmp(collector[_i]->obj_name, collector[_i + 1]->obj_name) > 0) {
        ObjNode *temp = collector[_i];
        collector[_i] = collector[_i + 1];
        collector[_i + 1] = temp;
      }
    }
  }
  //
  for (int j1 = 0; j1 < i; j1++) {
    if (collector[j1])
      printf("%s\n", collector[j1]->obj_name);
  }
  free(collector);
  return;
}
int main() {
  memory_manager_t mm;
  setup_memory_manager(&mm);
  mm.create(10, 10); //максимальное количество 10 объектов
  //создание тестовых объектов
  mm.create_object("wuw");
  mm.create_object("b");
  mm.create_object("a");
  mm.create_object("wuwww");
  mm.create_object("c");
  //============
  mm.set_root("b");
  mm.link("b", "a");
  mm.link("b", "c");
  mm.link("a", "wuw");
  mm.link("wuw", "c");
  mm.destroy_object("c");
  mm.destroy();
  mm.print_objects();
  mm.collect_live_objects();
  mm.destroy();
  return 0;
}
