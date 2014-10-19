#include <stdio.h>
#include <string.h>
#include "../macroStuff.h"
#include "cmdlineStuff.h"

char *source;
char *destination;
char *delete_mask;
int max_size_allowed;
BOOL dry_run;
BOOL bGetHelp;
BOOL bVerbose;
void a_test_func();

struct parameter_struct param[] = {
	{"source", 's', 'c', (void **) &source, (void *) "", "Full path of file to be copied into the directory"},
	{"destination", 0, 'c', (void**) &destination, "", "Destination directory for source"},
	{"delete-file-mask", 0, 'c', (void**) &delete_mask, "*", "Mask for files in root to delete"},
	{"max-size", 0, 'i', (void**) &max_size_allowed, (void*) 0, "Maximum space allowed"},
	{"dry-run", 0, 'b', (void**) &dry_run, FALSE, "Don't really delete anything"},
	{"help", 'h', 'b', (void**) &bGetHelp, (void*) FALSE, "Show this help screen"},
	{"", 'v', 'b', (void**) &bVerbose, FALSE, "\tMaximum verbosity"},
};

void show_help(struct param_parse_struct *param_parse)
{
	printf("\nTestbed for cmdlineStuff.\n\n");
    list_params(param_parse);
	printf("\n");
}

int main(int argc, char **argv)
{
    struct param_parse_struct *param_parse = 
        parse_command_line(param, ARRAY_SIZE(param), argc, argv);
    char *value;


    if (bGetHelp)
        show_help(param_parse);

    if (bVerbose) {
		printf("dry_run = %s\n", dry_run? "TRUE" : "FALSE");
		printf("source = %s\n", source);
		printf("destination = %s\n", destination);
		printf("max_size_allowed = %d\n", max_size_allowed);
		printf("bGetHelp = %s\n", bGetHelp? "TRUE" : "FALSE");
		printf("bVerbose = %s\n", bVerbose? "TRUE" : "FALSE");
		printf("delete_file_mask = %s\n", delete_mask);
		printf("\n\n");
    }

    printf("Left over arguments:\n");
    while ((value = get_next_argument(param_parse)))
        printf("\t%s\n", value);


    clean_up_params(param_parse);
    return 0;
};

