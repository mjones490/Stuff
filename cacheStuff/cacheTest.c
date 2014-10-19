#include <stdio.h>
#include "cacheStuff.h"

struct my_struct {
    char name[32];
    int  age;
    short a_short_value;
};

void show_my_object(struct my_struct *object)
{
    struct cachePage *page = find_page(object);
    printf("%s : %p, page = %p, cache = %p, age = %d\n", object->name, object,
        page, page->cache_header, object->age);
}

void print_page_stats(struct cachePage *page)
{
    printf("\tPage located at %p:\n", page);
    printf("\t\tFirst object:  %p\n", page->first_obj);
    printf("\t\tObjects:       %d\n", page->nr_objs);
    printf("\t\tAllocated:     %d\n", page->nr_objs_allocated);
}

void print_stats(struct cache *cache)
{
    struct list_head *list, *first;
    
    printf("Cache located at %p:\n", cache);
    printf("\tPages:          %d\n", cache->nr_pages);
    printf("\tMax pages:      %d\n", cache->max_nr_pages);
    printf("\tObject size:    %d\n", cache->obj_size);
    printf("\tObjects:        %d\n", cache->nr_objs);
    printf("\tAllocated:      %d\n", cache->nr_objs_allocated);
    printf("\tMax allocated:  %d\n", cache->max_objs_allocated);

    LIST_FOR_EVERY(list, &cache->first_page->list, first)
        print_page_stats(GET_ELEMENT(struct cachePage, list, list));
}
int main (int argv, char **argc)
{
    struct cache *cache;
    struct my_struct *my_object[15];
    int i;

    printf("Cache Stuff...\n");

    cache = cache_create(sizeof(struct my_struct));
//    print_stats(cache);

    printf("\n\nAllocating 10 objects.\n");
    for (i = 0; i < 10; ++i) {
        my_object[i] = cache_get_object(cache);
        sprintf(my_object[i]->name, "Object #%d", i + 1);
        my_object[i]->age = i * 2;
        show_my_object(my_object[i]);
    }

  //  print_stats(cache);
  //  for (i = 0; i < 10; ++i) {
  //      show_my_object(my_object[i]);
  //  }
    
//    cache_put_object(my_object[5]);
//    cache_put_object(my_object[4]);
//    my_object[5] = 0;
//    my_object[4] = 0;
  //  print_stats(cache);
    
    
    printf("\n\nAllocating 5 objects.\n");
    for (i = 10; i < 15; ++i) {
        my_object[i] = cache_get_object(cache);
        sprintf(my_object[i]->name, "Object #%d", i + 1);
        my_object[i]->age = i * 2;
        show_my_object(my_object[i]);
    }

    printf("\n\nDeallocating 5 objects.\n");
    for (i = 5; i < 10; ++i)
        if (my_object[i])
            cache_put_object(my_object[i]);

    printf("\n\nAllocating 5 objects.\n");
    for (i = 5; i < 10; ++i) {
        my_object[i] = cache_get_object(cache);
        sprintf(my_object[i]->name, "Object #%d", i + 1);
        my_object[i]->age = i * 2;
        show_my_object(my_object[i]);
    }

    printf("\n\nDeallocating 10 objects.\n");
    for (i = 5; i < 15; ++i)
        if (my_object[i])
            cache_put_object(my_object[i]);

    printf("\n\nDeallocating 5 objects.\n");
    for (i = 0; i < 5; ++i)
        if (my_object[i])
            cache_put_object(my_object[i]);

    print_stats(cache);
    cache_destroy(cache);

    return 0;

}
