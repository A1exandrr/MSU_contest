#ifndef _OS_MEM_H // подключение предцпроцесса. Разобрать после
#define _OS_MEM_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mem_handle_t
{
    int addr;
    int size;
    mem_handle_t(int addr, int size) : addr(addr), size(size)
    {
    } // подключение предцпроцесса. Разобрать после

    bool operator==(const mem_handle_t &other)
    {
        return other.addr == addr && other.size == size;
    }
    bool operator!=(const mem_handle_t &other)
    {
        return !operator==(other);
    }
};

typedef struct
{
    int (*create)(int size, int num_pages);
    int (*destroy)();
    mem_handle_t (*alloc)(int block_size);
    int (*free)(mem_handle_t h);
    mem_handle_t (*get_block)(int addr, int size);
    int (*get_max_block_size)();
    int (*get_free_space)();
    void (*print_blocks)();
    int (*create_object)(const char *name);
    int (*destroy_object)(const char *name);
    void (*print_objects)();
    int (*set_root)(const char *name);
    int (*link)(const char *object1_name, const char *object2_name);
    void (*collect_live_objects)(void);
} memory_manager_t;

void setup_memory_manager(memory_manager_t *mm);

#endif
