#include "list.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>






/************************************************************************************/
struct my_struct {
	char name[32];
	int n;
	struct list_head list;
};

void dump_list(struct list_head* list)
{
	struct my_struct* mine;
	struct list_head* tmp;

	printf("\nWalk list from head to last. . .\n");
	LIST_FOR_EACH(tmp, list) {
		mine = GET_ELEMENT(struct my_struct, list, tmp);
		printf("\tThis = %p  Next = %p  Prev = %p  N = %d  Name = \"%s\"\n", 
            mine, tmp->next, tmp->prev, mine->n, mine->name);
	}

	printf("\n");

}

int main()
{
	struct list_head list;
	struct list_head other_list;
	struct list_head* tmp;
	struct my_struct* mine;
	int i;

	printf("list test\nBy Mark\n\n");

	INIT_LIST_HEAD(&list);
	INIT_LIST_HEAD(&other_list);

	printf("Building list:\n");
	for (i = 0; i < 8; i++) {
		mine = malloc(sizeof(struct my_struct));
		printf("\tmine = %p  mine->list = %p\n", mine, &mine->list);
		sprintf(mine->name, "This is my list #%d.", i);
		mine->n = i * 2;
		list_add_tail(&mine->list, &list);
	}
	

	dump_list(&list);

	tmp = list.next->next;
	mine = GET_ELEMENT(struct my_struct, list, list.next->next);
	printf("\nRemoving %p from list. . .\n", mine);
    list_remove(&mine->list);
	free(mine);

	tmp = list.next->next;
	mine = GET_ELEMENT(struct my_struct, list, list.next->next);
	printf("\nRemoving %p from list. . .\n", mine);
    list_remove(&mine->list);
	free(mine);

	dump_list(&list);

	printf("\nAdding one more element to the list. . .\n");
	mine = malloc(sizeof(struct my_struct));
	sprintf(mine->name, "This is the last element added.");
	mine->n = 100;
	list_add(&mine->list, &list);

	dump_list(&list);

	printf("Moving items 3 , 4, and 5 to the other list\n");
	for (i = 0; i < 3; ++i) {
		tmp = list.next->next->next;
		list_move(tmp, &other_list);
	}

	printf("Moving top item to bottom of list\n");
	list_move_tail(list.next, &list);

	dump_list(&list);
	dump_list(&other_list);

	printf("Splicing both lists back together\n");
	list_splice(&other_list, &list);
	dump_list(&list);

	printf("\nNow backwards. . .\n");
	LIST_FOR_EACH_PREV(tmp, &list) {
		mine = GET_ELEMENT(struct my_struct, list, tmp);
		printf("\tThis = %p  Next = %p  Prev = %p  N = %d  Name = \"%s\"\n", mine, tmp->next, tmp->prev, mine->n, mine->name);
	}

	return 0;
} 
