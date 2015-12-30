#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#undef _POSIX_C_SOURCE
#include <regex.h>

#include "macroStuff.h"
#include "listStuff.h"
#include "configStuff.h"

#define MAX_REC_SIZE        81
#define MAX_PARSED_FIELDS   8

/****************************************************************************
 * Section and Key functions
 ****************************************************************************/
struct key_struct *create_key(struct key_struct *prev_key, 
    char *name, char *value);
void destroy_key(struct key_struct *key);
struct section_struct *create_section(struct section_struct *prev_section, 
    char *name);
void destroy_section(struct section_struct *section);


struct key_struct *create_key(struct key_struct *prev_key, 
    char *name, char *value)
{
    struct key_struct *key = NULL;
    
    if (NULL != prev_key) 
        key = find_key(prev_key, name);

    if (NULL == key) { 
        key = malloc(sizeof(struct key_struct));
        key->name = malloc(strlen(name) + 2);
        strcpy(key->name, name); 
    } else {
        free(key->value);
        list_remove(&key->list);
    }

    key->value = malloc(strlen(value) + 2);
    strcpy(key->value, value);

    if (NULL == prev_key) 
        INIT_LIST_HEAD(&key->list);
    else 
        list_add(&key->list, &prev_key->list);    

    return key;
}

void destroy_key(struct key_struct *key)
{
    if (NULL != key->name)
        free (key->name);

    if (NULL != key->value)
        free(key->value);

    list_remove(&key->list);
    free(key);
}

struct key_struct *find_key(struct key_struct *start, char *name)
{
    struct key_struct *found = start;
    struct list_head *list;

    if (0 != strcmp(start->name, name)) {
        LIST_FOR_EACH(list, &start->list) {
            found = GET_ELEMENT(struct key_struct, list, list);
            if (0 == strcmp(found->name, name))
                break;
        }

        if (list == &start->list)
            found = NULL;
    }

    return found;
}

struct section_struct *create_section(struct section_struct *prev_section, 
    char *name)
{
    struct section_struct *section = NULL;

    if (NULL != prev_section)
        section = find_section(prev_section, name);

    if (NULL == section) {
        section = malloc(sizeof(struct section_struct));
        section->name = malloc(strlen(name) + 2);
        strcpy(section->name, name);
        section->key = NULL;
    } else {
        list_remove(&section->list);
    }

    if (NULL == prev_section) 
        INIT_LIST_HEAD(&section->list);
    else 
        list_add(&section->list, &prev_section->list);

    return section;
}

void destroy_section(struct section_struct *section)
{
    struct list_head *current, *next;
    struct key_struct *key;

    if (NULL != section->name)
        free(section->name);

    LIST_FOR_EACH_SAFE(current, next, &section->key->list) {
        key = GET_ELEMENT(struct key_struct, list, current);
        destroy_key(key);
    }
        
    destroy_key(section->key);

    free(section);
}

struct section_struct *find_section(struct section_struct *start, char *name)
{
    struct section_struct *found = start;
    struct list_head *list;

    if (0 != strcmp(start->name, name)) {
        LIST_FOR_EACH(list, &start->list) {
            found = GET_ELEMENT(struct section_struct, list, list);
            if (0 == strcmp(found->name, name))
                break;
        }

        if (list == &start->list)
            found = NULL;
    }

    return found;
}

/****************************************************************************/
/* Loading/Saving functions
 ****************************************************************************/
#define LINETYPE_EMPTY      0
#define LINETYPE_KEY        1
#define LINETYPE_SECTION    2

struct re_pattern_struct {
    short   line_type;
    char    *pattern_string;
}; 

struct re_pattern_struct re_pattern[] = {
    {LINETYPE_EMPTY,    "^[ \t]*#.*\n"},
    {LINETYPE_KEY,      "^[ \t]*\\(.*\\)=\\(.*\\)#+.*\n$"},
    {LINETYPE_KEY,      "^[ \t]*\\(.*\\)=\\(.*\\)\n$"},
    {LINETYPE_SECTION,  "\\[\\(.*\\)\\]\n"}
};


#define NR_PATTERNS ARRAY_SIZE(re_pattern) 
#define MAX_MATCHES 8

static regex_t expression[NR_PATTERNS];

short parse_record(char *record, char **field_buffer, char *parsed_field[])
{
    int i;
    short match_pattern = LINETYPE_EMPTY;
    int pattern_num;
    size_t buf_len = 0;
    char *buf_end;
    regmatch_t match[MAX_MATCHES];
    
    // Free any previous field buffers
    if (0 != *field_buffer) {
        free(*field_buffer);
        *field_buffer = 0;
    }

    // Look for a match    
    for (i = 0; i < NR_PATTERNS; ++i) {
        if (0 == regexec(&expression[i], record, expression[i].re_nsub + 1,
            match, 0)) {
    //        printf("%s matches pattern %s\n", record, 
      //          re_pattern[i].pattern_string);
            break;
        }
    }
   
    if (i < NR_PATTERNS)        
    {
        // Match found!
        match_pattern = re_pattern[i].line_type;
        pattern_num = i;
        for (i = 1; i <= expression[pattern_num].re_nsub; ++i) 
           buf_len += (match[i].rm_eo - match[i].rm_so) + 2;

        buf_end = malloc(buf_len);
        *field_buffer = buf_end; 

        for (i = 1; i <= expression[pattern_num].re_nsub; ++i) {
            buf_len = (match[i].rm_eo - match[i].rm_so);
            strncpy(buf_end, record + match[i].rm_so, buf_len);
            parsed_field[i - 1] = buf_end;
            buf_end += buf_len;
            *(buf_end++) = 0;
        }
    }
   
   return match_pattern;
}

struct section_struct *load_file(FILE *fp)
{
    char *record = NULL;
    size_t read_size = 0;
    struct section_struct *section = NULL;
    char *parsed_field[MAX_PARSED_FIELDS];
    char *field_buffer = NULL;
    short pattern_type;

    // Read each record
    while (-1 != getline(&record, &read_size, fp)) {
        if (0 < read_size) {
            pattern_type = parse_record(record, &field_buffer, parsed_field);
            switch (pattern_type) {
            case LINETYPE_KEY:
                section->key = create_key(section->key, parsed_field[0], 
                    parsed_field[1]);
                break;

            case LINETYPE_SECTION:
                section = create_section(section, parsed_field[0]);
                break;
                
            default:
                break; 
            }
        }
    }

    if (record)
        free(record);

    if (field_buffer)
        free(field_buffer);

    return section;
}

#ifdef DEBUG_OUT
void find_section_test(struct config_struct *config, char *name)
{
    struct section_struct *test_section = NULL;
    printf("Searching for secton %s. . .", name);
    test_section = find_section(config->section, name);
    if (NULL == test_section) {
        printf("Not found.\n");
    } else { 
        printf("Found.\n");
        config->section = test_section;        
    }
}
void find_key_test(struct config_struct *config, char *name)
{
    struct key_struct *test_key = NULL;
    printf("Searching for key %s in section %s. . .", name, 
        config->section->name);
    test_key = find_key(config->section->key, name);
    if (NULL == test_key) {
        printf("Not found.\n");
    } else { 
        printf("Found.  Value = %s\n", test_key->value);
        config->section->key = test_key;
    }
}
#endif
/****************************************************************************
 * Interface functions
 ****************************************************************************/
struct config_struct *config_open(const char *pathname)
{
    FILE *fp = fopen(pathname, "r");
    struct config_struct *config = NULL;
 
    int i = 0;

    if (NULL == fp)
        goto out;

    config = malloc(sizeof(struct config_struct));
    config->pathname = malloc(strlen(pathname) + 1);
    strcpy(config->pathname, pathname);

    for (i = 0; i < NR_PATTERNS; ++i) 
        regcomp(&expression[i], re_pattern[i].pattern_string, 0);

    config->section = load_file(fp);
    fclose(fp);


#ifdef DEBUG_OUT
    printf("Tesing internal find functions:\n");
    find_key_test(config, "fred");
    find_key_test(config, "bob");
    find_key_test(config, "bill");
    find_key_test(config, "hank");
    find_section_test(config, "bill");
    find_section_test(config, "jack");
    find_key_test(config, "fred");
    find_key_test(config, "bob");
    find_key_test(config, "bill");
    find_key_test(config, "hank");
    find_key_test(config, "hank");
    find_section_test(config, "jack");
    find_section_test(config, "SECTION_TWO");
    find_section_test(config, "goober");
    find_key_test(config, "LAST_LINE");

    printf("\n\nValue test...\n");
    config->section = find_section(config->section, "jack");
    printf("Hank = %s\n", find_key_string(config, "hank"));
#endif

out:
    return config;
}

void config_close(struct config_struct *config) 
{
    int i;
    struct list_head *current, *next;
    struct section_struct *section;

    free(config->pathname);
    
    LIST_FOR_EACH_SAFE(current, next, &config->section->list) {
        section = GET_ELEMENT(struct section_struct, list, current);
        destroy_section(section);
    }
    
    destroy_section(config->section);

    for (i = 0; i < NR_PATTERNS; ++i)
        regfree(&expression[i]);

    free(config);
}

const char *config_section(struct config_struct *config)
{
    return config->section->name;
}

const char *config_key(struct config_struct *config)
{
    return config->section->key->name;    
}

const char *config_value(struct config_struct *config)
{
    return config->section->key->value;
}

void config_next_section(struct config_struct *config)
{
    config->section = 
        GET_ELEMENT(struct section_struct, list, config->section->list.next);
}

void config_next_key(struct config_struct *config)
{
    config->section->key =
        GET_ELEMENT(struct key_struct, list, config->section->key->list.next);
}
