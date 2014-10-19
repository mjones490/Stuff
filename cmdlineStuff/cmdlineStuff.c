#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include "macroStuff.h"
#include "cmdlineStuff.h"


struct param_parse_struct *create_param_parse(
    struct parameter_struct *param, int nr_params)
{
    struct param_parse_struct *param_parse =
        malloc(sizeof(struct param_parse_struct));

    param_parse->param = param;
    param_parse->nr_params = nr_params;
    param_parse->arg = NULL;

    return param_parse;
}

void destroy_param_parse(struct param_parse_struct *param_parse)
{
    free(param_parse);
}

void remove_argument(struct list_head *current)
{
    struct argument_struct *argument;
    argument = GET_ELEMENT(struct argument_struct, list, current);
    list_remove(&argument->list);
    free(argument);    
}

/***
 * FUNCTION:    
 * PARAMETERS:  
 * RETURNS:     
 * DESCRIPTION: 
 ***/ 
static BOOL  set_param(struct param_parse_struct *param_parse, 
    char* name, char* value)
{
	int i;
	int name_length = strlen(name);
    struct argument_struct *argument;
    struct list_head *current;
    BOOL found_name = FALSE;

	for (i = 0; i < param_parse->nr_params; ++i) {
		if (name_length == 1 && 
            param_parse->param[i].short_name != name[0])
			continue;			
		else if (name_length > 1 && 0 != 
            strcmp(param_parse->param[i].name, name))
			continue;

        found_name = TRUE;

        // Get next good argument if value is null.
        if (value == NULL && strchr("cil", param_parse->param[i].type)) {
            LIST_FOR_EACH(current, param_parse->current_arg) {
                argument = GET_ELEMENT(struct argument_struct, list, current);

                // Named argument?
                if ('-' == *argument->value)
                    continue;

                // Back at first argument?
                if (argument == param_parse->arg) 
                    break;

                // Argument found! Mark for deletion on next cycle
                value = argument->value;
                argument->value = NULL;
                break;                
            }
        }

		switch (param_parse->param[i].type)
		{
		case 'c':
			if (NULL != value) {
				*(param_parse->param[i].value) = 
                    malloc(strlen(value) + 1);
				strcpy(*(param_parse->param[i].value), value);		
			} else {
				*(param_parse->param[i].value) = NULL;
			}
			break;

		case 'b':
			*param_parse->param[i].value = (void*) 
                (!(BOOL) param_parse->param[i].default_value);
			break;

		case 'i':
			sscanf(value, "%d", (int *) param_parse->param[i].value);
			break;

		case 'l':
			// TODO: fix to input long
            sscanf(value, "%d", (int *) param_parse->param[i].value);
			break;

		default:
			break;
		}
	}

    return found_name;
}

/***
 * FUNCTION:    
 * PARAMETERS:  
 * RETURNS:     
 * DESCRIPTION: 
 ***/
struct param_parse_struct *parse_command_line(struct parameter_struct *param, 
    int nr_params, int argc, char** argv)
{
    int i;
	char* name;
	char* value;
	char* splitter;
	char flag[2] = {0, 0};
    BOOL found_name;
    struct list_head *next;
    struct argument_struct *argument;
    struct param_parse_struct *param_parse = 
        create_param_parse(param, nr_params);

    // Initialize default values
	for (i = 0; i < param_parse->nr_params; ++i) {
		*(param_parse->param[i].value) = 
            param_parse->param[i].default_value;
	}

    // Load argument list
    for (i = 0; i < argc; ++i) {
        argument = malloc(sizeof(struct argument_struct));
        argument->value = argv[i];
        if (NULL == param_parse->arg) {
            param_parse->arg = argument;
            INIT_LIST_HEAD(&param_parse->arg->list);
        }
        list_add_tail(&argument->list, &param_parse->arg->list);
    }

    // Traverse list, process each argument
	LIST_FOR_EACH_SAFE(param_parse->current_arg, next, &param_parse->arg->list) {
		argument = GET_ELEMENT(struct argument_struct, list, param_parse->current_arg);

        // Is argument marked for removal?
        if (NULL == argument->value) {
            remove_argument(param_parse->current_arg);
            continue;
        }

        if (*argument->value == '-') {
            if  (*(argument->value + 1) == '-') {
			    // Argument begins with "--"
                name = argument->value + 2;
			    splitter = strchr(name, '=');
			    if (NULL != splitter) {
				    *splitter = 0;
				    value = splitter + 1;			
			    } else {
				    value = NULL;
			    }
			    found_name = set_param(param_parse, name, value);
		    } else {
                // Argument begins with "-"
			    name = argument->value + 1;
			    while (*name) {
				    flag[0] = *(name++);
				    found_name = set_param(param_parse, flag, NULL);
			    }
            }			

            // Since this was a named argument, remove it from the list.
            if (TRUE == found_name)
                remove_argument(param_parse->current_arg);
		} else {
			continue;
		}

	}

    return param_parse;
}

/***
 * FUNCTION:    
 * PARAMETERS:  
 * RETURNS:     
 * DESCRIPTION: 
 ***/ 
void clean_up_params(struct param_parse_struct *param_parse)
{
    struct list_head *current, *next;

	int iparams;
	for (iparams = 0; NULL != param_parse->param[iparams].name; ++iparams) {
		if (param_parse->param[iparams].type == 'c' && 
            NULL != *(param_parse->param[iparams].value) && 
			*(param_parse->param[iparams].value) != 
            param_parse->param[iparams].default_value)
			free(*(param_parse->param[iparams].value));
	}


    LIST_FOR_EACH_SAFE(current, next, &param_parse->arg->list)
        remove_argument(current);

    free(param_parse->arg);
    destroy_param_parse(param_parse);
}

char* get_next_argument(struct param_parse_struct *param_parse) 
{
    char *value = NULL;
    struct argument_struct *argument;

    argument = GET_ELEMENT(struct argument_struct, list, param_parse->arg->list.next);
    if (argument != param_parse->arg) {
        value = argument->value;
        remove_argument(&argument->list);
    }    
    
    return value;
}

/***
 * FUNCTION:    
 * PARAMETERS:  
 * RETURNS:     
 * DESCRIPTION: 
 ***/ 
void list_params(struct param_parse_struct *param_parse)
{
	int iparams;
	char full_name[32];
	char* name_build;

	for (iparams = 0; iparams < param_parse->nr_params; ++iparams) {
		name_build = full_name;
		if (strlen(param_parse->param[iparams].name))
			name_build += sprintf(name_build, "  --%s", 
                param_parse->param[iparams].name);

		if (param_parse->param[iparams].short_name)
			name_build += sprintf(name_build, "  -%c", 
                param_parse->param[iparams].short_name);

		printf("%s\t%s\n", full_name, param_parse->param[iparams].description);
	}
}
