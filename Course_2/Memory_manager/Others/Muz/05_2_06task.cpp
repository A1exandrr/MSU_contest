#include "os_mem.h"
#include <iostream>

int ost_obj = 0;
int objs = 0;
char **name_obj = NULL;
char **save = NULL;
char **save1 = NULL;
int **index_obj = NULL;
int *del_index = NULL;
int *count = NULL;
int *root = NULL;
int *visit = NULL;

int create(int size, int num_pages)
{
    if (size <= 0 || objs > 0)
        return 0;

    else
    {
        objs = size;
        ost_obj = size;

        name_obj = (char **)calloc(sizeof(char *), size);
        for (int i = 0; i < size; i++)
        {
            name_obj[i] = (char *)calloc(sizeof(char), 33);
        }

        index_obj = (int **)calloc(sizeof(int *), size);
        for (int i = 0; i < size; i++)
        {
            index_obj[i] = (int *)calloc(sizeof(int), 1);
        }

        del_index = (int *)calloc(sizeof(int), size);

        count = (int *)calloc(sizeof(int), size);

        root = (int *)calloc(sizeof(int), size);

        visit = (int *)calloc(sizeof(int), size);

        return 1;
    }
}

int create_object(const char *name)
{
    for (int i = 0; i < objs; i++)
    {
        if (strcmp(name_obj[i], name) == 0)
            return 0;
    }

    for (int i = 0; i < objs; i++)
    {
        if (del_index[i] == 0)
        {

            del_index[i] = 1;
            ost_obj--;
            strcpy(name_obj[i], name);
            return 1;
        }
    }
    return 0;
}

int link(const char *object1_name, const char *object2_name)
{
    int parent = 0, child = 0, y, flag1 = 0, flag2 = 0;
    if (strcmp(object1_name, object2_name) != 0)
    {

        for (int i = 0; i < objs; i++)
        {
            if (strcmp(name_obj[i], object1_name) == 0)
            {
                parent = i;
                flag1 = 1;
            }
            else if (strcmp(name_obj[i], object2_name) == 0)
            {
                child = i;
                flag2 = 1;
            }
        }
        if (flag1 && flag2)
        {

            y = count[parent];

            index_obj[parent] = (int *)realloc(index_obj[parent], sizeof(int) * (y + 1));
            index_obj[parent][y] = child;
            y = count[parent]++;

            return 1;
        }
        else
            return 0;
    }
    else
        return 0;
}

int destroy_object(const char *name)
{
    if (objs > ost_obj)
    {
        for (int i = 0; i < objs; i++)
        {
            if (strcmp(name_obj[i], name) == 0)
            {
                for (int j = 0; j < count[i]; j++)
                {
                    int f = index_obj[i][j];
                    visit[f] = false;
                }
                visit[i] = false;
                root[i] = 0;
                count[i] = 0;

                free(index_obj[i]);
                index_obj[i] = NULL;

                del_index[i] = 0;
                ost_obj++;
                for (int j = 0; j < 33; j++)
                {
                    name_obj[i][j] = '\0';
                }
                return 1;
            }
        }
        return 0;
    }
    else
        return 0;
}

void print_objects()
{
    if (objs > ost_obj)
    {

        int m = 0;
        save = (char **)calloc(sizeof(char *), (objs - ost_obj));
        for (int i = 0; i < (objs - ost_obj); i++)
        {
            save[i] = (char *)calloc(sizeof(char), 33);
        }

        for (int i = 0; i < objs; i++)
        {
            if (del_index[i] == 1)
            {
                strcpy(save[m], name_obj[i]);
                m++;
            }
        }

        for (int j = 0; j < m - 1; j++)
        {
            for (int i = 0; i < m - 1; i++)
            {
                if (strcmp(save[i], save[i + 1]) > 0)
                {
                    char *t = save[i];
                    save[i] = save[i + 1];
                    save[i + 1] = t;
                }
            }
        }

        for (int i = 0; i < m; i++)
        {
            printf("%s ", save[i]);
        }

        for (int i = 0; i < objs - ost_obj; i++)
        {
            free(save[i]);
        }
        free(save);
    }
}

int set_root(const char *name)
{
    if (objs > ost_obj)
    {
        for (int i = 0; i < objs; i++)
        {
            if (strcmp(name_obj[i], name) == 0)
            {
                root[i] = 1;
                return 1;
            }
        }
        return 0;
    }
    else
        return 0;
}

void dfs(int v)
{
    visit[v] = true;

    for (int i = 0; i < count[v]; i++)
    {
        int u = index_obj[v][i];
        if (!visit[u])
            dfs(u);
    }
}

void collect_live_objects(void)
{
    for (int i = 0; i < objs; i++)
    {
        if (root[i])
        {
            dfs(i);
        }
    }

    int count = 0;
    for (int i = 0; i < objs - ost_obj; i++)
    {
        if (visit[i])
        {
            count++;
        }
    }

    save1 = (char **)calloc(sizeof(char *), count);
    for (int i = 0; i < count; i++)
    {
        save1[i] = (char *)calloc(sizeof(char), 33);
    }

    int r = 0;
    for (int i = 0; i < objs - ost_obj; i++)
    {
        if (visit[i])
        {
            strcpy(save1[r], name_obj[i]);
            r++;
        }
    }

    for (int j = 0; j < count - 1; j++)
    {
        for (int i = 0; i < count - 1; i++)
        {
            if (strcmp(save1[i], save1[i + 1]) > 0)
            {
                char *t = save1[i];
                save1[i] = save1[i + 1];
                save1[i + 1] = t;
            }
        }
    }

    for (int i = 0; i < count; i++)
    {
        printf("%s ", save1[i]);
    }

    if (count)
    {
        for (int i = 0; i < count; i++)
        {
            free(save1[i]);
        }
        free(save1);
    }
}

int destroy()
{

    if (objs <= 0)
    {
        return 0;
    }
    else
    {

        for (int i = 0; i < objs; i++)
        {
            free(name_obj[i]);
        }
        free(name_obj);

        for (int i = 0; i < objs; i++)
        {
            if (index_obj[i])
                free(index_obj[i]);
        }
        free(index_obj);

        free(del_index);

        free(count);

        free(root);

        free(visit);

        objs = 0;
        ost_obj = 0;

        return 1;
    }
}

void setup_memory_manager(memory_manager_t *mm)
{
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->destroy_object = destroy_object;
    mm->print_objects = print_objects;
    mm->link = link;
    mm->set_root = set_root;
    mm->collect_live_objects = collect_live_objects;
}