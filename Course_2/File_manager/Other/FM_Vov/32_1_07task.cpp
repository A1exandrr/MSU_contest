#include "os_file.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

typedef struct FileNode
{
    short int is_dir;
    short int is_pended;
    struct FileNode *left_neigh;
    struct FileNode *right_neigh;
    struct FileNode *next_subdir;
    struct FileNode *parent_dir;
    unsigned long long int size;
    char *obj_name;
    char *absolute_path;
} FileNode;

FileNode *DISK = NULL, *cur_dir = NULL, *loop_node = NULL,
         *dpt_fl = NULL; // main disk + tmp attributes
int fm_create(int disk_size);
int fm_destroy();
int fm_create_dir(const char *path);
int fm_create_file(const char *path, int file_size);
int fm_remove(const char *path, int recursive);
int fm_change_dir(const char *path);
int fm_move(const char *old_path, const char *new_path);
int locate_obj(FileNode *new_obj, int mode);
void correct_path(FileNode **node1);
void prnt(FileNode **tst1);
void fm_get_cur_dir(char *dst);
void destroy_fmnode(FileNode **obj1);
void next_token(char **pre_prev_token, char **prev_token, char **token);
void init_fmnode(FileNode *fmnode, short int is_dir, unsigned long long int size, FileNode *left_neigh,
                 FileNode *right_neigh, FileNode *next_subdir, FileNode *parent_dir, char **obj_name,
                 char **absolute_path);

void setup_file_manager(file_manager_t *fm)
{
    fm->create = fm_create;
    fm->destroy = fm_destroy;
    fm->get_cur_dir = fm_get_cur_dir;
    fm->create_dir = fm_create_dir;
    fm->create_file = fm_create_file;
    fm->remove = fm_remove;
    fm->change_dir = fm_change_dir;
    fm->move = fm_move;
}

void node_shift(int direction, int (*condition)(FileNode *))
{
    while (loop_node && ((direction) ? (loop_node->left_neigh != NULL) : (loop_node->right_neigh != NULL)) &&
           condition(loop_node))
    {
        loop_node = ((direction) ? (loop_node->left_neigh) : (loop_node->right_neigh));
    }
}

void init_fmnode(FileNode *fmnode, short int is_dir, unsigned long long int size, FileNode *left_neigh,
                 FileNode *right_neigh, FileNode *next_subdir, FileNode *parent_dir, char **obj_name,
                 char **absolute_path)
{
    fmnode->is_dir = is_dir;
    fmnode->size = size;
    fmnode->is_pended = 0;
    fmnode->left_neigh = left_neigh;
    fmnode->right_neigh = right_neigh;
    fmnode->next_subdir = next_subdir;
    fmnode->parent_dir = parent_dir;
    fmnode->absolute_path = *absolute_path;
    fmnode->obj_name = *obj_name;
    return;
}

int check_disk(FileNode *tmp)
{
    return ((tmp == NULL) ? (1) : (0));
}

int __sc(FileNode *node)
{
    return 1;
} // condition for skip

int is_valid_name(char *obj_name)
{
    if ((strlen(obj_name) > 32) || !strcmp(obj_name, ".") || !strcmp(obj_name, "..") || (obj_name[0] == '.') ||
        !strcmp(obj_name, ""))
    {
        return 0;
    }
    int result = 1;
    char bad_chars[] = "!#$%&\'()*+,-/:;<=>?@[\\]^`{|}~";
    for (int i = 0; i < strlen(bad_chars); i++)
    {
        if (strchr(obj_name, bad_chars[i]) != NULL)
        {
            result = 0;
            break;
        }
    }

    return result;
}

void next_token(char **pre_prev_token, char **prev_token, char **token)
{
    *pre_prev_token = *prev_token;
    *prev_token = *token;
    *token = strtok(NULL, "/");
}
//!!!
int path_handler(const char *path, char **name, int *mode)
{
    char *input = strdup(path);
    int _ = 0,
        l = strlen(path);                                 // "_" for clearing begining and ending fromspaces
    int result = 0, tmp_res = 1, par_flag = 0, st_fl = 0; // rec not to change
    while ((input[_] == ' ') && (_ < l))
    {
        _++;
    }
    memmove(input, input + _, l - _ + 1);
    _ = (l - _ - 1);
    while ((input[_] == ' ') && (_ > 0))
    {
        _--;
    }
    char *tmp = strdup(input);
    memmove(tmp, input, _ + 1); // rec not to change
    free(input);
    input = strdup(tmp);
    free(tmp);
    char *token = strtok(input, "/");
    char *prev_token = NULL, *pre_prev_token = NULL;
    if (input[0] != '/')
    {
        loop_node = cur_dir;
        prev_token = cur_dir->obj_name;
        pre_prev_token = cur_dir->parent_dir->obj_name;
    }
    else
    {
        loop_node = DISK;
        pre_prev_token = (char *)"";
        prev_token = (char *)"";
    }
    if ((loop_node->next_subdir != NULL))
    {
        loop_node = loop_node->next_subdir;
    }
    node_shift(1, __sc);
    // printf("%s|%s|%s|%s|%d|%d|end\n", loop_node->obj_name, token,
    //        prev_token, pre_prev_token, result, *mode);
    while ((token != NULL) && tmp_res)
    {
        par_flag = 0; //
        st_fl = 0;
        if (!strcmp(token, "."))
        {
            token = strtok(NULL, "/");
            st_fl = 1;
        }
        else if (!strcmp(token, ".."))
        { // edit
            loop_node = loop_node->parent_dir;
            if ((loop_node == DISK) && DISK->next_subdir)
            {
                loop_node = DISK->next_subdir; //
            }
            node_shift(1, __sc);
            pre_prev_token = loop_node->parent_dir->parent_dir->obj_name; // :)) -_-
            prev_token = loop_node->parent_dir->obj_name;
            token = strtok(NULL, "/"); //
            par_flag = 1;
        }
        else
        {
            while ((loop_node->right_neigh != NULL) && loop_node->is_dir && strcmp(loop_node->obj_name, token))
            {
                loop_node = loop_node->right_neigh;
                dpt_fl = ((dpt_fl == loop_node) ? (NULL) : (dpt_fl));
            }
            dpt_fl = ((dpt_fl == loop_node) ? (NULL) : (dpt_fl));
            if (strcmp(loop_node->obj_name, token))
            {
                tmp_res = 0;
                if (prev_token && !strcmp(loop_node->obj_name, prev_token) && (loop_node->next_subdir != NULL))
                {
                    loop_node = loop_node->next_subdir;
                } //
            }
            else if (!strcmp(loop_node->obj_name, token))
            {
                if (loop_node->next_subdir != NULL)
                {
                    loop_node = loop_node->next_subdir;
                }
                else
                {
                    next_token(&pre_prev_token, &prev_token, &token);
                    tmp_res = 0;
                }
            }
            else
            {
                tmp_res = 0;
            }
        }
        next_token(&pre_prev_token, &prev_token, &token);
    }
    if (loop_node->left_neigh && pre_prev_token && !strcmp(loop_node->left_neigh->obj_name, pre_prev_token))
    {
        loop_node = loop_node->left_neigh;
    }
    if (prev_token != NULL)
    {
        *name = strdup(prev_token);
    }
    else if (pre_prev_token != NULL)
    {
        *name = strdup(pre_prev_token);
    }
    if ((strchr(path, '/') == NULL) && !par_flag && pre_prev_token && strcmp(loop_node->obj_name, pre_prev_token))
    {
        loop_node = loop_node->parent_dir;
        dpt_fl = ((dpt_fl == loop_node) ? (NULL) : (dpt_fl));
    }

    if (token == NULL)
    {
        if (pre_prev_token == NULL)
        {
            pre_prev_token = (char *)"";
        }
        if (!strcmp(loop_node->obj_name, pre_prev_token) && prev_token && !st_fl)
        {
            result = 1;
            *mode = 1;
        }
        else if (!strcmp(loop_node->parent_dir->obj_name, pre_prev_token) && prev_token &&
                 strcmp(loop_node->obj_name, prev_token))
        {
            result = 1;
            *mode = -1;
        }
        else
        {
            result = 1;
            *mode = 0;
        }
    }
    if ((*mode > 0) && result && (loop_node->next_subdir != NULL))
    {
        dpt_fl = ((dpt_fl == loop_node) ? (NULL) : (dpt_fl));
        loop_node = loop_node->next_subdir;
        *mode = -1;
    }
    free(input);
    return result;
}

int _ob_is_dir(FileNode *node)
{
    return node->is_dir;
}

int check_node_existance(char *node_name, short int node_type)
{ // 1 - exisst 0 - not
    while ((loop_node->right_neigh != NULL) && (loop_node->is_dir == node_type) &&
           (strcmp(node_name, loop_node->obj_name) > 0))
    {
        loop_node = loop_node->right_neigh;
    }
    return ((strcmp(loop_node->obj_name, node_name)) ? (0) : (1));
}

int locate_obj(FileNode *new_obj, int mode)
{ // finds new location for obj
    if (mode < 0)
    {
        node_shift(1, __sc);
        if (!(new_obj->is_dir))
        {
            node_shift(0, _ob_is_dir);
        }
        if (check_node_existance(new_obj->obj_name, new_obj->is_dir))
        {
            return 0;
        }
        if (((new_obj->is_dir > loop_node->is_dir) || strcmp(new_obj->obj_name, loop_node->obj_name) < 0) &&
            (loop_node->left_neigh == NULL))
        {
            loop_node->left_neigh = new_obj;
            new_obj->right_neigh = loop_node;
            loop_node->parent_dir->next_subdir = new_obj;
            new_obj->parent_dir = loop_node->parent_dir;
        }
        else if (loop_node->right_neigh != NULL)
        {
            new_obj->left_neigh = loop_node;
            new_obj->right_neigh = loop_node->right_neigh;
            new_obj->parent_dir = loop_node->parent_dir;
            loop_node->right_neigh->left_neigh = new_obj;
            loop_node->right_neigh = new_obj;
        }
        else
        {
            loop_node->right_neigh = new_obj;
            new_obj->left_neigh = loop_node;
            new_obj->parent_dir = loop_node->parent_dir;
        }
    }
    else if ((mode > 0) && (loop_node->is_dir > 0) && (loop_node->next_subdir == NULL))
    {
        loop_node->next_subdir = new_obj;
        new_obj->parent_dir = loop_node;
    }
    else
    {
        return 0;
    }
    return 1;
}

void create_new_path(char *base_path, char *base_name, char **new_path)
{
    const size_t len1 = strlen(base_path);
    const size_t len3 = strlen(base_name);
    const size_t len2 = strlen("/");
    *new_path = (char *)malloc(len1 + len2 + len3 + 1);
    memcpy(*new_path, base_path, len1);
    memcpy(*new_path + len1, base_name, len3);
    memcpy(*new_path + len1 + len3, "/", len2 + 1);
}

int create_obj(const char *path, int object_size, short int isdir)
{

    char *new_obj_name = NULL;
    int mode = 0;
    int result = (path_handler(path, &new_obj_name, &mode) && mode);

    if (result)
    {
        if ((!isdir && ((int)(DISK->size - object_size) < 0)) || !is_valid_name(new_obj_name))
        {
            free(new_obj_name);
            return 0;
        }
        else
        {
            (DISK->size) -= object_size;
        }

        FileNode *new_node = (FileNode *)malloc(sizeof(FileNode)); //

        char *base_absolute_path =
            strdup((mode != 1) ? (loop_node->parent_dir->absolute_path) : (loop_node->absolute_path));
        char *base_prev_name = strdup((mode != 1) ? (loop_node->parent_dir->obj_name) : (loop_node->obj_name));
        char *new_absolute_path;

        create_new_path(base_absolute_path, base_prev_name, &new_absolute_path);

        free(base_absolute_path);
        free(base_prev_name);

        init_fmnode(new_node, isdir, object_size, NULL, NULL, NULL, NULL, &new_obj_name, &new_absolute_path);
        result = (locate_obj(new_node, mode) && ((strlen(new_node->absolute_path) + strlen(new_node->obj_name)) < 128));
        if (!result)
            destroy_fmnode(&new_node);
    }
    else
    {
        free(new_obj_name);
    }
    return result;
}

int fm_create(int disk_size)
{ // t+
    if (disk_size < 0 || !check_disk(DISK))
        return 0;
    DISK = (FileNode *)malloc(sizeof(FileNode));
    if (DISK == NULL)
        return 0;
    char *t1 = strdup(""), *t2 = strdup("");
    init_fmnode(DISK, 1, disk_size, NULL, NULL, NULL, DISK, &t1, &t2);
    cur_dir = DISK;
    return 1;
}

void clear_obj(FileNode **obj1)
{
    FileNode *obj = *obj1;
    if ((obj->left_neigh == NULL) && (obj->right_neigh != NULL))
    {
        obj->right_neigh->left_neigh = NULL;
        obj->parent_dir->next_subdir = obj->right_neigh;
    }
    else if ((obj->left_neigh != NULL) && (obj->right_neigh != NULL))
    {
        obj->left_neigh->right_neigh = obj->right_neigh;
        obj->right_neigh->left_neigh = obj->left_neigh;
    }
    else if ((obj->left_neigh != NULL) && (obj->right_neigh == NULL))
    {
        obj->left_neigh->right_neigh = NULL;
    }
    else if (obj->parent_dir)
    {
        obj->parent_dir->next_subdir = NULL;
    }
    obj->left_neigh = NULL;
    obj->right_neigh = NULL;
    obj->parent_dir = NULL;
}

void destroy_fmnode(FileNode **obj1)
{ // t+
    FileNode *obj = *obj1;
    if ((obj == NULL) || check_disk(DISK))
        return;
    if (!obj->is_dir)
        DISK->size += obj->size;
    clear_obj(obj1);
    if (obj->absolute_path)
        free(obj->absolute_path);
    if (obj->obj_name)
        free(obj->obj_name);
    if (obj)
        free(obj);
    return;
}

void dirs_pending(FileNode **start_node, int mode, void (*func)(FileNode **))
{ // t+
    // dirs pending with castom function
    // (function can take only 1 arg - inputnode)
    if (check_disk(DISK))
        return;
    FileNode *tmp = (*start_node)->next_subdir;
    while (tmp && tmp != *start_node)
    {
        while (tmp && (!tmp->is_pended) && (tmp->next_subdir != NULL))
        {
            if (!(tmp->is_pended) && !mode)
                func(&tmp);
            tmp->is_pended = 1;
            tmp = tmp->next_subdir;
        }
        if (!(tmp->is_pended) && !mode)
            func(&tmp);
        tmp->is_pended = 1;
        while (tmp && (tmp->right_neigh != NULL) && (tmp->next_subdir != NULL))
        {
            if (!(tmp->is_pended) && !mode)
                func(&tmp);
            tmp->is_pended = 1;
            tmp = tmp->right_neigh;
        }

        if (tmp && tmp->right_neigh == NULL)
        {
            if (!(tmp->is_pended) && (tmp->next_subdir != NULL))
                continue;
            if (!(tmp->is_pended) && !mode)
            {
                func(&tmp);
                tmp->is_pended = 1;
            }
            while (tmp && (tmp->left_neigh != NULL))
            { // edit
                tmp->is_pended = 0;
                tmp = tmp->left_neigh;
                if (tmp && mode)
                    func(&(tmp->right_neigh));
            }
            tmp->is_pended = 0;
            tmp = tmp->parent_dir;
            if (tmp && mode)
                func(&(tmp->next_subdir));
        }
        else if (tmp && (tmp->right_neigh != NULL) && tmp->is_pended)
        {
            tmp = tmp->right_neigh;
        }
    }
    if (mode)
        func(start_node);
    return;
}

int fm_change_dir(const char *path)
{ // t+
    if (check_disk(DISK))
        return 0;
    char *_;
    int mode = 0, result = (path_handler(path, &_, &mode) && !mode);
    if (result)
        cur_dir = loop_node;
    free(_);
    return result;
}

int fm_destroy()
{ // t+
    if (check_disk(DISK))
        return 0;
    dirs_pending(&DISK, 1, destroy_fmnode);
    DISK = NULL;
    return 1;
}

void fm_get_cur_dir(char *dst)
{
    if (!check_disk(DISK))
    {
        char *base_path = cur_dir->absolute_path;
        char *base_name = cur_dir->obj_name;
        char *new_path;
        create_new_path(base_path, base_name, &new_path);
        strcpy(dst, new_path);
        free(new_path);
    }
    return;
}

int fm_create_dir(const char *path)
{ //
    return ((check_disk(DISK)) ? (0) : (create_obj(path, 0, 1)));
}

int fm_create_file(const char *path, int file_size)
{ //
    if (check_disk(DISK) || (file_size < 1) || ((int)(DISK->size - file_size) < 0))
    {
        return 0;
    }
    return create_obj(path, file_size, 0);
}

void prnt(FileNode **tst1)
{
    FileNode *tst = *tst1;
    printf("%s  %s\n", tst->obj_name, tst->absolute_path);
}

int fm_remove(const char *path, int recursive)
{
    if (check_disk(DISK))
        return 0;
    char *tmp = NULL;
    int mode = -1, result = (path_handler(path, &tmp, &mode) && (mode <= 0));
    if (result)
    {
        result = 0;
        if (((loop_node->next_subdir == NULL) ? (0) : (1)) == recursive)
        {
            if (cur_dir == loop_node)
            {
                cur_dir = DISK;
                loop_node = DISK;
            }
            (recursive) ? (dirs_pending(&loop_node, recursive, destroy_fmnode)) : (destroy_fmnode(&loop_node));
            result = 1;
        }
    }
    if (tmp)
        free(tmp);
    return result;
}

void correct_path(FileNode **node1)
{
    FileNode *node = *node1;
    if (node->absolute_path)
        free(node->absolute_path);
    if (node->left_neigh == NULL)
    {
        char *base_absolute_path = node->parent_dir->absolute_path;
        char *base_prev_name = node->parent_dir->obj_name;
        char *new_absolute_path;
        create_new_path(base_absolute_path, base_prev_name, &new_absolute_path);
        node->absolute_path = new_absolute_path;
    }
    else
    {
        node->absolute_path = strdup(node->left_neigh->absolute_path);
    }
    return;
}

int fm_move(const char *old_path, const char *new_path)
{ // acceptable realization
    char *tmp, *tmp1;
    int mode, result = path_handler(old_path, &tmp, &mode);
    FileNode *old_node = NULL, *new_dest = NULL;
    if (result && (mode <= 0))
    {
        old_node = loop_node;
        if (mode < 0)
        { // case with dir and file
            node_shift(1, __sc);
            while ((loop_node->right_neigh != NULL) && strcmp(tmp, loop_node->obj_name))
            {
                loop_node = loop_node->right_neigh;
            }
            old_node = loop_node;
            result = ((strcmp(loop_node->obj_name, tmp)) ? (0) : (1));
        }
        int new_mode = 0;
        int new_res = (path_handler(new_path, &tmp1, &new_mode) &&
                       ((strlen(loop_node->absolute_path) + strlen(loop_node->obj_name)) < 128));
        if (result & new_res && new_mode)
        {
            new_dest = loop_node;
            dpt_fl = ((old_node->next_subdir != NULL) ? (old_node->next_subdir) : (NULL));
            char *corr_path;
            create_new_path(loop_node->absolute_path, loop_node->obj_name, &corr_path);
            char *new_name = strdup(tmp1);
            free(tmp1);
            int flag = ((dpt_fl == NULL) ? (1) : (0));
            path_handler(corr_path, &tmp1, &new_mode);
            if ((!flag && (dpt_fl != NULL)) || (flag && (dpt_fl == NULL)))
            {
                new_mode = -1;
                if (new_dest->left_neigh != NULL)
                {
                    new_dest = new_dest->left_neigh;
                }
                else if (new_dest->right_neigh != NULL)
                {
                    new_dest = new_dest->right_neigh;
                }
                loop_node = new_dest;
                clear_obj(&old_node);
                free(old_node->obj_name);
                old_node->obj_name = new_name;
                locate_obj(old_node, new_mode);
                correct_path(&old_node);
                dirs_pending(&old_node, 0, correct_path);
                result = 1;
            }
            else
            {
                result = 0;
                free(new_name);
            }
            free(corr_path);
        }
    }
    if (tmp)
        free(tmp);
    if (tmp1)
        free(tmp1);
    return result;
}

// sample strtok implemetation (for questions about built-in strtok)
// unsigned int is_delim(char c, char *delim) {
//   while (*delim != '\0') {
//     if (c == *delim)
//       return 1;
//     delim++;
//   }
//   return 0;
// }
// char *my_strtok(char *srcString, char *delim) {
//   static char *backup_string;
//   if (!srcString) {
//     srcString = backup_string;
//   }
//   if (!srcString) {
//     return NULL;
//   }
//   while (1) {
//     if (is_delim(*srcString, delim)) {
//       srcString++;
//       continue;
//     }
//     if (*srcString == '\0') {
//       return NULL;
//     }
//     break;
//   }
//   char *ret = srcString;
//   while (1) {
//     if (*srcString == '\0') {
//       backup_string = srcString;
//       return ret;
//     }
//     if (is_delim(*srcString, delim)) {
//       *srcString = '\0';
//       backup_string = srcString + 1;
//       return ret;
//     }
//     srcString++;
//   }
// }
