#ifndef CONFIG_STRUCTS_H
#define CONFIG_STRUCTS_H


struct key_struct {
    char                *name;
    char                *value;
    struct list_head    list;
};

struct section_struct {
    char                *name;
    struct list_head    list;
    struct key_struct   *key;
};

struct config_struct {
    struct section_struct   *section;
    char                    *pathname;
};

struct key_struct *create_key(struct key_struct *prev_key, 
    char *name, char *value);
void destroy_key(struct key_struct *key);
struct section_struct *create_section(struct section_struct *prev_struct, 
    char *name);
void destroy_section(struct section_struct *section);

#endif
