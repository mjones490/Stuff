#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "listStuff.h"

struct variable;

struct variable_ops {
    void (*destroy)(struct variable *v);
    char *(*to_string)(struct variable *v);
};

struct variable {
    char                *name;      // Name of variable
    void                *data;      // Value (whether direct or pointer
    struct variable_ops *ops;       // Operations for this variable
    struct list_head    list;       // List of variables
};

struct variable *create_variable(char *name, struct variable_ops *ops, struct list_head *list)
{
    struct variable *v;
    size_t len;

    v = malloc(sizeof(struct variable));
    v->data = NULL;
    v->ops = ops;
    len = strlen(name);
    v->name = malloc(len + 1);
    strncpy(v->name, name, len);
    list_add_tail(&v->list, list);

    return v;    
}

void destroy_variable(struct variable *v)
{
    list_remove(&v->list);
    if (NULL != v->ops->destroy)
        v->ops->destroy(v);
    free(v->name);
    free(v);
}

char *to_string(struct variable *v)
{
    char *value = "";
    if (NULL != v->ops->to_string)
        value = v->ops->to_string(v);
    return value;
}

//--------------------------------------------------
void destroy_string_variable(struct variable *v) { free(v->data);
}

char *string_to_string(struct variable *v)
{
    return v->data;
}

struct variable_ops string_variable_ops = {
    destroy_string_variable,
    string_to_string
};

struct variable *create_string_variable(char *name, char *value, struct list_head *list) 
{
    struct variable *v;
    size_t len;

    v = create_variable(name, &string_variable_ops, list); 

    len = strlen(value);
    v->data = malloc(len + 1);
    strncpy(v->data, value, len);
    return v;

}
//--------------------------------------------------
char *int_to_string(struct variable *v)
{
    static char str[16];
    sprintf(str, "%d", (int) v->data);
    return str;
}

struct variable_ops int_variable_ops = {
    0,
    int_to_string
};

struct variable *create_int_variable(char *name, int value, struct list_head *list)
{
    struct variable *v;
    v = create_variable(name, &int_variable_ops, list);
    v->data = (void *) value;
    return v;
}

//--------------------------------------------------
int main(int argc, char **argv)
{
    struct list_head list;
    struct list_head *i, *j;
    struct variable *v;

    printf("varStuff Test\nBy Mark\n\n");

    INIT_LIST_HEAD(&list);

    create_int_variable("Mark", 42, &list);
    create_int_variable("Tonya", 40, &list);
    create_int_variable("Janae", 12, &list);
    create_int_variable("Warren", 8, &list);
    create_string_variable("Sandee", "dog", &list);
    create_string_variable("Q", "cat", &list);

    LIST_FOR_EACH(i, &list) {
        v = GET_ELEMENT(struct variable, list, i);
        printf("%s = %s\n", v->name, to_string(v));
    }

    printf("\n");

    LIST_FOR_EACH_SAFE(i, j, &list) {
        v = GET_ELEMENT(struct variable, list, i);
        destroy_variable(v);
    }
    
    return 0;
}
