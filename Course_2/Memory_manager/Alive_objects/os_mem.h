#ifndef _OS_MEM_H
#define _OS_MEM_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mem_handle_t
{
    // адрес размещения блока относительно начала всей памяти (в байтах)
    int addr;
    // размер блока в байтах
    int size;

    mem_handle_t(int addr, int size) : addr(addr), size(size)
    {
    }

    bool operator==(const mem_handle_t &other)
    {
        return other.addr == addr && other.size == size;
    }
    bool operator!=(const mem_handle_t &other)
    {
        return !operator==(other);
    }
}; // дескриптор блока памяти

// Структура менеджера памяти
// угловые скобочки в комментариях к функциях означают, что берется значение соответствующей переменной, так, выдача на
// экран load <page_num> означает, что нужно выдать слово load и номер страницы page_num, например load 5 (без угловых
// скобочек).
typedef struct
{
    // создание менеджера памяти,
    // size: для менеджеров памяти - размер всей памяти в байтах, для garbage collection - максимальное количество
    // объектов, для страничных менеджеров - размер одной страницы памяти num_pages: для страничных менеджеров -
    // количество физических страниц возвращаемое значение: 1 - успех, 0 - неудача
    int (*create)(int size, int num_pages);

    // удаление менеджера памяти
    // возвращаемое значение: 1 - успех, 0 - неудача (например, менеждер не был создан)
    int (*destroy)();

    //
    // выделение блока памяти размером block_size байт
    // для менеджеров памяти с фиксированным размером блока памяти аргумент функции игнорируется.
    // возвращаемое значение: дескриптор выделенного блока - успех, нулевой дескриптор {0,0} - неудача
    mem_handle_t (*alloc)(int block_size);

    // освобождение блока памяти с дескриптором h
    // возвращаемое значение: 1 - успех, 0 - неудача (например, неверный дескриптор)
    // стековый менеджер должен игнорировать параметр h и всегда освобождать последний выделенный блок
    int (*free)(mem_handle_t h);

    // возвращает дескриптор выделенного блока памяти, целиком содержащего указанный регион
    // в случае, если такого блока не существует, возвращается нулевой дескриптор {0,0}
    mem_handle_t (*get_block)(int addr, int size);

    // возвращаемое значение: максимальный размер блока памяти (в байтах), который может быть сейчас выделен
    int (*get_max_block_size)();

    // возвращаемое значение: суммарный объем свободной памяти в байтах
    int (*get_free_space)();

    // распечатывает все выделенные блоки памяти в формате <аddr> <size>
    // порядок блоков - по возрастанию адресов.
    void (*print_blocks)();

    //////////////////////////////////////////////////////////
    // Garbage collection
    //////////////////////////////////////////////////////////

    // создание объекта
    // длина имени объекта ограничена 32 символами
    // возвращаемое значение: 1 - успех, 0 - неудача (например, если объект с таким именем уже существует)
    int (*create_object)(const char *name);

    // уничтожение объекта
    // при этом удаляются все исходящие из объекта ссылки
    // объект задается своим именем
    // возвращаемое значение: 1 - успех, 0 - неудача (например, если такого объекта не существует)
    int (*destroy_object)(const char *name);

    // печатает имена всех созданных и не уничтоженных объектов в алфавитном порядке
    void (*print_objects)();

    // делает объект с заданным именем корневым
    // допускается существование множества корневых объектов
    // возвращаемое значение: 1 - успех, 0 - неудача (например, если такого объекта не существует)
    int (*set_root)(const char *name);

    // создает ссылку из объекта object1 на объект object2
    // количество ссылок из каждого объекта ограничено числом 16
    // возвращаемое значение: 1 - успех, 0 - неудача (например, если одного из объектов не существует)
    int (*link)(const char *object1_name, const char *object2_name);

    // вычисляет и печатает множество живых объектов.
    // живыми считаются объекты типа root (корневые), а также все объекты, доступные из других
    // живых объектов по ссылкам
    // вывод объектов осуществляется в алфавитном порядке
    void (*collect_live_objects)(void);

    // вычисляет и печатает множество мертвых ссылок в объектах, т.е. ссылок, указывающих на
    // удаленные объекты. Для этого ведется учет всех объектов, созданных и удаленных
    // с помощью create_object и destroy_object
    // формат вывода ссылки: <object1_name>,<ref_num>
    // вывод осуществляется после упорядочивания объектов в алфавитном порядке, а потом по номеру ссылки по возрастанию
    void (*collect_dead_links)(void);

    // распечатывает для каждого объекта количество ссылок на него
    // считаются только ссылки, ведущие из не удаленных объектов
    // формат вывода: <object_name> <link_count>
    // вывод объектов осуществляется в алфавитном порядке
    void (*print_link_counts)();

    //////////////////////////////////////////////////////////
    // Paging
    //////////////////////////////////////////////////////////

    // записывает страницу физической памяти real_page в файл с именем page_<virtual_page>
    // данные записываются в бинарном формате
    // в случае успеха выводит на экран сообщение: save_page <real_page> <virtual_page>
    // возвращаемое значение: 1 - успех, 0 - неудача (например, файл не открывается)
    int (*save_page)(int real_page, int virtual_page);

    // считывает из файла с именем page_<virtual_page> страницу памяти и загружает ее в физическую страницу real_page.
    // данные считываются в бинарном формате
    // если файл с таким именем отсутствует, физическая страница заполняется нулями.
    // в случае успеха выводит на экран сообщение: load_page <virtual_page> <real_page>
    // возвращаемое значение: 1 - успех (отсутствующий файл также является успехом), 0 - неудача (например номер
    // страницы меньше нуля или больше общего числа страниц)
    int (*load_page)(int virtual_page, int real_page);

    // записывает по указанному виртуальному адресу addr указанное значение (байт) val
    // в случае, если соответствующая виртуальная страница отсутствует в памяти, данные не записываются
    // возвращаемое значение: 1 - успех, 2 - виртуальная страница отсутствует, 0 - неудача (например, адрес меньше 0)
    int (*set_byte)(int addr, char val);

    // считывает значение (байт) из ячейки виртуальной памяти, расположенной по указанному адресу addr, и кладет в буфер
    // dst возвращаемое значение: 1 - успех, 2 - виртуальная страница отсутствует, 0 - неудача (например, адрес меньше
    // 0)
    int (*get_byte)(int addr, char *dst);

    // возвращает номер виртуальной страницы памяти, содержащей указанный блок
    // возвращаемое значение: номер страницы в случае успеха, -1 в случае неудачи (если указанный блок целиком не
    // содержится ни в одной странице)
    int (*page_num)(mem_handle_t block);

    // инициирует обращение к памяти типа чтение
    // в случае успеха метод выводит на экран сообщение в формате: read <addr> <size> <ppa>
    // где ppa - номер физической страницы, к которой происходит обращение
    // сами данные копировать не требуется
    // возвращаемое значение: 1 - успех, -1 - неудача (если указанный регион не лежит внутри одной страницы)
    int (*read)(int addr, int size);

    // инициирует обращение к памяти типа запись
    // в случае успеха метод выводит на экран сообщение в формате: write <addr> <size> <ppa>
    // где ppa - номер физической страницы, к которой происходит обращение
    // сами данные копировать не требуется
    // возвращаемое значение: 1 - успех, -1 - неудача (если указанный регион не лежит внутри одной страницы)
    int (*write)(int addr, int size);

} memory_manager_t;

void setup_memory_manager(memory_manager_t *mm);

#endif
