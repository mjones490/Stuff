#ifndef CONFIG_STUFF_H
#define CONFIG_STUFF_H
#include "configStructs.h"

struct config_struct *config_open(const char *pathname);
void config_close(struct config_struct *config);

const char *config_value(struct config_struct *config);
const char *config_key(struct config_struct *config);
const char *config_section(struct config_struct *config);

void config_next_section(struct config_struct *config);
void config_next_key(struct config_struct *config);

struct key_struct *find_key(struct key_struct *start, char *name);
struct section_struct *find_section(struct section_struct *start, char *name);
#endif
