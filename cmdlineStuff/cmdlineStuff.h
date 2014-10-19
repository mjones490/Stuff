#ifndef CMD_LINE_STUFF_H
#define CMD_LINE_STUFF_H
#include "listStuff/listStuff.h"

struct parameter_struct {
	char    *name;
	char    short_name;
	char    type;
	void    **value;
	void    *default_value;
	char    *description;
};

struct argument_struct {
    char                    *value;
    struct list_head        list;
};

struct param_parse_struct {
    struct parameter_struct *param;
    int                     nr_params;
    struct argument_struct  *arg;
    struct list_head        *current_arg;
};

struct param_parse_struct *parse_command_line(struct parameter_struct *param, 
    int nr_params, int argc, char **argv);
void clean_up_params(struct param_parse_struct *param_parse);
char* get_next_argument(struct param_parse_struct *param_parse); 
void list_params(struct param_parse_struct *param_parse);

#define PARAM(LONG_NAME, SHORT_NAME, TYPE, VAR_ADDRESS, DEFAULT, DESC) { \
    LONG_NAME, \
    SHORT_NAME, \
    TYPE, \
    (void *) VAR_ADDRESS, \
    (void *) DEFAULT, \
    DESC \
},


#endif
