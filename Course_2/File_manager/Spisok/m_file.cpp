#include <memory.h>
#include <stdlib.h>

typedef struct
{
    int (*create)(int disk_size);
    int (*destroy)();
    int (*create_dir)(const char *path);
    int (*create_file)(const char *path, int file_size);
    int (*remove)(const char *path, int recursive);
    int (*change_dir)(const char *path);
    void (*get_cur_dir)(char *dst);
    // Реализации задачи по контесту
    int (*list)(const char *path, int dir_first);
} file_manager_t;

void setup_file_manager(file_manager_t *fm)
{
    fm->create = fm_create;
    fm->destroy = fm_destroy;
}

int main(int argc, char *argv[])
{
    return return 0;
}
