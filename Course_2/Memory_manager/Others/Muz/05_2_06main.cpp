#include <iostream>
#include "os_mem.h"

int main() {

    memory_manager_t mm;
    setup_memory_manager(&mm);


    /*mm.create(3, 0);

    mm.create_object("c");
    mm.create_object("a");
    mm.create_object("b");
    //mm.create_object("t");

    mm.link("a", "b");
    mm.link("a", "c");
    mm.link("b", "c");
    mm.link("c", "a");
    //mm.link("c", "b");
    //mm.link("t", "c");

    /*mm.link("t", "a");
    mm.link("t", "b");

    mm.destroy_object("a");
    mm.destroy_object("a");

    mm.set_root("a");
    
    mm.collect_live_objects();
    printf("\n");
    
    mm.destroy_object("a");

    mm.collect_live_objects();


    mm.destroy();*/

    printf("%d\n", mm.create(10, 1) != 0);
    printf("%d\n", mm.create_object("o9"));
    printf("%d\n", mm.create_object("o1"));
    printf("%d\n", mm.create_object("o2"));
    printf("%d\n", mm.create_object("o3"));
    printf("%d\n", mm.create_object("o4"));
    printf("%d\n", mm.create_object("o5"));
    printf("%d\n", mm.create_object("o6"));
    printf("%d\n", mm.create_object("o7"));
    printf("%d\n", mm.create_object("o8"));
    printf("%d\n", mm.create_object("o0"));
    printf("%d\n", mm.set_root("1234567"));
    printf("%d\n", mm.set_root("o1"));
    printf("%d\n", mm.set_root("o2"));
    printf("%d\n", mm.set_root("o9"));
    printf("%d\n", mm.link("o1", "1234567"));
    printf("%d\n", mm.link("o1", "o3"));
    printf("%d\n", mm.destroy_object("o1"));
    printf("%d\n", mm.link("o1", "o4"));
    printf("%d\n", mm.link("o2", "o4"));
    printf("%d\n", mm.link("o4", "o2"));
    printf("%d\n", mm.link("o4", "o6"));
    printf("%d\n", mm.link("o5", "o7"));
    printf("%d\n", mm.link("o7", "o1"));
    printf("%d\n", mm.link("o4", "o8"));
    printf("\n\n");
    mm.print_objects();
    printf("\n\n");
    mm.collect_live_objects();
    printf("\n\n");
    printf("%d\n", mm.destroy_object("o4"));
    printf("\n\n");
    mm.collect_live_objects();
    printf("\n\n");
    printf("%d\n", mm.destroy_object("o2"));
    printf("%d\n", mm.destroy_object("o9"));
    printf("\n\n");
    mm.print_objects();
    printf("\n\n");
    mm.collect_live_objects();
    printf("\n\n");
    printf("%d\n", mm.destroy());

    return 0;
}