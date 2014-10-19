#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "cacheStuff.h"
/***
 * FUNCTION:    struct cachePage *get_page()
 * PARAMETERS:  none
 * RETURNS:     struct cachePage
 * DESCRIPTION: Map and initializes a new page.
 ***/ 
struct cachePage *get_page()
{
    int last_errno;
    struct cachePage *page = mmap(0, getpagesize(), PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (-1 == (int) page) {
        last_errno = errno;
        exit(EXIT_FAILURE);
    }

    INIT_LIST_HEAD(&page->list);
    INIT_LIST_HEAD(&page->partial_pages);
    INIT_LIST_HEAD(&page->free_list);
    page->flags = 0;
    
    return page;
}

#ifdef OBJ_TEST
#define calc_end (void *) obj_start + (size_t) cache->obj_size * OBJ_TEST
#include <stdio.h>
#else
#define calc_end (void *) page + getpagesize()
#define printf(format, ...)
#endif

/***
 * FUNCTION:    add_page            
 * PARAMETERS:  struct cache *      - Cache to add page to
 *              struct cachePage *  - Page to add
 *              void *              - First object in this page
 * RETURNS:     void
 * DESCRIPTION: Adds page to cache.  Adds free objects to cache.  Updates stats.
 ***/ 
static inline void add_page(struct cache *cache, struct cachePage *page, void *obj_start)
{
    void *obj_end = calc_end;
    void *current_obj;

    printf("Adding page %p into cache %p.\n", page, cache);
    page->cache_header = cache;
    page->nr_objs = 0;
    page->nr_objs_allocated = 0;
    page->first_obj = obj_start;
    ++cache->nr_pages;
    if (cache->nr_pages > cache->max_nr_pages)
        ++cache->max_nr_pages;
    if (NULL == cache->first_page)
        cache->first_page = page;
    else    
        list_add_tail(&page->list, &cache->first_page->list);

    for (current_obj = obj_start; (current_obj + cache->obj_size) < obj_end; 
        current_obj += cache->obj_size) {
        list_add_tail((struct list_head *) current_obj, &page->free_list);
        ++cache->nr_objs;
        ++page->nr_objs;
    }
}

/***
 * FUNCTION:    put_page
 * PARAMETERS:  struct cachePage *  - Page to unmap
 * RETURNS:     void
 * DESCRIPTION: Unmaps page and updates cache stats.
 ***/ 
static inline void put_page(struct cachePage *page)
{
    printf("Putting page %p.\n", page);
    if (page->cache_header->first_page != page) {
        page->cache_header->nr_objs -= page->nr_objs;
        --page->cache_header->nr_pages;
        munmap(page, getpagesize());
    }
}

/***
 * FUNCTION:    cache_create    
 * PARAMETERS:  size_t obj_size - Size in bytes of object held in cache
 * RETURNS:     struct cache
 * DESCRIPTION: Allocates one page for a new cache. Inits cache structure.
 *                Adds the page to the cache.
 ***/ 
struct cache *cache_create(size_t obj_size)
{
    struct cache *cache_header;
    struct cachePage *page = get_page();
    cache_header = (void *) page + sizeof(struct cachePage);
    cache_header->first_page = NULL;
    cache_header->obj_size = obj_size;
    cache_header->nr_objs_allocated = 0;
    cache_header->max_objs_allocated = 0;
    cache_header->max_nr_pages = 0;
    cache_header->free_page = page;
    INIT_LIST_HEAD(&cache_header->partial_pages);
    add_page(cache_header, page, (void *)cache_header + sizeof(struct cache));
    
    printf("Cache %p created.\n", cache_header);    
    return cache_header;
}

/***
 * FUNCTION:    cache_destroy    
 * PARAMETERS:  struct cache * - Cache to destroy
 * RETURNS:     void
 * DESCRIPTION: Unmaps all pages in cache, and then the cache itself.
 ***/ 
void cache_destroy(struct cache *cache_header) 
{
    struct list_head *current;
    struct list_head *next;
    struct cachePage *page;

    printf("Destroying cache %p.\n", cache_header);
    LIST_FOR_EACH_SAFE(current, next, &cache_header->first_page->list) {
        page = GET_ELEMENT(struct cachePage, list, current);
        put_page(page);
    }
    
    page = cache_header->first_page;
    cache_header->first_page = NULL;
    put_page(page);
    
}

/***
 * FUNCTION:    cache_get_object
 * PARAMETERS:  struct cache * - Cache to retrieve object from
 * RETURNS:     void * 
 * DESCRIPTION: Removes first free object from free list and returns it to the caller.
 *                Updates cache stats.
 ***/ 
void *cache_get_object(struct cache *cache) 
{
    struct cachePage *page;
    void *obj;
    
    if (LIST_IS_EMPTY(cache->partial_pages)) {
        printf("Finding an empty page.\n");
        if (NULL == cache->free_page) {
            printf("Creating a new page.\n");
            page = get_page();
            add_page(cache, page, page + sizeof(struct cachePage));
        } else {
            printf("Using free page %p.\n", cache->free_page);
            page = cache->free_page;
            cache->free_page = NULL;
        }
        printf("Adding list to partial pages...");
        list_add_tail(&page->partial_pages, &cache->partial_pages);
        printf("done!\n");
    } else {
        page = GET_ELEMENT(struct cachePage, partial_pages, cache->partial_pages.next);
        printf("Using partial page %p.\n", page);
    }

    obj = page->free_list.next;
    list_remove((struct list_head *) obj);
    ++page->nr_objs_allocated;
    if (++cache->nr_objs_allocated > cache->max_objs_allocated)
        cache->max_objs_allocated++;

    if (page->nr_objs_allocated == page->nr_objs) {
        printf("Page %p full.  Remove from free list.\n", page);
        list_remove(&page->partial_pages);
    }

    return obj;
}

/***
 * FUNCTION:    cache_put_object    
 * PARAMETERS:  void * - Object to return
 * RETURNS:     void
 * DESCRIPTION: Puts object back into free list. Updates cache stats.
 ***/ 
void cache_put_object(void *obj) 
{
    struct cachePage *page = find_page(obj);
    list_add_tail((struct list_head *) obj, &page->free_list);
    if (page->nr_objs_allocated == page->nr_objs) {
        printf("Page %p not full anymore. Add to partial list.\n", page);
        list_add_tail(&page->partial_pages, &page->cache_header->partial_pages);
    }
    --page->cache_header->nr_objs_allocated;
    --page->nr_objs_allocated;

    if (0 == page->nr_objs_allocated) {
        printf("Page %p empty. Remove from partial pages.\n", page);
        list_remove(&page->partial_pages);
        if (NULL != page->cache_header->free_page) {
            if (page->cache_header->first_page == page) {
                printf("First page in cache.  Swap with currnet free page.\n");
                page = page->cache_header->free_page;
                page->cache_header->free_page = page->cache_header->first_page;
            }
            list_remove(&page->list);
            put_page(page);
        } else {
            printf("Set to cache free page.\n");
            page->cache_header->free_page = page;
        }
    }
};

