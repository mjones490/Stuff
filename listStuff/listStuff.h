#ifndef __LIST_H__
#define __LIST_H__

//#define inline __inline

struct list_head {
	struct list_head* prev;
	struct list_head* next;
};

static inline void list_add(struct list_head* new_list, struct list_head* head)
{
	new_list->prev = head;
	new_list->next = head->next;
	head->next = new_list;
	new_list->next->prev = new_list;
}

static inline void list_add_tail(struct list_head* new_list, struct list_head* head)
{
	list_add(new_list, head->prev);
}

static inline void list_remove(struct list_head* old_list)
{
	old_list->prev->next = old_list->next;
	old_list->next->prev = old_list->prev;
}

static inline void list_move(struct list_head* list, struct list_head* head)
{
	list_remove(list);
	list_add(list, head);
}

static inline void list_move_tail(struct list_head* list, struct list_head* head)
{
	list_remove(list);
	list_add_tail(list, head);
}

static inline void list_splice(struct list_head* tail, struct list_head* head)
{	
	tail->next->prev = head->prev;
	tail->prev->next = head;

	head->prev->next = tail->next;
	head->prev = tail->prev;
}

#define INIT_LIST_HEAD(LIST) \
	do { \
        (LIST)->prev = LIST; \
	    (LIST)->next = LIST; \
    } while (0)

#define LIST_FOR_EACH(ITERATOR, LIST) \
	for (ITERATOR = (LIST)->next; ITERATOR != (LIST); ITERATOR = ITERATOR->next)

#define LIST_FOR_EACH_SAFE(ITERATOR, NEXT, LIST) \
	for (ITERATOR = (LIST)->next, NEXT = ITERATOR->next; \
		ITERATOR != (LIST); \
		ITERATOR = NEXT, NEXT = NEXT->next)

#define LIST_FOR_EACH_PREV(ITERATOR, LIST) \
	for (ITERATOR = (LIST)->prev; ITERATOR != (LIST); ITERATOR = ITERATOR->prev)

#define LIST_FOR_EVERY(ITERATOR, LIST, FIRST) \
    for (FIRST = 0, ITERATOR = (LIST); \
        ITERATOR != FIRST; \
        FIRST = (FIRST == 0? (LIST) : FIRST), \
        ITERATOR = ITERATOR->next)

#define OFFSET_OF(STRUCTURE, MEMBER) \
	(size_t)(&((STRUCTURE*)0)->MEMBER)

#define GET_ELEMENT(STRUCTURE, MEMBER, LIST) \
	(STRUCTURE*)((char*)(LIST) - OFFSET_OF(STRUCTURE, MEMBER))

#define LIST_IS_EMPTY(LIST) \
	((&LIST) == LIST.next)

#endif // __LIST_H__

