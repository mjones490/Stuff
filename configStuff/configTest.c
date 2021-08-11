#include <stdio.h>
#include "listStuff.h"
#include "configStuff.h"

int main(int argv, char **argc)
{
    struct config_struct *config;
    const char *first_section;
    const char *section;
    const char *first_key;
    const char *key;

    printf("configTest, by Mark\n\n");

    printf("Opening config...\n");
    config = config_open("test.conf");

    printf("--------------------------------------------------\n");
    config_next_section(config);
    first_section = config_section(config);
    section = first_section;
    do {
        printf("%s\n", section);

        config_next_key(config);
        first_key = config_key(config);
        key = first_key;
        do {
            printf("\t%s = %s\n", key, config_value(config));
            config_next_key(config); 
            key = config_key(config);
        } while (first_key != key);
        config_next_section(config);
        section = config_section(config);
    } while (first_section != section);
    
    
    printf("--------------------------------------------------\n");
    printf("Closing config...\n");
    config_close(config);

    return 0;
}
