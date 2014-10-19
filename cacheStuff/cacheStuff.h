#ifndef __CACHE_STUFF_H__
#define __CACHE_STUFF_H__

#include <unistd.h>
#include "listStuff/listStuff.h"

struct cachePage;   // Forward declaration for struct cache 
struct cache {
    char                name[16];
    struct cachePage    *first_page;
    struct cachePage    *free_page;
    struct list_head    partial_pages;
    size_t              obj_size;
    unsigned int        nr_pages;
    unsigned int        max_nr_pages;
    unsigned int        nr_objs;
    unsigned int        nr_objs_allocated;
    unsigned int        max_objs_allocated;
};

#define PAGE_ZEROTH     0x01    // Page is zeroth in cache

struct cachePage {
    struct cache        *cache_header;
    struct list_head    list;
    struct list_head    partial_pages;
    void                *first_obj;
    struct list_head    free_list;
    unsigned int        nr_objs;
    unsigned int        nr_objs_allocated;
    unsigned long       flags;
};

static inline void *find_page(void *obj)
{
    return (void *) ((size_t) obj & (size_t) -getpagesize());
}

void cache_put_object(void *obj);
void *cache_get_object(struct cache *cache);
struct cache *cache_create(size_t obj_size);
void cache_destroy(struct cache *cache_header); 

#endif
